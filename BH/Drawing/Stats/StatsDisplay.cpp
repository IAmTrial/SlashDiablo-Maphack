#include "StatsDisplay.h"

#include <wchar.h>
#include <windows.h>

#include <algorithm>
#include <format>
#include <list>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "bh/common/string_util.hpp"
#include "../../BH.h"
#include "../../Common.h"
#include "../../Config.h"
#include "../../Constants.h"
#include "../../D2Ptrs.h"
#include "../../D2Strings.h"
#include "../../D2Structs.h"
#include "../../MPQInit.h"
#include "../Basic/Boxhook/Boxhook.h"
#include "../Basic/Framehook/Framehook.h"
#include "../Basic/Texthook/Texthook.h"
#include "../Hook.h"

namespace {

using ::common::str_util::Trim;
using ::common::str_util::wide::FromUtf8;

enum class QuestBugState {
	kIncomplete = 0,
	kNotBugged = 1,
	kBugged = 2,
};

static const wchar_t* ToString(QuestBugState questBugState) {
	switch (questBugState) {
		case QuestBugState::kIncomplete: {
			static const std::wstring kBugText =
					GetColorCode(TextColor::White) + L"???";
			
			return kBugText.c_str();
		}

		case QuestBugState::kNotBugged: {
			static const std::wstring kBugText =
					GetColorCode(TextColor::Red) + D2LANG_GetLocaleText(D2STR_NO);

			return kBugText.c_str();
		}

		case QuestBugState::kBugged: {
			static const std::wstring kBugText =
					GetColorCode(TextColor::Green) + D2LANG_GetLocaleText(D2STR_YES);
			
			return kBugText.c_str();
		}
	}
}

static QuestBugState GetAndarielBuggedState(void* questInfo) {
	BOOL isRewardGranted =
			D2COMMON_GetQuestFlag(
					questInfo,
					SISTERS_TO_THE_SLAUGHTER,
					QFLAG_REWARD_GRANTED);
	BOOL isRewardPending =
			D2COMMON_GetQuestFlag(
					questInfo,
					SISTERS_TO_THE_SLAUGHTER,
					QFLAG_REWARD_PENDING);
	// Andariel has not been killed yet.
	if (!(isRewardGranted || isRewardPending)) {
		return QuestBugState::kIncomplete;
	}

	BOOL isQuestMarkedComplete =
			D2COMMON_GetQuestFlag(
					questInfo,
					SISTERS_TO_THE_SLAUGHTER,
					QFLAG_QUEST_COMPLETED_BEFORE);
	return isQuestMarkedComplete
			? QuestBugState::kNotBugged
			: QuestBugState::kBugged;
}

static QuestBugState GetDurielBuggedState(void* questInfo) {
	BOOL isUpdateQuestLog =
			D2COMMON_GetQuestFlag(
					questInfo,
					THE_SEVEN_TOMBS,
					QFLAG_UPDATE_QUEST_LOG);
	BOOL isQuestCompletedBefore =
			D2COMMON_GetQuestFlag(questInfo,
					THE_SEVEN_TOMBS,
					QFLAG_QUEST_COMPLETED_BEFORE);

	// Duriel has not been killed yet.
	if (!(isUpdateQuestLog || isQuestCompletedBefore)) {
		return QuestBugState::kIncomplete;
	}

	BOOL isQuestMarkedComplete =
			D2COMMON_GetQuestFlag(questInfo, THE_SEVEN_TOMBS, QFLAG_CUSTOM_1);
	return isQuestMarkedComplete
			? QuestBugState::kNotBugged
			: QuestBugState::kBugged;
}

}  // namespace

