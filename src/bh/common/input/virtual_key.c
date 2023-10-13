/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2023  SlashDiablo Team
 *
 * This file is part of SlashDiablo Maphack.
 *
 * SlashDiablo Maphack is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include "bh/common/input/virtual_key.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/input/internal/virtual_key_symbol_name_table.h"
#include "bh/common/string_util/ascii.h"

#define STRING_VIEW(str_lit) (str_lit), (sizeof(str_lit) / sizeof(str_lit[0]) - 1)

static const char kNone[] = "NONE";

/**
 * This table is only used for initialization purposes.
 */
static struct Input_VirtualKey kVirtualKeyTable[] = {
  { 0x00, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Not Set") },

  { VK_LBUTTON, STRING_VIEW("VK_LBUTTON"), STRING_VIEW("VK_LBUTTON"), STRING_VIEW(L"Left mouse button") },
  { VK_RBUTTON, STRING_VIEW("VK_RBUTTON"), STRING_VIEW("VK_RBUTTON"), STRING_VIEW(L"Right mouse button") },
  { VK_CANCEL, STRING_VIEW("VK_CANCEL"), STRING_VIEW("VK_CANCEL"), STRING_VIEW(L"Control-break processing") },
  { VK_MBUTTON, STRING_VIEW("VK_MBUTTON"), STRING_VIEW("VK_MBUTTON"), STRING_VIEW(L"Middle mouse button (three-button mouse)") },
  { VK_XBUTTON1, STRING_VIEW("VK_XBUTTON1"), STRING_VIEW("VK_XBUTTON1"), STRING_VIEW(L"X1 mouse button") },
  { VK_XBUTTON2, STRING_VIEW("VK_XBUTTON2"), STRING_VIEW("VK_XBUTTON2"), STRING_VIEW(L"X2 mouse button") },

  { 0x07, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },

  { VK_BACK, STRING_VIEW("VK_BACK"), STRING_VIEW("VK_BACK"), STRING_VIEW(L"BACKSPACE key") },
  { VK_TAB, STRING_VIEW("VK_TAB"), STRING_VIEW("VK_TAB"), STRING_VIEW(L"TAB key") },

  { 0x0A, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },
  { 0x0B, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },

  { VK_CLEAR, STRING_VIEW("VK_CLEAR"), STRING_VIEW("VK_CLEAR"), STRING_VIEW(L"CLEAR key") },
  { VK_RETURN, STRING_VIEW("VK_RETURN"), STRING_VIEW("VK_RETURN"), STRING_VIEW(L"ENTER key") },

  { 0x0E, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x0F, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },

  { VK_SHIFT, STRING_VIEW("VK_SHIFT"), STRING_VIEW("VK_SHIFT"), STRING_VIEW(L"SHIFT key") },
  { VK_CONTROL, STRING_VIEW("VK_CONTROL"), STRING_VIEW("VK_CONTROL"), STRING_VIEW(L"CTRL key") },
  { VK_MENU, STRING_VIEW("VK_MENU"), STRING_VIEW("VK_ALT"), STRING_VIEW(L"ALT key") },
  { VK_PAUSE, STRING_VIEW("VK_PAUSE"), STRING_VIEW("VK_PAUSE"), STRING_VIEW(L"PAUSE key") },
  { VK_CAPITAL, STRING_VIEW("VK_CAPITAL"), STRING_VIEW("VK_CAPSLOCK"), STRING_VIEW(L"CAPS LOCK key") },

  { VK_KANA, STRING_VIEW("VK_KANA"), STRING_VIEW("VK_KANA"), STRING_VIEW(L"IME VK_KANA mode") },
  { VK_HANGEUL, STRING_VIEW("VK_HANGEUL"), STRING_VIEW("VK_HANGEUL"), STRING_VIEW(L"IME Hanguel mode") },
  { VK_HANGUL, STRING_VIEW("VK_HANGUL"), STRING_VIEW("VK_HANGUL"), STRING_VIEW(L"IME Hangul mode") },

  { VK_IME_ON, STRING_VIEW("VK_IME_ON"), STRING_VIEW("VK_IME_ON"), STRING_VIEW(L"IME On") },
  { VK_JUNJA, STRING_VIEW("VK_JUNJA"), STRING_VIEW("VK_JUNJA"), STRING_VIEW(L"IME Junja mode") },
  { VK_FINAL, STRING_VIEW("VK_FINAL"), STRING_VIEW("VK_FINAL"), STRING_VIEW(L"IME final mode") },
  { VK_HANJA, STRING_VIEW("VK_HANJA"), STRING_VIEW("VK_HANJA"), STRING_VIEW(L"IME Hanja mode") },
  { VK_KANJI, STRING_VIEW("VK_KANJI"), STRING_VIEW("VK_KANJI"), STRING_VIEW(L"IME Kanji mode") },
  { VK_IME_OFF, STRING_VIEW("VK_IME_OFF"), STRING_VIEW("VK_IME_OFF"), STRING_VIEW(L"IME Off") },

