
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_H_

#include <windows.h>

#include <string>
#include <vector>

#include "ItemInfo.h"
#include "UnitItemInfo.h"
#include "ConditionType.h"
#include "Rule.h"

namespace bh::modules::item {

extern BYTE LastConditionType;

class Condition {
 public:
  Condition() = default;
  virtual ~Condition() = default;

  static const std::string tokenDelims;
  static void BuildConditions(std::vector<Condition*> &conditions, std::string token);
  static void ProcessConditions(std::vector<Condition*> &rawConditions, std::vector<Condition*> &processedConditions);
  static void AddOperand(std::vector<Condition*> &conditions, Condition *cond);
  static void AddNonOperand(std::vector<Condition*> &conditions, Condition *cond);

  bool Evaluate(UnitItemInfo *uInfo, ItemInfo *info, Condition *arg1, Condition *arg2);

  BYTE conditionType;

 private:
  virtual bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) { return false; }
  virtual bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) { return false; }
};

class TrueCondition : public Condition
{
public:
	TrueCondition() { conditionType = CT_Operand; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class FalseCondition : public Condition
{
public:
	FalseCondition() { conditionType = CT_Operand; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class NegationOperator : public Condition
{
public:
	NegationOperator() { conditionType = CT_NegationOperator; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class LeftParen : public Condition
{
public:
	LeftParen() { conditionType = CT_LeftParen; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class RightParen : public Condition
{
public:
	RightParen() { conditionType = CT_RightParen; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class AndOperator : public Condition
{
public:
	AndOperator() { conditionType = CT_BinaryOperator; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class OrOperator : public Condition
{
public:
	OrOperator() { conditionType = CT_BinaryOperator; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ItemCodeCondition : public Condition
{
public:
	ItemCodeCondition(const char *code) {
		targetCode[0] = code[0];
		targetCode[1] = code[1];
		targetCode[2] = code[2];
		targetCode[3] = 0;
		conditionType = CT_Operand;
	};
private:
	char targetCode[4];
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class FlagsCondition : public Condition
{
public:
	FlagsCondition(unsigned int flg) : flag(flg) { conditionType = CT_Operand; };
private:
	unsigned int flag;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class PlayerTypeCondition : public Condition
{
public:
	PlayerTypeCondition(unsigned int m) : mode(m) { conditionType = CT_Operand; };
private:
	unsigned int mode;
	bool EvaluateInternal(UnitItemInfo* uInfo, Condition* arg1, Condition* arg2);
	bool EvaluateInternalFromPacket(ItemInfo* info, Condition* arg1, Condition* arg2);
};

class QualityCondition : public Condition
{
public:
	QualityCondition(unsigned int qual) : quality(qual) { conditionType = CT_Operand; };
private:
	unsigned int quality;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class NonMagicalCondition : public Condition
{
public:
	NonMagicalCondition() { conditionType = CT_Operand; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class QualityIdCondition : public Condition
{
public:
	QualityIdCondition(unsigned int quality, unsigned int id) : quality(quality), id(id){ conditionType = CT_Operand; };
private:
	unsigned int quality;
	unsigned int id;
	bool EvaluateInternal(UnitItemInfo* uInfo, Condition* arg1, Condition* arg2);
	bool EvaluateInternalFromPacket(ItemInfo* info, Condition* arg1, Condition* arg2);
};

class GemLevelCondition : public Condition
{
public:
	GemLevelCondition(BYTE op, BYTE gem) : gemLevel(gem), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE gemLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class GemTypeCondition : public Condition
{
public:
	GemTypeCondition(BYTE op, BYTE gType) : gemType(gType), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE gemType;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class RuneCondition : public Condition
{
public:
	RuneCondition(BYTE op, BYTE rune) : runeNumber(rune), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE runeNumber;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class GoldCondition : public Condition
{
public:
	GoldCondition(BYTE op, unsigned int amt) : goldAmount(amt), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int goldAmount;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ItemLevelCondition : public Condition {
 public:
  ItemLevelCondition(BYTE op, BYTE ilvl);
 private:
  BYTE operation;
  BYTE itemLevel;
  bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
  bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};
class QualityLevelCondition : public Condition
{
public:
	QualityLevelCondition(BYTE op, BYTE qlvl) : qualityLevel(qlvl), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE qualityLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class AffixLevelCondition : public Condition
{
public:
	AffixLevelCondition(BYTE op, BYTE alvl) : affixLevel(alvl), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE affixLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class CraftAffixLevelCondition : public Condition
{
public:
	CraftAffixLevelCondition(BYTE op, BYTE alvl) : affixLevel(alvl), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE affixLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class RequiredLevelCondition : public Condition
{
public:
	RequiredLevelCondition(BYTE op, BYTE rlvl) : requiredLevel(rlvl), operation(op) { conditionType = CT_Operand; };
private:
	BYTE operation;
	BYTE requiredLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ItemGroupCondition : public Condition
{
public:
	ItemGroupCondition(unsigned int group) : itemGroup(group) { conditionType = CT_Operand; };
private:
	unsigned int itemGroup;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class EDCondition : public Condition
{
public:
	EDCondition(BYTE op, unsigned int target) : operation(op), targetED(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int targetED;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
	bool EvaluateED(unsigned int flags);
};

class DurabilityCondition : public Condition
{
public:
	DurabilityCondition(BYTE op, unsigned int target) : operation(op), targetDurability(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int targetDurability;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ChargedCondition : public Condition
{
public:
	ChargedCondition(BYTE op, unsigned int sk, unsigned int target) : operation(op), skill(sk), targetLevel(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int skill;
	unsigned int targetLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class FoolsCondition : public Condition
{
public:
	FoolsCondition() { conditionType = CT_Operand; };
private:
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class SkillListCondition : public Condition
{
public:
	SkillListCondition(BYTE op, unsigned int t, unsigned int target) : operation(op), type(t), targetStat(target) {
		conditionType = CT_Operand;
		Init();
	};
private:
	BYTE operation;
	unsigned int type;
	unsigned int targetStat;
	std::map<std::string, std::string> classSkillList;
	std::map<std::string, std::string> skillList;
	std::vector<unsigned int> goodClassSkills;
	std::vector<unsigned int> goodTabSkills;
	void Init();
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class CharStatCondition : public Condition
{
public:
	CharStatCondition(unsigned int stat, unsigned int stat2, BYTE op, unsigned int target)
		: stat1(stat), stat2(stat2), operation(op), targetStat(target) { conditionType = CT_Operand; };
private:
	unsigned int stat1;
	unsigned int stat2;
	BYTE operation;
	unsigned int targetStat;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class DifficultyCondition : public Condition
{
public:
	DifficultyCondition(BYTE op, unsigned int target)
		: operation(op), targetDiff(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int targetDiff;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class FilterLevelCondition : public Condition
{
public:
	FilterLevelCondition(BYTE op, unsigned int target)
		: operation(op), filterLevel(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int filterLevel;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ItemStatCondition : public Condition
{
public:
	ItemStatCondition(unsigned int stat, unsigned int stat2, BYTE op, unsigned int target)
		: itemStat(stat), itemStat2(stat2), operation(op), targetStat(target) { conditionType = CT_Operand; };
private:
	unsigned int itemStat;
	unsigned int itemStat2;
	BYTE operation;
	unsigned int targetStat;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class PartialCondition : public Condition
{
public:
	PartialCondition(BYTE op, int target_count, std::vector<std::string> tokens)
		: operation(op), target_count(target_count) {
		for (auto token : tokens) {
			make_count_subrule(token);	
		}
		conditionType = CT_Operand;
	};
	
	// make_count_subrule calls BuildConditon, which creates new Conditions. We free these here.
	~PartialCondition();
private:
	BYTE operation;
	const int target_count;
	std::vector<Rule> rules; // TODO: should be const, but Rule::Evalate needs to be modified
	void make_count_subrule(std::string token);
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ItemPriceCondition : public Condition
{
public:
	ItemPriceCondition(BYTE op, unsigned int target)
		: operation(op), targetStat(target) {
		conditionType = CT_Operand;
	};
private:
	BYTE operation;
	unsigned int targetStat;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class ResistAllCondition : public Condition
{
public:
	ResistAllCondition(BYTE op, unsigned int target) : operation(op), targetStat(target) { conditionType = CT_Operand; };
private:
	BYTE operation;
	unsigned int targetStat;
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

class AddCondition : public Condition
{
public:
	AddCondition(std::string& k, BYTE op, unsigned int target) : key(k), operation(op), targetStat(target) { 
		conditionType = CT_Operand;
		Init();
	};
private:
	BYTE operation;
	std::vector<std::string> codes;
	std::vector<DWORD> stats;
	unsigned int targetStat;
	std::string key;
	void Init();
	bool EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2);
	bool EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2);
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_H_
