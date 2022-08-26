/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
 *
 * BH
 * Copyright 2011 (C) McGod
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

#include "Input.h"

#include <ctype.h>
#include <stddef.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace common::input {
namespace {

/**
 * This table is only used for initialization purposes.
 */
static constexpr std::array<VirtualKey, 259> kVirtualKeyTable = {{
  VirtualKey::GetUnset(),

  { VK_LBUTTON, "VK_LBUTTON", "VK_LBUTTON", "Left mouse button" },
  { VK_RBUTTON, "VK_RBUTTON", "VK_RBUTTON", "Right mouse button" },
  { VK_CANCEL, "VK_CANCEL", "VK_CANCEL", "Control-break processing" },
  { VK_MBUTTON, "VK_MBUTTON", "VK_MBUTTON", "Middle mouse button (three-button mouse)" },
  { VK_XBUTTON1, "VK_XBUTTON1", "VK_XBUTTON1", "X1 mouse button" },
  { VK_XBUTTON2, "VK_XBUTTON2", "VK_XBUTTON2", "X2 mouse button" },

  { 0x07, "None", "None", "Undefined" },

  { VK_BACK, "VK_BACK", "VK_BACK", "BACKSPACE key" },
  { VK_TAB, "VK_TAB", "VK_TAB", "TAB key" },

  { 0x0A, "None", "None", "Reserved" },
  { 0x0B, "None", "None", "Reserved" },

  { VK_CLEAR, "VK_CLEAR", "VK_CLEAR", "CLEAR key" },
  { VK_RETURN, "VK_RETURN", "VK_RETURN", "ENTER key" },

  { 0x0E, "None", "None", "Undefined" },
  { 0x0F, "None", "None", "Undefined" },

  { VK_SHIFT, "VK_SHIFT", "VK_SHIFT", "SHIFT key" },
  { VK_CONTROL, "VK_CONTROL", "VK_CONTROL", "CTRL key" },
  { VK_MENU, "VK_MENU", "VK_ALT", "ALT key" },
  { VK_PAUSE, "VK_PAUSE", "VK_PAUSE", "PAUSE key" },
  { VK_CAPITAL, "VK_CAPITAL", "VK_CAPSLOCK", "CAPS LOCK key" },

  { VK_KANA, "VK_KANA", "VK_KANA", "IME VK_KANA mode" },
  { VK_HANGEUL, "VK_HANGEUL", "VK_HANGEUL", "IME Hanguel mode" },
  { VK_HANGUL, "VK_HANGUL", "VK_HANGUL", "IME Hangul mode" },

  { VK_IME_ON, "VK_IME_ON", "VK_IME_ON", "IME On" },
  { VK_JUNJA, "VK_JUNJA", "VK_JUNJA", "IME Junja mode" },
  { VK_FINAL, "VK_FINAL", "VK_FINAL", "IME final mode" },
  { VK_HANJA, "VK_HANJA", "VK_HANJA", "IME Hanja mode" },
  { VK_KANJI, "VK_KANJI", "VK_KANJI", "IME Kanji mode" },
  { VK_IME_OFF, "VK_IME_OFF", "VK_IME_OFF", "IME Off" },

  { VK_ESCAPE, "VK_ESCAPE", "VK_ESCAPE", "ESC key" },
  { VK_CONVERT, "VK_CONVERT", "VK_CONVERT", "IME convert" },
  { VK_NONCONVERT, "VK_NONCONVERT", "VK_NONCONVERT", "IME nonconvert" },
  { VK_ACCEPT, "VK_ACCEPT", "VK_ACCEPT", "IME accept" },
  { VK_MODECHANGE, "VK_MODECHANGE", "VK_MODECHANGE", "IME mode change request" },
  { VK_SPACE, "VK_SPACE", "VK_SPACE", "SPACEBAR" },
  { VK_PRIOR, "VK_PRIOR", "VK_PAGEUP", "PAGE UP key" },
  { VK_NEXT, "VK_NEXT", "VK_PAGEDN", "PAGE DOWN key" },
  { VK_END, "VK_END", "VK_END", "END key" },
  { VK_HOME, "VK_HOME", "VK_HOME", "HOME key" },
  { VK_LEFT, "VK_LEFT", "VK_LEFT", "LEFT ARROW key" },
  { VK_UP, "VK_UP", "VK_UP", "UP ARROW key" },
  { VK_RIGHT, "VK_RIGHT", "VK_RIGHT", "RIGHT ARROW key" },
  { VK_DOWN, "VK_DOWN", "VK_DOWN", "DOWN ARROW key" },
  { VK_SELECT, "VK_SELECT", "VK_SELECT", "SELECT key" },
  { VK_PRINT, "VK_PRINT", "VK_PRINT", "PRINT key" },
  { VK_EXECUTE, "VK_EXECUTE", "VK_EXECUTE", "EXECUTE key" },
  { VK_SNAPSHOT, "VK_SNAPSHOT", "VK_SNAPSHOT", "PRINT SCREEN key" },
  { VK_INSERT, "VK_INSERT", "VK_INSERT", "INS key" },
  { VK_DELETE, "VK_DELETE", "VK_DELETE", "DEL key" },
  { VK_HELP, "VK_HELP", "VK_HELP", "HELP key" },

  { 0x30, "VK_0", "VK_0", "0 key" },
  { 0x31, "VK_1", "VK_1", "1 key" },
  { 0x32, "VK_2", "VK_2", "2 key" },
  { 0x33, "VK_3", "VK_3", "3 key" },
  { 0x34, "VK_4", "VK_4", "4 key" },
  { 0x35, "VK_5", "VK_5", "5 key" },
  { 0x36, "VK_6", "VK_6", "6 key" },
  { 0x37, "VK_7", "VK_7", "7 key" },
  { 0x38, "VK_8", "VK_8", "8 key" },
  { 0x39, "VK_9", "VK_9", "9 key" },

  { 0x3A, "None", "None", "Undefined" },
  { 0x3B, "None", "None", "Undefined" },
  { 0x3C, "None", "None", "Undefined" },
  { 0x3D, "None", "None", "Undefined" },
  { 0x3E, "None", "None", "Undefined" },
  { 0x3F, "None", "None", "Undefined" },
  { 0x40, "None", "None", "Undefined" },

  { 0x41, "VK_A", "VK_A", "A key" },
  { 0x42, "VK_B", "VK_B", "B key" },
  { 0x43, "VK_C", "VK_C", "C key" },
  { 0x44, "VK_D", "VK_D", "D key" },
  { 0x45, "VK_E", "VK_E", "E key" },
  { 0x46, "VK_F", "VK_F", "F key" },
  { 0x47, "VK_G", "VK_G", "G key" },
  { 0x48, "VK_H", "VK_H", "H key" },
  { 0x49, "VK_I", "VK_I", "I key" },
  { 0x4A, "VK_J", "VK_J", "J key" },
  { 0x4B, "VK_K", "VK_K", "K key" },
  { 0x4C, "VK_L", "VK_L", "L key" },
  { 0x4D, "VK_M", "VK_M", "M key" },
  { 0x4E, "VK_N", "VK_N", "N key" },
  { 0x4F, "VK_O", "VK_O", "O key" },
  { 0x50, "VK_P", "VK_P", "P key" },
  { 0x51, "VK_Q", "VK_Q", "Q key" },
  { 0x52, "VK_R", "VK_R", "R key" },
  { 0x53, "VK_S", "VK_S", "S key" },
  { 0x54, "VK_T", "VK_T", "T key" },
  { 0x55, "VK_U", "VK_U", "U key" },
  { 0x56, "VK_V", "VK_V", "V key" },
  { 0x57, "VK_W", "VK_W", "W key" },
  { 0x58, "VK_X", "VK_X", "X key" },
  { 0x59, "VK_Y", "VK_Y", "Y key" },
  { 0x5A, "VK_Z", "VK_Z", "Z key" },

  { VK_LWIN, "VK_LWIN", "VK_LWIN", "Left Windows key (Natural keyboard)" },
  { VK_RWIN, "VK_RWIN", "VK_RWIN", "Right Windows key (Natural keyboard)" },
  { VK_APPS, "VK_APPS", "VK_APPS", "Applications key (Natural keyboard)" },

  { 0x5E, "None", "None", "Reserved" },

  { VK_SLEEP, "VK_SLEEP", "VK_SLEEP", "Computer Sleep key" },

  { VK_NUMPAD0, "VK_NUMPAD0", "VK_NUMPAD0", "Numeric keypad 0 key" },
  { VK_NUMPAD1, "VK_NUMPAD1", "VK_NUMPAD1", "Numeric keypad 1 key" },
  { VK_NUMPAD2, "VK_NUMPAD2", "VK_NUMPAD2", "Numeric keypad 2 key" },
  { VK_NUMPAD3, "VK_NUMPAD3", "VK_NUMPAD3", "Numeric keypad 3 key" },
  { VK_NUMPAD4, "VK_NUMPAD4", "VK_NUMPAD4", "Numeric keypad 4 key" },
  { VK_NUMPAD5, "VK_NUMPAD5", "VK_NUMPAD5", "Numeric keypad 5 key" },
  { VK_NUMPAD6, "VK_NUMPAD6", "VK_NUMPAD6", "Numeric keypad 6 key" },
  { VK_NUMPAD7, "VK_NUMPAD7", "VK_NUMPAD7", "Numeric keypad 7 key" },
  { VK_NUMPAD8, "VK_NUMPAD8", "VK_NUMPAD8", "Numeric keypad 8 key" },
  { VK_NUMPAD9, "VK_NUMPAD9", "VK_NUMPAD9", "Numeric keypad 9 key" },

  { VK_MULTIPLY, "VK_MULTIPLY", "VK_NUMPADMULTIPLY", "Multiply key" },
  { VK_ADD, "VK_ADD", "VK_NUMPADADD", "Add key" },
  { VK_SEPARATOR, "VK_SEPARATOR", "VK_NUMPADSEPARATOR", "Separator key" },
  { VK_SUBTRACT, "VK_SUBTRACT", "VK_NUMPADSUBTRACT", "Subtract key" },
  { VK_DECIMAL, "VK_DECIMAL", "VK_NUMPADDECIMAL", "Decimal key" },
  { VK_DIVIDE, "VK_DIVIDE", "VK_NUMPADDIVIDE", "Divide key" },

  { VK_F1, "VK_F1", "VK_F1", "F1 key" },
  { VK_F2, "VK_F2", "VK_F2", "F2 key" },
  { VK_F3, "VK_F3", "VK_F3", "F3 key" },
  { VK_F4, "VK_F4", "VK_F4", "F4 key" },
  { VK_F5, "VK_F5", "VK_F5", "F5 key" },
  { VK_F6, "VK_F6", "VK_F6", "F6 key" },
  { VK_F7, "VK_F7", "VK_F7", "F7 key" },
  { VK_F8, "VK_F8", "VK_F8", "F8 key" },
  { VK_F9, "VK_F9", "VK_F9", "F9 key" },
  { VK_F10, "VK_F10", "VK_F10", "F10 key" },
  { VK_F11, "VK_F11", "VK_F11", "F11 key" },
  { VK_F12, "VK_F12", "VK_F12", "F12 key" },
  { VK_F13, "VK_F13", "VK_F13", "F13 key" },
  { VK_F14, "VK_F14", "VK_F14", "F14 key" },
  { VK_F15, "VK_F15", "VK_F15", "F15 key" },
  { VK_F16, "VK_F16", "VK_F16", "F16 key" },
  { VK_F17, "VK_F17", "VK_F17", "F17 key" },
  { VK_F18, "VK_F18", "VK_F18", "F18 key" },
  { VK_F19, "VK_F19", "VK_F19", "F19 key" },
  { VK_F20, "VK_F20", "VK_F20", "F20 key" },
  { VK_F21, "VK_F21", "VK_F21", "F21 key" },
  { VK_F22, "VK_F22", "VK_F22", "F22 key" },
  { VK_F23, "VK_F23", "VK_F23", "F23 key" },
  { VK_F24, "VK_F24", "VK_F24", "F24 key" },

  { VK_NAVIGATION_VIEW, "VK_NAVIGATION_VIEW", "VK_NAVIGATION_VIEW", "Navigation view key" },
  { VK_NAVIGATION_MENU, "VK_NAVIGATION_MENU", "VK_NAVIGATION_MENU", "Navigation menu key" },
  { VK_NAVIGATION_UP, "VK_NAVIGATION_UP", "VK_NAVIGATION_UP", "Navigation up key" },
  { VK_NAVIGATION_DOWN, "VK_NAVIGATION_DOWN", "VK_NAVIGATION_DOWN", "Navigation down key" },
  { VK_NAVIGATION_LEFT, "VK_NAVIGATION_LEFT", "VK_NAVIGATION_LEFT", "Navigation left key" },
  { VK_NAVIGATION_RIGHT, "VK_NAVIGATION_RIGHT", "VK_NAVIGATION_RIGHT", "Navigation right key" },
  { VK_NAVIGATION_ACCEPT, "VK_NAVIGATION_ACCEPT", "VK_NAVIGATION_ACCEPT", "Navigation accept key" },
  { VK_NAVIGATION_CANCEL, "VK_NAVIGATION_CANCEL", "VK_NAVIGATION_CANCEL", "Navigation cancel key" },

  { VK_NUMLOCK, "VK_NUMLOCK", "VK_NUMLOCK", "NUM LOCK key" },
  { VK_SCROLL, "VK_SCROLL", "VK_SCROLL", "SCROLL LOCK key" },

  { 0x92, "None", "None", "OEM specific" },
  { 0x93, "None", "None", "OEM specific" },
  { 0x94, "None", "None", "OEM specific" },
  { 0x95, "None", "None", "OEM specific" },
  { 0x96, "None", "None", "OEM specific" },

  { 0x97, "None", "None", "Unassigned" },
  { 0x98, "None", "None", "Unassigned" },
  { 0x99, "None", "None", "Unassigned" },
  { 0x9A, "None", "None", "Unassigned" },
  { 0x9B, "None", "None", "Unassigned" },
  { 0x9C, "None", "None", "Unassigned" },
  { 0x9D, "None", "None", "Unassigned" },
  { 0x9E, "None", "None", "Unassigned" },
  { 0x9F, "None", "None", "Unassigned" },

  { VK_LSHIFT, "VK_LSHIFT", "VK_LSHIFT", "Left SHIFT key" },
  { VK_RSHIFT, "VK_RSHIFT", "VK_RSHIFT", "Right SHIFT key" },
  { VK_LCONTROL, "VK_LCONTROL", "VK_LCTRL", "Left CONTROL key" },
  { VK_RCONTROL, "VK_RCONTROL", "VK_RCTRL", "Right CONTROL key" },
  { VK_LMENU, "VK_LMENU", "VK_LMENU", "Left ALT key" },
  { VK_RMENU, "VK_RMENU", "VK_RMENU", "Right ALT key" },

  { VK_BROWSER_BACK, "VK_BROWSER_BACK", "VK_BROWSER_BACK", "Browser Back key" },
  { VK_BROWSER_FORWARD, "VK_BROWSER_FORWARD", "VK_BROWSER_FORWARD", "Browser Forward key" },
  { VK_BROWSER_REFRESH, "VK_BROWSER_REFRESH", "VK_BROWSER_REFRESH", "Browser Refresh key" },
  { VK_BROWSER_STOP, "VK_BROWSER_STOP", "VK_BROWSER_STOP", "Browser Stop key" },
  { VK_BROWSER_SEARCH, "VK_BROWSER_SEARCH", "VK_BROWSER_SEARCH", "Browser Search key" },
  { VK_BROWSER_FAVORITES, "VK_BROWSER_FAVORITES", "VK_BROWSER_FAVORITES", "Browser Favorites key" },
  { VK_BROWSER_HOME, "VK_BROWSER_HOME", "VK_BROWSER_HOME", "Browser Start and Home key" },

  { VK_VOLUME_MUTE, "VK_VOLUME_MUTE", "VK_VOLUME_MUTE", "Volume Mute key" },
  { VK_VOLUME_DOWN, "VK_VOLUME_DOWN", "VK_VOLUME_DOWN", "Volume Down key" },
  { VK_VOLUME_UP, "VK_VOLUME_UP", "VK_VOLUME_UP", "Volume Up key" },
  { VK_MEDIA_NEXT_TRACK, "VK_MEDIA_NEXT_TRACK", "VK_MEDIA_NEXT_TRACK", "Next Track key" },
  { VK_MEDIA_PREV_TRACK, "VK_MEDIA_PREV_TRACK", "VK_MEDIA_PREV_TRACK", "Previous Track key" },
  { VK_MEDIA_STOP, "VK_MEDIA_STOP", "VK_MEDIA_STOP", "Stop Media key" },
  { VK_MEDIA_PLAY_PAUSE, "VK_MEDIA_PLAY_PAUSE", "VK_MEDIA_PLAY_PAUSE", "Play/Pause Media key" },
  { VK_LAUNCH_MAIL, "VK_LAUNCH_MAIL", "VK_LAUNCH_MAIL", "Start Mail key" },
  { VK_LAUNCH_MEDIA_SELECT, "VK_LAUNCH_MEDIA_SELECT", "VK_LAUNCH_MEDIA_SELECT", "Select Media key" },
  { VK_LAUNCH_APP1, "VK_LAUNCH_APP1", "VK_LAUNCH_APP1", "Start Application 1 key" },
  { VK_LAUNCH_APP2, "VK_LAUNCH_APP2", "VK_LAUNCH_APP2", "Start Application 2 key" },

  { 0xB8, "None", "None", "Reserved" },
  { 0xB9, "None", "None", "Reserved" },

  { VK_OEM_1, "VK_OEM_1", "VK_SEMICOLON", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key" },
  { VK_OEM_PLUS, "VK_OEM_PLUS", "VK_PLUS", "For any country/region, the '+' key" },
  { VK_OEM_COMMA, "VK_OEM_COMMA", "VK_COMMA", "For any country/region, the ',' key" },
  { VK_OEM_MINUS, "VK_OEM_MINUS", "VK_MINUS", "For any country/region, the '-' key" },
  { VK_OEM_PERIOD, "VK_OEM_PERIOD", "VK_PERIOD", "For any country/region, the '.' key" },
  { VK_OEM_2, "VK_OEM_2", "VK_FORWARDSLASH", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key" },
  { VK_OEM_3, "VK_OEM_3", "VK_TILDE", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key" },

  { 0xC1, "None", "None", "Reserved" },
  { 0xC2, "None", "None", "Reserved" },

  { VK_GAMEPAD_A, "VK_GAMEPAD_A", "VK_GAMEPAD_A", "Gamepad A button" },
  { VK_GAMEPAD_B, "VK_GAMEPAD_B", "VK_GAMEPAD_B", "Gamepad B button" },
  { VK_GAMEPAD_X, "VK_GAMEPAD_X", "VK_GAMEPAD_X", "Gamepad C button" },
  { VK_GAMEPAD_Y, "VK_GAMEPAD_Y", "VK_GAMEPAD_Y", "Gamepad Y button" },
  { VK_GAMEPAD_RIGHT_SHOULDER, "VK_GAMEPAD_RIGHT_SHOULDER", "VK_GAMEPAD_RIGHT_SHOULDER", "Gamepad right shoulder press" },
  { VK_GAMEPAD_LEFT_SHOULDER, "VK_GAMEPAD_LEFT_SHOULDER", "VK_GAMEPAD_LEFT_SHOULDER", "Gamepad left shoulder press" },
  { VK_GAMEPAD_LEFT_TRIGGER, "VK_GAMEPAD_LEFT_TRIGGER", "VK_GAMEPAD_LEFT_TRIGGER", "Gamepad left shoulder trigger" },
  { VK_GAMEPAD_RIGHT_TRIGGER, "VK_GAMEPAD_RIGHT_TRIGGER", "VK_GAMEPAD_RIGHT_TRIGGER", "Gamepad right shoulder trigger" },
  { VK_GAMEPAD_DPAD_UP, "VK_GAMEPAD_DPAD_UP", "VK_GAMEPAD_DPAD_UP", "Gamepad D-pad up" },
  { VK_GAMEPAD_DPAD_DOWN, "VK_GAMEPAD_DPAD_DOWN", "VK_GAMEPAD_DPAD_DOWN", "Gamepad D-pad down" },
  { VK_GAMEPAD_DPAD_LEFT, "VK_GAMEPAD_DPAD_LEFT", "VK_GAMEPAD_DPAD_LEFT", "Gamepad D-pad left" },
  { VK_GAMEPAD_DPAD_RIGHT, "VK_GAMEPAD_DPAD_RIGHT", "VK_GAMEPAD_DPAD_RIGHT", "Gamepad D-pad right" },
  { VK_GAMEPAD_MENU, "VK_GAMEPAD_MENU", "VK_GAMEPAD_MENU", "Gamepad menu button" },
  { VK_GAMEPAD_VIEW, "VK_GAMEPAD_VIEW", "VK_GAMEPAD_VIEW", "Gamepad view button" },
  { VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON", "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON", "Gamepad left thumbstick button" },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON", "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON", "Gamepad right thumbstick button" },
  { VK_GAMEPAD_LEFT_THUMBSTICK_UP, "VK_GAMEPAD_LEFT_THUMBSTICK_UP", "VK_GAMEPAD_LEFT_THUMBSTICK_UP", "Gamepad left thumbstick move up" },
  { VK_GAMEPAD_LEFT_THUMBSTICK_DOWN, "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN", "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN", "Gamepad left thumbstick move down" },
  { VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT, "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT", "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT", "Gamepad left thumbstick move right" },
  { VK_GAMEPAD_LEFT_THUMBSTICK_LEFT, "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT", "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT", "Gamepad left thumbstick move left" },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_UP, "VK_GAMEPAD_RIGHT_THUMBSTICK_UP", "VK_GAMEPAD_RIGHT_THUMBSTICK_UP", "Gamepad right thumbstick move up" },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN, "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN", "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN", "Gamepad right thumbstick move down" },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT, "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT", "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT", "Gamepad right thumbstick move right" },
  { VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT, "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT", "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT", "Gamepad right thumbstick move left" },

  { VK_OEM_4, "VK_OEM_4", "VK_LEFTBRACKET", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key" },
  { VK_OEM_5, "VK_OEM_5", "VK_BACKSLASH", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\\|' key" },
  { VK_OEM_6, "VK_OEM_6", "VK_RIGHTBRACKET", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key" },
  { VK_OEM_7, "VK_OEM_7", "VK_QUOTE", "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key" },
  { VK_OEM_8, "VK_OEM_8", "VK_OEM_8", "Used for miscellaneous characters; it can vary by keyboard." },

  { 0xE0, "None", "None", "Reserved" },
  { VK_OEM_AX, "VK_OEM_AX", "VK_OEM_AX", "'AX' key on Japanese AX kbd" },
  { VK_OEM_102, "VK_OEM_102", "VK_OEM_102", "The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard" },
  { VK_ICO_HELP, "VK_ICO_HELP", "VK_ICO_HELP", "Help key on ICO" },
  { VK_ICO_00, "VK_ICO_00", "VK_ICO_00", "00 key on ICO" },
  { VK_PROCESSKEY, "VK_PROCESSKEY", "VK_PROCESSKEY", "IME PROCESS key" },
  { VK_ICO_CLEAR, "VK_ICO_CLEAR", "VK_ICO_CLEAR", "Clear key on ICO" },

  { VK_PACKET, "VK_PACKET", "VK_PACKET", "Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP" },

  { 0xE8, "None", "None", "Unassigned" },

  { VK_OEM_RESET, "VK_OEM_RESET", "VK_OEM_RESET", "Nokia/Ericsson Reset key" },
  { VK_OEM_JUMP, "VK_OEM_JUMP", "VK_OEM_JUMP", "Nokia/Ericsson Jump key" },
  { VK_OEM_PA1, "VK_OEM_PA1", "VK_OEM_PA1", "Nokia/Ericsson PA1 key" },
  { VK_OEM_PA2, "VK_OEM_PA2", "VK_OEM_PA2", "Nokia/Ericsson PA2 key" },
  { VK_OEM_PA3, "VK_OEM_PA3", "VK_OEM_PA3", "Nokia/Ericsson PA3 key" },
  { VK_OEM_WSCTRL, "VK_OEM_WSCTRL", "VK_OEM_WSCTRL", "Nokia/Ericsson WsCtrl key" },
  { VK_OEM_CUSEL, "VK_OEM_CUSEL", "VK_OEM_CUSEL", "Nokia/Ericsson CuSel key" },
  { VK_OEM_ATTN, "VK_OEM_ATTN", "VK_OEM_ATTN", "Nokia/Ericsson Attn key" },
  { VK_OEM_FINISH, "VK_OEM_FINISH", "VK_OEM_FINISH", "Nokia/Ericsson Finish key" },
  { VK_OEM_COPY, "VK_OEM_COPY", "VK_OEM_COPY", "Nokia/Ericsson Copy key" },
  { VK_OEM_AUTO, "VK_OEM_AUTO", "VK_OEM_AUTO", "Nokia/Ericsson Auto key" },
  { VK_OEM_ENLW, "VK_OEM_ENLW", "VK_OEM_ENLW", "Nokia/Ericsson Enlw key" },
  { VK_OEM_BACKTAB, "VK_OEM_BACKTAB", "VK_OEM_BACKTAB", "Nokia/Ericsson BackTab key" },

  { VK_ATTN, "VK_ATTN", "VK_ATTN", "Attn key" },
  { VK_CRSEL, "VK_CRSEL", "VK_CRSEL", "CrSel key" },
  { VK_EXSEL, "VK_EXSEL", "VK_EXSEL", "ExSel key" },
  { VK_EREOF, "VK_EREOF", "VK_EREOF", "Erase EOF key" },
  { VK_PLAY, "VK_PLAY", "VK_PLAY", "Play key" },
  { VK_ZOOM, "VK_ZOOM", "VK_ZOOM", "Zoom key" },
  { VK_NONAME, "VK_NONAME", "VK_NONAME", "Reserved" },
  { VK_PA1, "VK_PA1", "VK_PA1", "PA1 key" },
  { VK_OEM_CLEAR, "VK_OEM_CLEAR", "VK_OEM_CLEAR", "Clear key" },

  { 0xFF, "None", "None", "Reserved" },
}};

static inline constexpr std::array<
    VirtualKey, std::numeric_limits<BYTE>::max() + 1>
        InitVirtualKeyByCodeTable() {
  // The array size needs to be +1 to ensure constexpr satisfaction.
  // This is due to the compiler thinking there might be an index
  // out-of-bounds write.
  std::array<VirtualKey, std::numeric_limits<BYTE>::max() + 1> table = {};

  size_t i = 0;
  for (const VirtualKey& virtual_key : kVirtualKeyTable) {
    if (virtual_key.code == i) {
      table[i] = virtual_key;
      ++i;
    }
  }

  return table;
}

static std::unordered_map<std::string, VirtualKey>
    InitVirtualKeyBySymbolNameTable() {
  std::unordered_map<std::string, VirtualKey> table(
      kVirtualKeyTable.size());
  for (const VirtualKey& virtual_key : kVirtualKeyTable) {
    // Initialize the table keys with lowercase symbol names.
    std::string lower_symbol_name(virtual_key.symbol_name);
    std::transform(
        lower_symbol_name.cbegin(),
        lower_symbol_name.cend(),
        lower_symbol_name.begin(),
        &tolower);

    table.insert(std::make_pair(std::move(lower_symbol_name), virtual_key));

    // Avoid doing unnecessary work.
    if (virtual_key.symbol_name == virtual_key.old_symbol_name) {
      continue;
    }

    std::string lower_old_symbol_name(virtual_key.old_symbol_name);
    std::transform(
        lower_old_symbol_name.cbegin(),
        lower_old_symbol_name.cend(),
        lower_old_symbol_name.begin(),
        &tolower);

    table.insert(
        std::make_pair(std::move(lower_old_symbol_name), virtual_key));
  }

  return table;
}

}  // namespace

const VirtualKey& VirtualKey::GetFromCode(BYTE code) {
  static constexpr std::array kSortedVirtualKeyByCodeTable =
      InitVirtualKeyByCodeTable();

  return kSortedVirtualKeyByCodeTable[code];
}

std::optional<VirtualKey> VirtualKey::GetFromSymbolName(
    std::string_view symbol_name) {
  static const std::unordered_map<std::string, VirtualKey>
      kVirtualKeyBySymbolNameTable = InitVirtualKeyBySymbolNameTable();

  // Defense against arbitrary string parsing.
  if (symbol_name.length() > 64) {
    return std::nullopt;
  }

  // Case-insensitive compare.
  std::string lower_symbol_name(symbol_name);
  std::transform(
      lower_symbol_name.cbegin(),
      lower_symbol_name.cend(),
      lower_symbol_name.begin(),
      &tolower);

  const auto& find_result =
      kVirtualKeyBySymbolNameTable.find(lower_symbol_name);
  if (find_result == kVirtualKeyBySymbolNameTable.cend()) {
    return std::nullopt;
  }

  return find_result->second;
}

}  // namespace common::input