namespace Drawing {

StatsDisplay *StatsDisplay::display;

StatsDisplay::StatsDisplay(std::string name) {
	int yPos = 10;
	int width = 240;

	InitializeCriticalSection(&crit);
	SetY(yPos);
	SetXSize(width);

	LoadConfig();

	SetName(name);
	SetActive(true);
	SetMinimized(true);

	BH::config->ReadKey("Character Stats", "VK_8", statsKey);
	display = this;
}

StatsDisplay::~StatsDisplay() {
	Lock();
	// Remove all hooks associated with the display
	while (Hooks.size() > 0) {
		delete (*Hooks.begin());
	}
	Unlock();
	DeleteCriticalSection(&crit);
}

void StatsDisplay::LoadConfig(){
	int height = 342 + 8 * 3 + 16 * 6;
	customStats.clear();

	BH::config->ReadToggle("Stats on Right", "None", false, Toggles["Stats on Right"]);

	std::vector<std::pair<std::string, std::string>> stats;
	BH::config->ReadMapList("Stat Screen", stats);
	for (unsigned int i = 0; i < stats.size(); i++) {
		std::transform(stats[i].first.begin(), stats[i].first.end(), stats[i].first.begin(), ::tolower);
		if (StatMap.empty()) {
			continue;
		}

		StatProperties *sp = StatMap[stats[i].first];
		DisplayedStat customStat;
		customStat.name = stats[i].first;
		customStat.useValue = false;
		std::ranges::transform(customStat.name, customStat.name.begin(), ::tolower);
		// Getting rid of the check for sp->saveParamBits > 0 to display weapon mastery values
		// if a param is supplied it will be used
		int num = -1;
		std::stringstream ss(Trim(stats[i].second));
		if (!(ss >> num).fail() && num > 0) {
			customStat.useValue = true;
			customStat.value = num;
		}
		customStats.push_back(std::move(customStat));
	}
	if (!customStats.empty()) {
		height += (customStats.size() * 16) + 8;
	}

	int xPos = Toggles["Stats on Right"].state ?
		*p_D2CLIENT_ScreenSizeX - 10 - GetXSize() : 10;
	SetX(xPos);
	SetYSize(height);
}

void StatsDisplay::SetX(unsigned int newX) {
	if (newX >= 0 && newX <= Hook::GetScreenWidth()) {
		Lock();
		x = newX;
		Unlock();
	}
}

void StatsDisplay::SetY(unsigned int newY) {
	if (newY >= 0 && newY <= Hook::GetScreenHeight()) {
		Lock();
		y = newY;
		Unlock();
	}
}

void StatsDisplay::SetXSize(unsigned int newXSize) {
	if (newXSize >= 0 && newXSize <= (Hook::GetScreenWidth() - GetX())) {
		Lock();
		xSize = newXSize;
		Unlock();
	}
}

void StatsDisplay::SetYSize(unsigned int newYSize) {
	if (newYSize >= 0 && newYSize <= (Hook::GetScreenHeight() - GetY())) {
		Lock();
		ySize = newYSize;
		Unlock();
	}
}

bool StatsDisplay::InRange(unsigned int x, unsigned int y) {
	return IsActive() &&
		x >= GetX() && y >= GetY() &&
		x <= GetX() + GetXSize() && y <= GetY() + GetYSize();
}

void StatsDisplay::Draw() {
	display->Lock();
	display->OnDraw();
	display->Unlock();
}

void StatsDisplay::OnDraw() {
	UnitAny *unit = D2CLIENT_GetPlayerUnit();
	bool isMerc = false;
	if (!unit)
		return;
	int column1 = GetX() + 5;
	int column2 = column1 + GetXSize() / 2;

	if (IsMinimized()) {
		return;
	}

	if (D2CLIENT_GetUIState(UI_MERC)) {
		unit = D2CLIENT_GetMercUnit();
		isMerc = true;
	}
	for(std::list<Hook*>::iterator it = Hooks.begin(); it != Hooks.end(); it++)
		(*it)->OnDraw();

	int y = GetY();
	RECT pRect;
	pRect.left = GetX();
	pRect.top = y;
	pRect.right = x + GetXSize();
	pRect.bottom = y + GetYSize();

	Drawing::Boxhook::Draw(GetX(),GetY(), GetXSize(), GetYSize(), White, Drawing::BTBlack);
	Drawing::Framehook::DrawRectStub(&pRect);

	y += 8;

	// Display name of the Unit.
	std::wstring unitName =
			isMerc
					? (GetColorCode(TextColor::Purple) + L"Mercenary")
					: (GetColorCode(TextColor::White)
							+ FromUtf8(unit->pPlayerData->szName));
	std::wstring nameText =
			std::format(
					L"{}{}:{} {}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_NAME_3),
					GetColorCode(TextColor::White),
					unitName);
	Texthook::Draw(column1, y, None, 6, Gold, nameText.c_str());

	// Display the level stat.
	std::wstring levelDisplayText =
			std::format(
					L"{}{}:{} {}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_STRCHRLVL),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_LEVEL, 0));
	Texthook::Draw(
			pRect.right - 5, y, Right, 6, Gold, levelDisplayText.c_str());

	// Display "+% to experience" stat.
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	std::wstring additionalExperienceText =
			std::format(
					L"{}+XP:{}{}%%",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::DarkGreen),
					(int)D2COMMON_GetUnitStat(unit, STAT_ADDEXPERIENCE, 0));
	Texthook::Draw(
			pRect.right - 5,
			y + 12,
			Right,
			6,
			Gold,
			additionalExperienceText.c_str());

	y += 8;

	// Display resistance stats.
	BnetData* pData = (*p_D2LAUNCH_BnData);
	int xPacMultiplier = pData->nCharFlags & PLAYER_TYPE_EXPANSION ? 2 : 1;
	int resPenalty[3] = { RES_PENALTY_CLS_NORM, RES_PENALTY_CLS_NM, RES_PENALTY_CLS_HELL };
	int penalty = resPenalty[D2CLIENT_GetDifficulty()] * xPacMultiplier;
	int fRes = (int)D2COMMON_GetUnitStat(unit, STAT_FIRERESIST, 0) + penalty;
	int fMax = (int)D2COMMON_GetUnitStat(unit, STAT_MAXFIRERESIST, 0) + 75;
	int cRes = (int)D2COMMON_GetUnitStat(unit, STAT_COLDRESIST, 0) + penalty;
	int cMax = (int)D2COMMON_GetUnitStat(unit, STAT_MAXCOLDRESIST, 0) + 75;
	int lRes = (int)D2COMMON_GetUnitStat(unit, STAT_LIGHTNINGRESIST, 0) + penalty;
	int lMax = (int)D2COMMON_GetUnitStat(unit, STAT_MAXLIGHTNINGRESIST, 0) + 75;
	int pRes = (int)D2COMMON_GetUnitStat(unit, STAT_POISONRESIST, 0) + penalty;
	int pMax = (int)D2COMMON_GetUnitStat(unit, STAT_MAXPOISONRESIST, 0) + 75;
	int pLengthReduce = (int)D2COMMON_GetUnitStat(unit, STAT_POISONLENGTHREDUCTION, 0);

	std::wstring resistancesText =
			std::format(
					L"{}{}{}{}{}/{}{}  {}{}{}/{}{}  {}{}{}/{}{}  {}{}{}/{}{}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_STRSKILL90),

					// Fire resistance
					GetColorCode(TextColor::Red),
					fRes,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Red),
					fMax,

					// Cold resistance
					GetColorCode(TextColor::Blue),
					cRes,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Blue),
					cMax,