  { VK_ESCAPE, STRING_VIEW("VK_ESCAPE"), STRING_VIEW("VK_ESCAPE"), STRING_VIEW(L"ESC key") },
  { VK_CONVERT, STRING_VIEW("VK_CONVERT"), STRING_VIEW("VK_CONVERT"), STRING_VIEW(L"IME convert") },
  { VK_NONCONVERT, STRING_VIEW("VK_NONCONVERT"), STRING_VIEW("VK_NONCONVERT"), STRING_VIEW(L"IME nonconvert") },
  { VK_ACCEPT, STRING_VIEW("VK_ACCEPT"), STRING_VIEW("VK_ACCEPT"), STRING_VIEW(L"IME accept") },
  { VK_MODECHANGE, STRING_VIEW("VK_MODECHANGE"), STRING_VIEW("VK_MODECHANGE"), STRING_VIEW(L"IME mode change request") },
  { VK_SPACE, STRING_VIEW("VK_SPACE"), STRING_VIEW("VK_SPACE"), STRING_VIEW(L"SPACEBAR") },
  { VK_PRIOR, STRING_VIEW("VK_PRIOR"), STRING_VIEW("VK_PAGEUP"), STRING_VIEW(L"PAGE UP key") },
  { VK_NEXT, STRING_VIEW("VK_NEXT"), STRING_VIEW("VK_PAGEDN"), STRING_VIEW(L"PAGE DOWN key") },
  { VK_END, STRING_VIEW("VK_END"), STRING_VIEW("VK_END"), STRING_VIEW(L"END key") },
  { VK_HOME, STRING_VIEW("VK_HOME"), STRING_VIEW("VK_HOME"), STRING_VIEW(L"HOME key") },
  { VK_LEFT, STRING_VIEW("VK_LEFT"), STRING_VIEW("VK_LEFT"), STRING_VIEW(L"LEFT ARROW key") },
  { VK_UP, STRING_VIEW("VK_UP"), STRING_VIEW("VK_UP"), STRING_VIEW(L"UP ARROW key") },
  { VK_RIGHT, STRING_VIEW("VK_RIGHT"), STRING_VIEW("VK_RIGHT"), STRING_VIEW(L"RIGHT ARROW key") },
  { VK_DOWN, STRING_VIEW("VK_DOWN"), STRING_VIEW("VK_DOWN"), STRING_VIEW(L"DOWN ARROW key") },
  { VK_SELECT, STRING_VIEW("VK_SELECT"), STRING_VIEW("VK_SELECT"), STRING_VIEW(L"SELECT key") },
  { VK_PRINT, STRING_VIEW("VK_PRINT"), STRING_VIEW("VK_PRINT"), STRING_VIEW(L"PRINT key") },
  { VK_EXECUTE, STRING_VIEW("VK_EXECUTE"), STRING_VIEW("VK_EXECUTE"), STRING_VIEW(L"EXECUTE key") },
  { VK_SNAPSHOT, STRING_VIEW("VK_SNAPSHOT"), STRING_VIEW("VK_SNAPSHOT"), STRING_VIEW(L"PRINT SCREEN key") },
  { VK_INSERT, STRING_VIEW("VK_INSERT"), STRING_VIEW("VK_INSERT"), STRING_VIEW(L"INS key") },
  { VK_DELETE, STRING_VIEW("VK_DELETE"), STRING_VIEW("VK_DELETE"), STRING_VIEW(L"DEL key") },
  { VK_HELP, STRING_VIEW("VK_HELP"), STRING_VIEW("VK_HELP"), STRING_VIEW(L"HELP key") },

  { 0x30, STRING_VIEW("VK_0"), STRING_VIEW("VK_0"), STRING_VIEW(L"0 key") },
  { 0x31, STRING_VIEW("VK_1"), STRING_VIEW("VK_1"), STRING_VIEW(L"1 key") },
  { 0x32, STRING_VIEW("VK_2"), STRING_VIEW("VK_2"), STRING_VIEW(L"2 key") },
  { 0x33, STRING_VIEW("VK_3"), STRING_VIEW("VK_3"), STRING_VIEW(L"3 key") },
  { 0x34, STRING_VIEW("VK_4"), STRING_VIEW("VK_4"), STRING_VIEW(L"4 key") },
  { 0x35, STRING_VIEW("VK_5"), STRING_VIEW("VK_5"), STRING_VIEW(L"5 key") },
  { 0x36, STRING_VIEW("VK_6"), STRING_VIEW("VK_6"), STRING_VIEW(L"6 key") },
  { 0x37, STRING_VIEW("VK_7"), STRING_VIEW("VK_7"), STRING_VIEW(L"7 key") },
  { 0x38, STRING_VIEW("VK_8"), STRING_VIEW("VK_8"), STRING_VIEW(L"8 key") },
  { 0x39, STRING_VIEW("VK_9"), STRING_VIEW("VK_9"), STRING_VIEW(L"9 key") },