					// Lightning resistance
					GetColorCode(TextColor::Yellow),
					lRes,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Yellow),
					lMax,

					// Poison resistance
					GetColorCode(TextColor::Green),
					pRes,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Green),
					pMax);
	Texthook::Draw(column1, (y += 16), None, 6, Gold, resistancesText.c_str());

	// Poison Length: {PsnLngthReduce}%
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	std::wstring poisonLengthText =
			std::format(
					L"{}{}{}{}%%",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_STRSKILL14),
					GetColorCode(TextColor::DarkGreen),
					(100 - penalty - pLengthReduce));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, poisonLengthText.c_str());
	y += 8;

	// Display absorb stats.
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	int fAbsorb = (int)D2COMMON_GetUnitStat(unit, STAT_FIREABSORB, 0);
	int fAbsorbPct = (int)D2COMMON_GetUnitStat(unit, STAT_FIREABSORBPERCENT, 0);
	int cAbsorb = (int)D2COMMON_GetUnitStat(unit, STAT_COLDABSORB, 0);
	int cAbsorbPct = (int)D2COMMON_GetUnitStat(unit, STAT_COLDABSORBPERCENT, 0);
	int lAbsorb = (int)D2COMMON_GetUnitStat(unit, STAT_LIGHTNINGABSORB, 0);
	int lAbsorbPct = (int)D2COMMON_GetUnitStat(unit, STAT_LIGHTNINGABSORBPERCENT, 0);
	int mAbsorb = (int)D2COMMON_GetUnitStat(unit, STAT_MAGICABSORB, 0);
	int mAbsorbPct = (int)D2COMMON_GetUnitStat(unit, STAT_MAGICABSORBPERCENT, 0);
	std::wstring elementalAbsorbText =
			std::format(
					L"{}Absorption: {}{}{}/{}{}%% {}{}{}/{}{}%% {}{}{}/{}{}%% {}{}{}/{}{}%%",
					GetColorCode(TextColor::Gold),

					// Fire absorb
					GetColorCode(TextColor::Red),
					fAbsorb,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Red),
					fAbsorbPct,

					// Cold absorb
					GetColorCode(TextColor::Blue),
					cAbsorb,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Blue),
					cAbsorbPct,

					// Lightning absorb
					GetColorCode(TextColor::Yellow),
					lAbsorb,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Yellow),
					lAbsorbPct,

					// Magic absorb
					GetColorCode(TextColor::Orange),
					mAbsorb,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Orange),
					mAbsorbPct);
	Texthook::Draw(
			column1, (y += 16), None, 6, Red, elementalAbsorbText.c_str());

	// Display damage reduction stats.
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	int dmgReduction = (int)D2COMMON_GetUnitStat(unit, STAT_DMGREDUCTION, 0);
	int dmgReductionPct = (int)D2COMMON_GetUnitStat(unit, STAT_DMGREDUCTIONPCT, 0);
	int magReduction = (int)D2COMMON_GetUnitStat(unit, STAT_MAGICDMGREDUCTION, 0);
	int magReductionPct = (int)D2COMMON_GetUnitStat(unit, STAT_MAGICDMGREDUCTIONPCT, 0);
	std::wstring damageReductionText =
			std::format(
					L"{}Damage Reduction: {}{}{}/{}{}%% {}{}{}/{}{}%%",

					GetColorCode(TextColor::Gold),

					// Damage reduction
					GetColorCode(TextColor::Tan),
					dmgReduction,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Tan),
					dmgReductionPct,

					// Magic damage reduction
					GetColorCode(TextColor::Orange),
					magReduction,
					GetColorCode(TextColor::White),
					GetColorCode(TextColor::Orange),
					magReductionPct);
	Texthook::Draw(
			column1, (y += 16), None, 6, Tan, damageReductionText.c_str());
	y += 8;

	// Display elemental mastery stats.
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	int fMastery = (int)D2COMMON_GetUnitStat(unit, STAT_FIREMASTERY, 0);
	int cMastery = (int)D2COMMON_GetUnitStat(unit, STAT_COLDMASTERY, 0);
	int lMastery = (int)D2COMMON_GetUnitStat(unit, STAT_LIGHTNINGMASTERY, 0);
	int pMastery = (int)D2COMMON_GetUnitStat(unit, STAT_POISONMASTERY, 0);
	int mMastery = (int)D2COMMON_GetUnitStat(unit, STAT_PASSIVEMAGICDMGMASTERY, 0);
	std::wstring elementalMasteryText =
			std::format(
					L"{}Elemental Mastery:{} {}%%{} {}%%{} {}%%{} {}%%{} {}%%",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Red),
					fMastery,
					GetColorCode(TextColor::Blue),
					cMastery,
					GetColorCode(TextColor::Yellow),
					lMastery,
					GetColorCode(TextColor::Green),
					pMastery,
					GetColorCode(TextColor::Orange),
					mMastery);
	Texthook::Draw(
			column1, (y += 16), None, 6, Gold, elementalMasteryText.c_str());

	// Display elemental pierce stats.
	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	int fPierce = (int)D2COMMON_GetUnitStat(unit, STAT_PSENEMYFIRERESREDUC, 0);
	int cPierce = (int)D2COMMON_GetUnitStat(unit, STAT_PSENEMYCOLDRESREDUC, 0);
	int lPierce = (int)D2COMMON_GetUnitStat(unit, STAT_PSENEMYLIGHTNRESREDUC, 0);
	int pPierce = (int)D2COMMON_GetUnitStat(unit, STAT_PSENEMYPSNRESREDUC, 0);
	int mPierce = (int)D2COMMON_GetUnitStat(unit, STAT_PASSIVEMAGICRESREDUC, 0);
	std::wstring elementalPierceText =
			std::format(
					L"{}Elemental Pierce:{} {}%%{} {}%%{} {}%%{} {}%%{} {}%%",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Red),
					fPierce,
					GetColorCode(TextColor::Blue),
					cPierce,
					GetColorCode(TextColor::Yellow),
					lPierce,
					GetColorCode(TextColor::Green),
					pPierce,
					GetColorCode(TextColor::Orange),
					mPierce);
	Texthook::Draw(
			column1, (y += 16), None, 6, Gold, elementalPierceText.c_str());

	// Display base attack rating stats.
	int classNum = pData->nCharClass;
	auto classArMod = CharList[classNum]->toHitFactor - 35;
	int dexAR = (int)D2COMMON_GetUnitStat(unit, STAT_DEXTERITY, 0) * 5 + classArMod;
	int gearAR = (int)D2COMMON_GetUnitStat(unit, STAT_ATTACKRATING, 0);

	std::wstring baseAttackRatingText =
			std::format(
					L"{}Base AR:{} dex:{} {}{} equip:{} {}{} total:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					dexAR,
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					gearAR,
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					dexAR + gearAR);
	Texthook::Draw(
			column1, (y += 16), None, 6, Gold, baseAttackRatingText.c_str());

	// Display base defense stats.
	int gearDef = (int)D2COMMON_GetUnitStat(unit, STAT_DEFENSE, 0);
	int dexDef = (int)D2COMMON_GetUnitStat(unit, STAT_DEXTERITY, 0) / 4;

	std::wstring baseDefenseText =
			std::format(
					L"{}Base Def:{} dex:{} {}{} equip:{} {}{} total:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					dexDef,
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					gearDef,
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					dexDef + gearDef);
	Texthook::Draw(column1, (y += 16), None, 6, Gold, baseDefenseText.c_str());

	// Display base damage stats.
	std::wstring baseDamageText =
			std::format(
					L"{}Base Damage:{} 1h:{} {}-{}{} 2h:{} {}-{}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMDAMAGE, 0),
					(int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMDAMAGE, 0),
					GetColorCode(TextColor::Grey),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_SECONDARYMINIMUMDAMAGE, 0),
					(int)D2COMMON_GetUnitStat(unit, STAT_SECONDARYMAXIMUMDAMAGE, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, baseDamageText.c_str());

	y += 8;

	// Display increase speed/rate stats.

	std::wstring castRateText =
			std::format(
					L"{}Cast Rate:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_FASTERCAST, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, castRateText.c_str());

	std::wstring blockRateText =
			std::format(
					L"{}Block Rate:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_FASTERBLOCK, 0));
	Texthook::Draw(column2, y, None, 6, Gold, blockRateText.c_str());

	std::wstring hitRecoveryText =
			std::format(
					L"{}Hit Recovery:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_FASTERHITRECOVERY, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, hitRecoveryText.c_str());

	std::wstring runWalkText =
			std::format(
					L"{}Run/Walk:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_FASTERRUNWALK, 0));
	Texthook::Draw(column2, y, None, 6, Gold, runWalkText.c_str());

	std::wstring attackRateText =
			std::format(
					L"{}Attack Rate:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_ATTACKRATE, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, attackRateText.c_str());

	// Display increase attack speed stats.
	std::wstring increasedAttackSpeedText =
			std::format(
					L"{}IAS:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_IAS, 0));
	Texthook::Draw(column2, y, None, 6, Gold, increasedAttackSpeedText.c_str());

	y += 8;

	// Display additional melee stats.

	std::wstring crushingBlowText =
			std::format(
					L"{}Crushing Blow:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_CRUSHINGBLOW, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, crushingBlowText.c_str());

	std::wstring openWoundsText =
			std::format(
					L"{}Open Wounds:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_OPENWOUNDS, 0));
	Texthook::Draw(column2, y, None, 6, Gold, openWoundsText.c_str());

	std::wstring deadlyStrikeText =
			std::format(
					L"{}{}:{} {}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_MODSTR5Q),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_DEADLYSTRIKE, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, deadlyStrikeText.c_str());

	std::wstring criticalStrikeText =
			std::format(
					L"{}{}:{} {}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_SKILLAN9),
					GetColorCode(TextColor::White),
					(int)D2COMMON_GetUnitStat(unit, STAT_CRITICALSTRIKE, 0));
	Texthook::Draw(column2, y, None, 6, Gold, criticalStrikeText.c_str());

	std::wstring lifeLeechText =
			std::format(
					L"{}Life Leech:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Red),
					(int)D2COMMON_GetUnitStat(unit, STAT_LIFELEECH, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, lifeLeechText.c_str());

	std::wstring manaLeechText =
			std::format(
					L"{}Mana Leech:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Blue),
					(int)D2COMMON_GetUnitStat(unit, STAT_MANALEECH, 0));
	Texthook::Draw(column2, y, None, 6, Gold, manaLeechText.c_str());

	int totalPierce =
			(int)D2COMMON_GetUnitStat(unit, STAT_PIERCINGATTACK, 0)
					+ (int)D2COMMON_GetUnitStat(unit, STAT_PIERCE, 0);
	std::wstring projectilePierceText =
			std::format(
					L"{}Projectile Pierce:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::White),
					totalPierce);
	Texthook::Draw(
			column1, (y += 16), None, 6, Gold, projectilePierceText.c_str());

	y += 8;

	// Display added melee damage

	int addedPhys = (int)D2COMMON_GetUnitStat(unit, STAT_ADDSDAMAGE, 0);
	std::wstring addedPhysicalDamageText =
			std::format(
					L"{}+{}{} {}",
					GetColorCode(TextColor::Gold),
					D2LANG_GetLocaleText(D2STR_ITEMSTATS1G),
					GetColorCode(TextColor::White),
					addedPhys);
	Texthook::Draw(
			column1, (y += 16), None, 6, Gold, addedPhysicalDamageText.c_str());

	int minMagic = (int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMMAGICALDAMAGE, 0);
	int maxMagic = (int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMMAGICALDAMAGE, 0);
	std::wstring magicDamageText =
			std::format(
					L"{}{}-{}",
					GetColorCode(TextColor::Orange),
					minMagic,
					maxMagic);
	Texthook::Draw(column2, y, None, 6, Orange, magicDamageText.c_str());

	int minFire = (int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMFIREDAMAGE, 0);
	int maxFire = (int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMFIREDAMAGE, 0);
	std::wstring fireDamageText =
			std::format(
					L"{}{}-{}",
					GetColorCode(TextColor::Red),
					minFire,
					maxFire);
	Texthook::Draw(column1, (y += 16), None, 6, Red, fireDamageText.c_str());

	int minCold = (int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMCOLDDAMAGE, 0);
	int maxCold = (int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMCOLDDAMAGE, 0);
	std::wstring coldDamageText =
			std::format(
					L"{}{}-{}",
					GetColorCode(TextColor::Blue),
					minCold,
					maxCold);
	Texthook::Draw(column2, y, None, 6, Blue, coldDamageText.c_str());

	int minLight = (int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMLIGHTNINGDAMAGE, 0);
	int maxLight = (int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMLIGHTNINGDAMAGE, 0);
	std::wstring lightningDamageText =
			std::format(
					L"{}{}-{}",
					GetColorCode(TextColor::Yellow),
					minLight,
					maxLight);
	Texthook::Draw(
			column1, (y += 16), None, 6, Yellow, lightningDamageText.c_str());

	int minPoison = (int)D2COMMON_GetUnitStat(unit, STAT_MINIMUMPOISONDAMAGE, 0);
	int maxPoison = (int)D2COMMON_GetUnitStat(unit, STAT_MAXIMUMPOISONDAMAGE, 0);
	int poisonLength = (int)D2COMMON_GetUnitStat(unit, STAT_POISONDAMAGELENGTH, 0);
	int poisonLengthOverride = (int)D2COMMON_GetUnitStat(unit, STAT_SKILLPOISONOVERRIDELEN, 0);
	if (poisonLengthOverride > 0) {
		poisonLength = poisonLengthOverride;
	}
	std::wstring poisonDamageText =
			std::format(
					L"{}{:.0f}-{:.0f} over {:.1f}s",
					GetColorCode(TextColor::Green),
					minPoison / 256.0 * poisonLength,
					maxPoison / 256.0 * poisonLength,
					poisonLength / 25.0);
	Texthook::Draw(column2, y, None, 6, Green, poisonDamageText.c_str());

	y += 8;

	// Display loot stats.

	// TODO (Mir Drualga): Replace %% with % when Texthook removes its printf
	std::wstring magicFindText =
			std::format(
					L"{}Magic Find:{} {}%%",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Blue),
					(int)D2COMMON_GetUnitStat(unit, STAT_MAGICFIND, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, magicFindText.c_str());

	std::wstring goldFindText =
			std::format(
					L"{}Gold Find:{} {}%%",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Yellow),
					(int)D2COMMON_GetUnitStat(unit, STAT_GOLDFIND, 0));
	Texthook::Draw(column2, y, None, 6, Gold, goldFindText.c_str());

	std::wstring stashGoldText =
			std::format(
					L"{}Stash Gold:{} {}",
					GetColorCode(TextColor::Gold),
					GetColorCode(TextColor::Yellow),
					(int)D2COMMON_GetUnitStat(unit, STAT_GOLDBANK, 0));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, stashGoldText.c_str());

	// Display quest stats.

	void* quests = D2CLIENT_GetQuestInfo();

	bool isCowKingDead =
			D2COMMON_GetQuestFlag(quests, THE_SEARCH_FOR_CAIN, QFLAG_CUSTOM_6);
	std::wstring cowKingText =
			std::format(
					L"{}Cow King Alive:{}{}",
					GetColorCode(TextColor::Gold),
					GetColorCode(isCowKingDead ? TextColor::Red : TextColor::Green),
					D2LANG_GetLocaleText(isCowKingDead ? D2STR_NO : D2STR_YES));
	Texthook::Draw(column2, y, None, 6, Gold, cowKingText.c_str());

	QuestBugState andyBugState = GetAndarielBuggedState(quests);
	std::wstring andyBuggedText =
			std::format(
					L"{}Andy Bugged:{}",
					GetColorCode(TextColor::Gold),
					ToString(andyBugState));
	Texthook::Draw(column1, (y += 16), None, 6, Gold, andyBuggedText.c_str());

	QuestBugState duryBugState = GetDurielBuggedState(quests);
	std::wstring duryBuggedText =
			std::format(
					L"{}Dury Bugged:{}",
					GetColorCode(TextColor::Gold),
					ToString(duryBugState));
	Texthook::Draw(column2, y, None, 6, Gold, duryBuggedText.c_str());
	y += 8;

	// Display custom stats.
	for (const auto& customStat : customStats) {
		std::optional<int> secondary =
				customStat.useValue
						? std::make_optional(customStat.value)
						: std::nullopt;
		int stat =
				(int)D2COMMON_GetUnitStat(
						unit,
						STAT_NUMBER(customStat.name),
						secondary.value_or(0));
		std::wstring customStatText;
		if (secondary.has_value()) {
			customStatText =
					std::format(
							L"{}{}[{}]:{} {}",
							GetColorCode(TextColor::Gold),
							FromUtf8(customStat.name),
							*secondary,
							GetColorCode(TextColor::White),
							stat);
		} else {
			customStatText =
					std::format(
							L"{}{}:{} {}",
							GetColorCode(TextColor::Gold),
							FromUtf8(customStat.name),
							GetColorCode(TextColor::White),
							stat);
		}
		Texthook::Draw(column1, (y += 16), None, 6, Gold, customStatText.c_str());
	}
}

bool StatsDisplay::KeyClick(bool bUp, BYTE bKey, LPARAM lParam) {
	display->Lock();
	bool block = display->OnKey(bUp, bKey, lParam);
	display->Unlock();
	return block;
}

bool StatsDisplay::OnKey(bool up, BYTE kkey, LPARAM lParam) {
	UnitAny *unit = D2CLIENT_GetPlayerUnit();
	if (!unit)
		return false;

	if (IsMinimized()) {
		if (!up && kkey == statsKey) {
			LoadConfig();
			SetMinimized(false);
			return true;
		}
	} else {
		if (!up && (kkey == statsKey || kkey == VK_ESCAPE)) {
			SetMinimized(true);
			return true;
		}
	}
	return false;
}

bool StatsDisplay::Click(bool up, unsigned int mouseX, unsigned int mouseY) {
	display->Lock();
	bool block = display->OnClick(up, mouseX, mouseY);
	display->Unlock();
	return block;
}

bool StatsDisplay::OnClick(bool up, unsigned int x, unsigned int y) {
	UnitAny *unit = D2CLIENT_GetPlayerUnit();
	if (!unit)
		return false;

	if (!IsMinimized() && InRange(x, y)) {
		SetMinimized(true);
		return true;
	}
	return false;
}

}  // namespace Drawing