  { 0x3A, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x3B, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x3C, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x3D, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x3E, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x3F, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },
  { 0x40, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Undefined") },

  { 0x41, STRING_VIEW("VK_A"), STRING_VIEW("VK_A"), STRING_VIEW(L"A key") },
  { 0x42, STRING_VIEW("VK_B"), STRING_VIEW("VK_B"), STRING_VIEW(L"B key") },
  { 0x43, STRING_VIEW("VK_C"), STRING_VIEW("VK_C"), STRING_VIEW(L"C key") },
  { 0x44, STRING_VIEW("VK_D"), STRING_VIEW("VK_D"), STRING_VIEW(L"D key") },
  { 0x45, STRING_VIEW("VK_E"), STRING_VIEW("VK_E"), STRING_VIEW(L"E key") },
  { 0x46, STRING_VIEW("VK_F"), STRING_VIEW("VK_F"), STRING_VIEW(L"F key") },
  { 0x47, STRING_VIEW("VK_G"), STRING_VIEW("VK_G"), STRING_VIEW(L"G key") },
  { 0x48, STRING_VIEW("VK_H"), STRING_VIEW("VK_H"), STRING_VIEW(L"H key") },
  { 0x49, STRING_VIEW("VK_I"), STRING_VIEW("VK_I"), STRING_VIEW(L"I key") },
  { 0x4A, STRING_VIEW("VK_J"), STRING_VIEW("VK_J"), STRING_VIEW(L"J key") },
  { 0x4B, STRING_VIEW("VK_K"), STRING_VIEW("VK_K"), STRING_VIEW(L"K key") },
  { 0x4C, STRING_VIEW("VK_L"), STRING_VIEW("VK_L"), STRING_VIEW(L"L key") },
  { 0x4D, STRING_VIEW("VK_M"), STRING_VIEW("VK_M"), STRING_VIEW(L"M key") },
  { 0x4E, STRING_VIEW("VK_N"), STRING_VIEW("VK_N"), STRING_VIEW(L"N key") },
  { 0x4F, STRING_VIEW("VK_O"), STRING_VIEW("VK_O"), STRING_VIEW(L"O key") },
  { 0x50, STRING_VIEW("VK_P"), STRING_VIEW("VK_P"), STRING_VIEW(L"P key") },
  { 0x51, STRING_VIEW("VK_Q"), STRING_VIEW("VK_Q"), STRING_VIEW(L"Q key") },
  { 0x52, STRING_VIEW("VK_R"), STRING_VIEW("VK_R"), STRING_VIEW(L"R key") },
  { 0x53, STRING_VIEW("VK_S"), STRING_VIEW("VK_S"), STRING_VIEW(L"S key") },
  { 0x54, STRING_VIEW("VK_T"), STRING_VIEW("VK_T"), STRING_VIEW(L"T key") },
  { 0x55, STRING_VIEW("VK_U"), STRING_VIEW("VK_U"), STRING_VIEW(L"U key") },
  { 0x56, STRING_VIEW("VK_V"), STRING_VIEW("VK_V"), STRING_VIEW(L"V key") },
  { 0x57, STRING_VIEW("VK_W"), STRING_VIEW("VK_W"), STRING_VIEW(L"W key") },
  { 0x58, STRING_VIEW("VK_X"), STRING_VIEW("VK_X"), STRING_VIEW(L"X key") },
  { 0x59, STRING_VIEW("VK_Y"), STRING_VIEW("VK_Y"), STRING_VIEW(L"Y key") },
  { 0x5A, STRING_VIEW("VK_Z"), STRING_VIEW("VK_Z"), STRING_VIEW(L"Z key") },

  { VK_LWIN, STRING_VIEW("VK_LWIN"), STRING_VIEW("VK_LWIN"), STRING_VIEW(L"Left Windows key (Natural keyboard)") },
  { VK_RWIN, STRING_VIEW("VK_RWIN"), STRING_VIEW("VK_RWIN"), STRING_VIEW(L"Right Windows key (Natural keyboard)") },
  { VK_APPS, STRING_VIEW("VK_APPS"), STRING_VIEW("VK_APPS"), STRING_VIEW(L"Applications key (Natural keyboard)") },

  { 0x5E, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },

  { VK_SLEEP, STRING_VIEW("VK_SLEEP"), STRING_VIEW("VK_SLEEP"), STRING_VIEW(L"Computer Sleep key") },

  { VK_NUMPAD0, STRING_VIEW("VK_NUMPAD0"), STRING_VIEW("VK_NUMPAD0"), STRING_VIEW(L"Numeric keypad 0 key") },
  { VK_NUMPAD1, STRING_VIEW("VK_NUMPAD1"), STRING_VIEW("VK_NUMPAD1"), STRING_VIEW(L"Numeric keypad 1 key") },
  { VK_NUMPAD2, STRING_VIEW("VK_NUMPAD2"), STRING_VIEW("VK_NUMPAD2"), STRING_VIEW(L"Numeric keypad 2 key") },
  { VK_NUMPAD3, STRING_VIEW("VK_NUMPAD3"), STRING_VIEW("VK_NUMPAD3"), STRING_VIEW(L"Numeric keypad 3 key") },
  { VK_NUMPAD4, STRING_VIEW("VK_NUMPAD4"), STRING_VIEW("VK_NUMPAD4"), STRING_VIEW(L"Numeric keypad 4 key") },
  { VK_NUMPAD5, STRING_VIEW("VK_NUMPAD5"), STRING_VIEW("VK_NUMPAD5"), STRING_VIEW(L"Numeric keypad 5 key") },
  { VK_NUMPAD6, STRING_VIEW("VK_NUMPAD6"), STRING_VIEW("VK_NUMPAD6"), STRING_VIEW(L"Numeric keypad 6 key") },
  { VK_NUMPAD7, STRING_VIEW("VK_NUMPAD7"), STRING_VIEW("VK_NUMPAD7"), STRING_VIEW(L"Numeric keypad 7 key") },
  { VK_NUMPAD8, STRING_VIEW("VK_NUMPAD8"), STRING_VIEW("VK_NUMPAD8"), STRING_VIEW(L"Numeric keypad 8 key") },
  { VK_NUMPAD9, STRING_VIEW("VK_NUMPAD9"), STRING_VIEW("VK_NUMPAD9"), STRING_VIEW(L"Numeric keypad 9 key") },

  { VK_MULTIPLY, STRING_VIEW("VK_MULTIPLY"), STRING_VIEW("VK_NUMPADMULTIPLY"), STRING_VIEW(L"Multiply key") },
  { VK_ADD, STRING_VIEW("VK_ADD"), STRING_VIEW("VK_NUMPADADD"), STRING_VIEW(L"Add key") },
  { VK_SEPARATOR, STRING_VIEW("VK_SEPARATOR"), STRING_VIEW("VK_NUMPADSEPARATOR"), STRING_VIEW(L"Separator key") },
  { VK_SUBTRACT, STRING_VIEW("VK_SUBTRACT"), STRING_VIEW("VK_NUMPADSUBTRACT"), STRING_VIEW(L"Subtract key") },
  { VK_DECIMAL, STRING_VIEW("VK_DECIMAL"), STRING_VIEW("VK_NUMPADDECIMAL"), STRING_VIEW(L"Decimal key") },
  { VK_DIVIDE, STRING_VIEW("VK_DIVIDE"), STRING_VIEW("VK_NUMPADDIVIDE"), STRING_VIEW(L"Divide key") },

  { VK_F1, STRING_VIEW("VK_F1"), STRING_VIEW("VK_F1"), STRING_VIEW(L"F1 key") },
  { VK_F2, STRING_VIEW("VK_F2"), STRING_VIEW("VK_F2"), STRING_VIEW(L"F2 key") },
  { VK_F3, STRING_VIEW("VK_F3"), STRING_VIEW("VK_F3"), STRING_VIEW(L"F3 key") },
  { VK_F4, STRING_VIEW("VK_F4"), STRING_VIEW("VK_F4"), STRING_VIEW(L"F4 key") },
  { VK_F5, STRING_VIEW("VK_F5"), STRING_VIEW("VK_F5"), STRING_VIEW(L"F5 key") },
  { VK_F6, STRING_VIEW("VK_F6"), STRING_VIEW("VK_F6"), STRING_VIEW(L"F6 key") },
  { VK_F7, STRING_VIEW("VK_F7"), STRING_VIEW("VK_F7"), STRING_VIEW(L"F7 key") },
  { VK_F8, STRING_VIEW("VK_F8"), STRING_VIEW("VK_F8"), STRING_VIEW(L"F8 key") },
  { VK_F9, STRING_VIEW("VK_F9"), STRING_VIEW("VK_F9"), STRING_VIEW(L"F9 key") },
  { VK_F10, STRING_VIEW("VK_F10"), STRING_VIEW("VK_F10"), STRING_VIEW(L"F10 key") },
  { VK_F11, STRING_VIEW("VK_F11"), STRING_VIEW("VK_F11"), STRING_VIEW(L"F11 key") },
  { VK_F12, STRING_VIEW("VK_F12"), STRING_VIEW("VK_F12"), STRING_VIEW(L"F12 key") },
  { VK_F13, STRING_VIEW("VK_F13"), STRING_VIEW("VK_F13"), STRING_VIEW(L"F13 key") },
  { VK_F14, STRING_VIEW("VK_F14"), STRING_VIEW("VK_F14"), STRING_VIEW(L"F14 key") },
  { VK_F15, STRING_VIEW("VK_F15"), STRING_VIEW("VK_F15"), STRING_VIEW(L"F15 key") },
  { VK_F16, STRING_VIEW("VK_F16"), STRING_VIEW("VK_F16"), STRING_VIEW(L"F16 key") },
  { VK_F17, STRING_VIEW("VK_F17"), STRING_VIEW("VK_F17"), STRING_VIEW(L"F17 key") },
  { VK_F18, STRING_VIEW("VK_F18"), STRING_VIEW("VK_F18"), STRING_VIEW(L"F18 key") },
  { VK_F19, STRING_VIEW("VK_F19"), STRING_VIEW("VK_F19"), STRING_VIEW(L"F19 key") },
  { VK_F20, STRING_VIEW("VK_F20"), STRING_VIEW("VK_F20"), STRING_VIEW(L"F20 key") },
  { VK_F21, STRING_VIEW("VK_F21"), STRING_VIEW("VK_F21"), STRING_VIEW(L"F21 key") },
  { VK_F22, STRING_VIEW("VK_F22"), STRING_VIEW("VK_F22"), STRING_VIEW(L"F22 key") },
  { VK_F23, STRING_VIEW("VK_F23"), STRING_VIEW("VK_F23"), STRING_VIEW(L"F23 key") },
  { VK_F24, STRING_VIEW("VK_F24"), STRING_VIEW("VK_F24"), STRING_VIEW(L"F24 key") },

  { VK_NAVIGATION_VIEW, STRING_VIEW("VK_NAVIGATION_VIEW"), STRING_VIEW("VK_NAVIGATION_VIEW"), STRING_VIEW(L"Navigation view key") },
  { VK_NAVIGATION_MENU, STRING_VIEW("VK_NAVIGATION_MENU"), STRING_VIEW("VK_NAVIGATION_MENU"), STRING_VIEW(L"Navigation menu key") },
  { VK_NAVIGATION_UP, STRING_VIEW("VK_NAVIGATION_UP"), STRING_VIEW("VK_NAVIGATION_UP"), STRING_VIEW(L"Navigation up key") },
  { VK_NAVIGATION_DOWN, STRING_VIEW("VK_NAVIGATION_DOWN"), STRING_VIEW("VK_NAVIGATION_DOWN"), STRING_VIEW(L"Navigation down key") },
  { VK_NAVIGATION_LEFT, STRING_VIEW("VK_NAVIGATION_LEFT"), STRING_VIEW("VK_NAVIGATION_LEFT"), STRING_VIEW(L"Navigation left key") },
  { VK_NAVIGATION_RIGHT, STRING_VIEW("VK_NAVIGATION_RIGHT"), STRING_VIEW("VK_NAVIGATION_RIGHT"), STRING_VIEW(L"Navigation right key") },
  { VK_NAVIGATION_ACCEPT, STRING_VIEW("VK_NAVIGATION_ACCEPT"), STRING_VIEW("VK_NAVIGATION_ACCEPT"), STRING_VIEW(L"Navigation accept key") },
  { VK_NAVIGATION_CANCEL, STRING_VIEW("VK_NAVIGATION_CANCEL"), STRING_VIEW("VK_NAVIGATION_CANCEL"), STRING_VIEW(L"Navigation cancel key") },

  { VK_NUMLOCK, STRING_VIEW("VK_NUMLOCK"), STRING_VIEW("VK_NUMLOCK"), STRING_VIEW(L"NUM LOCK key") },
  { VK_SCROLL, STRING_VIEW("VK_SCROLL"), STRING_VIEW("VK_SCROLL"), STRING_VIEW(L"SCROLL LOCK key") },

  { 0x92, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"OEM specific") },
  { 0x93, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"OEM specific") },
  { 0x94, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"OEM specific") },
  { 0x95, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"OEM specific") },
  { 0x96, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"OEM specific") },

  { 0x97, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x98, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x99, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9A, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9B, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9C, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9D, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9E, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },
  { 0x9F, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },

  { VK_LSHIFT, STRING_VIEW("VK_LSHIFT"), STRING_VIEW("VK_LSHIFT"), STRING_VIEW(L"Left SHIFT key") },
  { VK_RSHIFT, STRING_VIEW("VK_RSHIFT"), STRING_VIEW("VK_RSHIFT"), STRING_VIEW(L"Right SHIFT key") },
  { VK_LCONTROL, STRING_VIEW("VK_LCONTROL"), STRING_VIEW("VK_LCTRL"), STRING_VIEW(L"Left CONTROL key") },
  { VK_RCONTROL, STRING_VIEW("VK_RCONTROL"), STRING_VIEW("VK_RCTRL"), STRING_VIEW(L"Right CONTROL key") },
  { VK_LMENU, STRING_VIEW("VK_LMENU"), STRING_VIEW("VK_LMENU"), STRING_VIEW(L"Left ALT key") },
  { VK_RMENU, STRING_VIEW("VK_RMENU"), STRING_VIEW("VK_RMENU"), STRING_VIEW(L"Right ALT key") },

  { VK_BROWSER_BACK, STRING_VIEW("VK_BROWSER_BACK"), STRING_VIEW("VK_BROWSER_BACK"), STRING_VIEW(L"Browser Back key") },
  { VK_BROWSER_FORWARD, STRING_VIEW("VK_BROWSER_FORWARD"), STRING_VIEW("VK_BROWSER_FORWARD"), STRING_VIEW(L"Browser Forward key") },
  { VK_BROWSER_REFRESH, STRING_VIEW("VK_BROWSER_REFRESH"), STRING_VIEW("VK_BROWSER_REFRESH"), STRING_VIEW(L"Browser Refresh key") },
  { VK_BROWSER_STOP, STRING_VIEW("VK_BROWSER_STOP"), STRING_VIEW("VK_BROWSER_STOP"), STRING_VIEW(L"Browser Stop key") },
  { VK_BROWSER_SEARCH, STRING_VIEW("VK_BROWSER_SEARCH"), STRING_VIEW("VK_BROWSER_SEARCH"), STRING_VIEW(L"Browser Search key") },
  { VK_BROWSER_FAVORITES, STRING_VIEW("VK_BROWSER_FAVORITES"), STRING_VIEW("VK_BROWSER_FAVORITES"), STRING_VIEW(L"Browser Favorites key") },
  { VK_BROWSER_HOME, STRING_VIEW("VK_BROWSER_HOME"), STRING_VIEW("VK_BROWSER_HOME"), STRING_VIEW(L"Browser Start and Home key") },

  { VK_VOLUME_MUTE, STRING_VIEW("VK_VOLUME_MUTE"), STRING_VIEW("VK_VOLUME_MUTE"), STRING_VIEW(L"Volume Mute key") },
  { VK_VOLUME_DOWN, STRING_VIEW("VK_VOLUME_DOWN"), STRING_VIEW("VK_VOLUME_DOWN"), STRING_VIEW(L"Volume Down key") },
  { VK_VOLUME_UP, STRING_VIEW("VK_VOLUME_UP"), STRING_VIEW("VK_VOLUME_UP"), STRING_VIEW(L"Volume Up key") },
  { VK_MEDIA_NEXT_TRACK, STRING_VIEW("VK_MEDIA_NEXT_TRACK"), STRING_VIEW("VK_MEDIA_NEXT_TRACK"), STRING_VIEW(L"Next Track key") },
  { VK_MEDIA_PREV_TRACK, STRING_VIEW("VK_MEDIA_PREV_TRACK"), STRING_VIEW("VK_MEDIA_PREV_TRACK"), STRING_VIEW(L"Previous Track key") },
  { VK_MEDIA_STOP, STRING_VIEW("VK_MEDIA_STOP"), STRING_VIEW("VK_MEDIA_STOP"), STRING_VIEW(L"Stop Media key") },
  { VK_MEDIA_PLAY_PAUSE, STRING_VIEW("VK_MEDIA_PLAY_PAUSE"), STRING_VIEW("VK_MEDIA_PLAY_PAUSE"), STRING_VIEW(L"Play/Pause Media key") },
  { VK_LAUNCH_MAIL, STRING_VIEW("VK_LAUNCH_MAIL"), STRING_VIEW("VK_LAUNCH_MAIL"), STRING_VIEW(L"Start Mail key") },
  { VK_LAUNCH_MEDIA_SELECT, STRING_VIEW("VK_LAUNCH_MEDIA_SELECT"), STRING_VIEW("VK_LAUNCH_MEDIA_SELECT"), STRING_VIEW(L"Select Media key") },
  { VK_LAUNCH_APP1, STRING_VIEW("VK_LAUNCH_APP1"), STRING_VIEW("VK_LAUNCH_APP1"), STRING_VIEW(L"Start Application 1 key") },
  { VK_LAUNCH_APP2, STRING_VIEW("VK_LAUNCH_APP2"), STRING_VIEW("VK_LAUNCH_APP2"), STRING_VIEW(L"Start Application 2 key") },

  { 0xB8, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },
  { 0xB9, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },

  { VK_OEM_1, STRING_VIEW("VK_OEM_1"), STRING_VIEW("VK_SEMICOLON"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key") },
  { VK_OEM_PLUS, STRING_VIEW("VK_OEM_PLUS"), STRING_VIEW("VK_PLUS"), STRING_VIEW(L"For any country/region, the '+' key") },
  { VK_OEM_COMMA, STRING_VIEW("VK_OEM_COMMA"), STRING_VIEW("VK_COMMA"), STRING_VIEW(L"For any country/region, the ',' key") },
  { VK_OEM_MINUS, STRING_VIEW("VK_OEM_MINUS"), STRING_VIEW("VK_MINUS"), STRING_VIEW(L"For any country/region, the '-' key") },
  { VK_OEM_PERIOD, STRING_VIEW("VK_OEM_PERIOD"), STRING_VIEW("VK_PERIOD"), STRING_VIEW(L"For any country/region, the '.' key") },
  { VK_OEM_2, STRING_VIEW("VK_OEM_2"), STRING_VIEW("VK_FORWARDSLASH"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key") },
  { VK_OEM_3, STRING_VIEW("VK_OEM_3"), STRING_VIEW("VK_TILDE"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key") },

  { 0xC1, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },
  { 0xC2, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },

  { VK_GAMEPAD_A, STRING_VIEW("VK_GAMEPAD_A"), STRING_VIEW("VK_GAMEPAD_A"), STRING_VIEW(L"Gamepad A button") },
  { VK_GAMEPAD_B, STRING_VIEW("VK_GAMEPAD_B"), STRING_VIEW("VK_GAMEPAD_B"), STRING_VIEW(L"Gamepad B button") },
  { VK_GAMEPAD_X, STRING_VIEW("VK_GAMEPAD_X"), STRING_VIEW("VK_GAMEPAD_X"), STRING_VIEW(L"Gamepad C button") },
  { VK_GAMEPAD_Y, STRING_VIEW("VK_GAMEPAD_Y"), STRING_VIEW("VK_GAMEPAD_Y"), STRING_VIEW(L"Gamepad Y button") },
  { VK_GAMEPAD_RIGHT_SHOULDER, STRING_VIEW("VK_GAMEPAD_RIGHT_SHOULDER"), STRING_VIEW("VK_GAMEPAD_RIGHT_SHOULDER"), STRING_VIEW(L"Gamepad right shoulder press") },
  { VK_GAMEPAD_LEFT_SHOULDER, STRING_VIEW("VK_GAMEPAD_LEFT_SHOULDER"), STRING_VIEW("VK_GAMEPAD_LEFT_SHOULDER"), STRING_VIEW(L"Gamepad left shoulder press") },
  { VK_GAMEPAD_LEFT_TRIGGER, STRING_VIEW("VK_GAMEPAD_LEFT_TRIGGER"), STRING_VIEW("VK_GAMEPAD_LEFT_TRIGGER"), STRING_VIEW(L"Gamepad left shoulder trigger") },
  { VK_GAMEPAD_RIGHT_TRIGGER, STRING_VIEW("VK_GAMEPAD_RIGHT_TRIGGER"), STRING_VIEW("VK_GAMEPAD_RIGHT_TRIGGER"), STRING_VIEW(L"Gamepad right shoulder trigger") },
  { VK_GAMEPAD_DPAD_UP, STRING_VIEW("VK_GAMEPAD_DPAD_UP"), STRING_VIEW("VK_GAMEPAD_DPAD_UP"), STRING_VIEW(L"Gamepad D-pad up") },
  { VK_GAMEPAD_DPAD_DOWN, STRING_VIEW("VK_GAMEPAD_DPAD_DOWN"), STRING_VIEW("VK_GAMEPAD_DPAD_DOWN"), STRING_VIEW(L"Gamepad D-pad down") },
  { VK_GAMEPAD_DPAD_LEFT, STRING_VIEW("VK_GAMEPAD_DPAD_LEFT"), STRING_VIEW("VK_GAMEPAD_DPAD_LEFT"), STRING_VIEW(L"Gamepad D-pad left") },
  { VK_GAMEPAD_DPAD_RIGHT, STRING_VIEW("VK_GAMEPAD_DPAD_RIGHT"), STRING_VIEW("VK_GAMEPAD_DPAD_RIGHT"), STRING_VIEW(L"Gamepad D-pad right") },
  { VK_GAMEPAD_MENU, STRING_VIEW("VK_GAMEPAD_MENU"), STRING_VIEW("VK_GAMEPAD_MENU"), STRING_VIEW(L"Gamepad menu button") },
  { VK_GAMEPAD_VIEW, STRING_VIEW("VK_GAMEPAD_VIEW"), STRING_VIEW("VK_GAMEPAD_VIEW"), STRING_VIEW(L"Gamepad view button") },
  { VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON"), STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON"), STRING_VIEW(L"Gamepad left thumbstick button") },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON"), STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON"), STRING_VIEW(L"Gamepad right thumbstick button") },
  { VK_GAMEPAD_LEFT_THUMBSTICK_UP, STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_UP"), STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_UP"), STRING_VIEW(L"Gamepad left thumbstick move up") },
  { VK_GAMEPAD_LEFT_THUMBSTICK_DOWN, STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_DOWN"), STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_DOWN"), STRING_VIEW(L"Gamepad left thumbstick move down") },
  { VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT, STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT"), STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT"), STRING_VIEW(L"Gamepad left thumbstick move right") },
  { VK_GAMEPAD_LEFT_THUMBSTICK_LEFT, STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_LEFT"), STRING_VIEW("VK_GAMEPAD_LEFT_THUMBSTICK_LEFT"), STRING_VIEW(L"Gamepad left thumbstick move left") },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_UP, STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_UP"), STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_UP"), STRING_VIEW(L"Gamepad right thumbstick move up") },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN, STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN"), STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN"), STRING_VIEW(L"Gamepad right thumbstick move down") },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT, STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT"), STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT"), STRING_VIEW(L"Gamepad right thumbstick move right") },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT, STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT"), STRING_VIEW("VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT"), STRING_VIEW(L"Gamepad right thumbstick move left") },

  { VK_OEM_4, STRING_VIEW("VK_OEM_4"), STRING_VIEW("VK_LEFTBRACKET"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key") },
  { VK_OEM_5, STRING_VIEW("VK_OEM_5"), STRING_VIEW("VK_BACKSLASH"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\\|' key") },
  { VK_OEM_6, STRING_VIEW("VK_OEM_6"), STRING_VIEW("VK_RIGHTBRACKET"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key") },
  { VK_OEM_7, STRING_VIEW("VK_OEM_7"), STRING_VIEW("VK_QUOTE"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key") },
  { VK_OEM_8, STRING_VIEW("VK_OEM_8"), STRING_VIEW("VK_OEM_8"), STRING_VIEW(L"Used for miscellaneous characters; it can vary by keyboard.") },

  { 0xE0, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },
  { VK_OEM_AX, STRING_VIEW("VK_OEM_AX"), STRING_VIEW("VK_OEM_AX"), STRING_VIEW(L"'AX' key on Japanese AX kbd") },
  { VK_OEM_102, STRING_VIEW("VK_OEM_102"), STRING_VIEW("VK_OEM_102"), STRING_VIEW(L"The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard") },
  { VK_ICO_HELP, STRING_VIEW("VK_ICO_HELP"), STRING_VIEW("VK_ICO_HELP"), STRING_VIEW(L"Help key on ICO") },
  { VK_ICO_00, STRING_VIEW("VK_ICO_00"), STRING_VIEW("VK_ICO_00"), STRING_VIEW(L"00 key on ICO") },
  { VK_PROCESSKEY, STRING_VIEW("VK_PROCESSKEY"), STRING_VIEW("VK_PROCESSKEY"), STRING_VIEW(L"IME PROCESS key") },
  { VK_ICO_CLEAR, STRING_VIEW("VK_ICO_CLEAR"), STRING_VIEW("VK_ICO_CLEAR"), STRING_VIEW(L"Clear key on ICO") },

  { VK_PACKET, STRING_VIEW("VK_PACKET"), STRING_VIEW("VK_PACKET"), STRING_VIEW(L"Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP") },

  { 0xE8, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Unassigned") },

  { VK_OEM_RESET, STRING_VIEW("VK_OEM_RESET"), STRING_VIEW("VK_OEM_RESET"), STRING_VIEW(L"Nokia/Ericsson Reset key") },
  { VK_OEM_JUMP, STRING_VIEW("VK_OEM_JUMP"), STRING_VIEW("VK_OEM_JUMP"), STRING_VIEW(L"Nokia/Ericsson Jump key") },
  { VK_OEM_PA1, STRING_VIEW("VK_OEM_PA1"), STRING_VIEW("VK_OEM_PA1"), STRING_VIEW(L"Nokia/Ericsson PA1 key") },
  { VK_OEM_PA2, STRING_VIEW("VK_OEM_PA2"), STRING_VIEW("VK_OEM_PA2"), STRING_VIEW(L"Nokia/Ericsson PA2 key") },
  { VK_OEM_PA3, STRING_VIEW("VK_OEM_PA3"), STRING_VIEW("VK_OEM_PA3"), STRING_VIEW(L"Nokia/Ericsson PA3 key") },
  { VK_OEM_WSCTRL, STRING_VIEW("VK_OEM_WSCTRL"), STRING_VIEW("VK_OEM_WSCTRL"), STRING_VIEW(L"Nokia/Ericsson WsCtrl key") },
  { VK_OEM_CUSEL, STRING_VIEW("VK_OEM_CUSEL"), STRING_VIEW("VK_OEM_CUSEL"), STRING_VIEW(L"Nokia/Ericsson CuSel key") },
  { VK_OEM_ATTN, STRING_VIEW("VK_OEM_ATTN"), STRING_VIEW("VK_OEM_ATTN"), STRING_VIEW(L"Nokia/Ericsson Attn key") },
  { VK_OEM_FINISH, STRING_VIEW("VK_OEM_FINISH"), STRING_VIEW("VK_OEM_FINISH"), STRING_VIEW(L"Nokia/Ericsson Finish key") },
  { VK_OEM_COPY, STRING_VIEW("VK_OEM_COPY"), STRING_VIEW("VK_OEM_COPY"), STRING_VIEW(L"Nokia/Ericsson Copy key") },
  { VK_OEM_AUTO, STRING_VIEW("VK_OEM_AUTO"), STRING_VIEW("VK_OEM_AUTO"), STRING_VIEW(L"Nokia/Ericsson Auto key") },
  { VK_OEM_ENLW, STRING_VIEW("VK_OEM_ENLW"), STRING_VIEW("VK_OEM_ENLW"), STRING_VIEW(L"Nokia/Ericsson Enlw key") },
  { VK_OEM_BACKTAB, STRING_VIEW("VK_OEM_BACKTAB"), STRING_VIEW("VK_OEM_BACKTAB"), STRING_VIEW(L"Nokia/Ericsson BackTab key") },

  { VK_ATTN, STRING_VIEW("VK_ATTN"), STRING_VIEW("VK_ATTN"), STRING_VIEW(L"Attn key") },
  { VK_CRSEL, STRING_VIEW("VK_CRSEL"), STRING_VIEW("VK_CRSEL"), STRING_VIEW(L"CrSel key") },
  { VK_EXSEL, STRING_VIEW("VK_EXSEL"), STRING_VIEW("VK_EXSEL"), STRING_VIEW(L"ExSel key") },
  { VK_EREOF, STRING_VIEW("VK_EREOF"), STRING_VIEW("VK_EREOF"), STRING_VIEW(L"Erase EOF key") },
  { VK_PLAY, STRING_VIEW("VK_PLAY"), STRING_VIEW("VK_PLAY"), STRING_VIEW(L"Play key") },
  { VK_ZOOM, STRING_VIEW("VK_ZOOM"), STRING_VIEW("VK_ZOOM"), STRING_VIEW(L"Zoom key") },
  { VK_NONAME, STRING_VIEW("VK_NONAME"), STRING_VIEW("VK_NONAME"), STRING_VIEW(L"Reserved") },
  { VK_PA1, STRING_VIEW("VK_PA1"), STRING_VIEW("VK_PA1"), STRING_VIEW(L"PA1 key") },
  { VK_OEM_CLEAR, STRING_VIEW("VK_OEM_CLEAR"), STRING_VIEW("VK_OEM_CLEAR"), STRING_VIEW(L"Clear key") },

  { 0xFF, STRING_VIEW("None"), STRING_VIEW("None"), STRING_VIEW(L"Reserved") },
};

enum {
  kVirtualKeyTableCount =
      sizeof(kVirtualKeyTable) / sizeof(kVirtualKeyTable[0]),

  /*
   * The value of this constant may increase if there is a longer
   * symbol name, but it's unlikely.
   */
  kSymbolNameMaxLength = 63
};

static int Input_VirtualKey_CompareCodeAsVoid(
    const void* left, const void* right) {
  return Input_VirtualKey_CompareCode(left, right);
}

static struct VirtualKeySymbolNameTable global_virtual_key_symbol_table;

static void VirtualKeySymbolNameTable_InitGlobal(void) {
  static int inited = 0;
  struct VirtualKeySymbolNameTable* table_init_result;

  size_t i;

  if (inited) {
    return;
  }

  table_init_result =
      VirtualKeySymbolNameTable_Init(
          &global_virtual_key_symbol_table, kVirtualKeyTableCount * 2);

  for (i = 0; i < kVirtualKeyTableCount; ++i) {
    VirtualKeySymbolNameTable_Insert(
        &global_virtual_key_symbol_table, &kVirtualKeyTable[i]);
  }

  inited = 1;
}

/**
 * External
 */

const struct Input_VirtualKey* Input_VirtualKey_kUnset = &kVirtualKeyTable[0];

int Input_VirtualKey_CompareCode(
    const struct Input_VirtualKey* left, const struct Input_VirtualKey* right) {
  BYTE left_code;
  BYTE right_code;

  left_code = left->code;
  right_code = right->code;

  return (left_code < right_code) ? -1 : (left_code > right_code);
}

const struct Input_VirtualKey* Input_VirtualKey_GetFromCode(BYTE code) {
  struct Input_VirtualKey search_key;
  const struct Input_VirtualKey* search_result;

  search_key.code = code;

  search_result =
      bsearch(
          &search_key,
          kVirtualKeyTable,
          kVirtualKeyTableCount,
          sizeof(kVirtualKeyTable[0]),
          &Input_VirtualKey_CompareCodeAsVoid);
  if (search_result == NULL) {
    return NULL;
  }

  return search_result;
}

const struct Input_VirtualKey* Input_VirtualKey_GetFromSymbolName(
    const char* symbol_name, size_t symbol_name_length) {
  char uppercase_symbol_name[kSymbolNameMaxLength + 1];

  VirtualKeySymbolNameTable_InitGlobal();

  if (symbol_name_length >= kSymbolNameMaxLength) {
    return NULL;
  }

  Ascii_ToUpperStr(uppercase_symbol_name, symbol_name, symbol_name_length);

  return VirtualKeySymbolNameTable_Get(
      &global_virtual_key_symbol_table,
      uppercase_symbol_name,
      symbol_name_length);
}
