/**
*
* D2Ptrs.h
* BH: Copyright 2011 (C) McGod
* SlashDiablo Maphack: Copyright (C) SlashDiablo Team
*
*  This file is part of SlashDiablo Maphack.
*
*  SlashDiablo Maphack is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Affero General Public License as published
*  by the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Affero General Public License for more details.
*
*  You should have received a copy of the GNU Affero General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* This file incorporates work covered by the following copyright and
* permission notice:
*
*   ==========================================================
*   D2Ex2
*   https://github.com/lolet/D2Ex2
*   ==========================================================
*   Copyright (c) 2011-2014 Bartosz Jankowski
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*   ==========================================================
*
*/

#include "D2Version.h"
#include "Patch.h"

#define FUNCPTR(dll, name, callingret, args, ...) \
	static Offsets f##dll##_##name##_offsets = { __VA_ARGS__ }; \
	__declspec(naked) callingret dll##_##name args \
	{ \
		static DWORD f##dll##_##name = NULL; \
		if(f##dll##_##name == NULL) \
		{ \
		__asm { pushad } \
		f##dll##_##name = Patch::GetDllOffset(dll, *(&f##dll##_##name##_offsets._113c + D2Version::GetGameVersionID())); \
		__asm { popad } \
		} \
		__asm jmp [f##dll##_##name] \
	}

#define ASMPTR(dll, name, ...) \
	DWORD* Asm_##dll##_##name(VOID) \
	{ \
		static DWORD fAsm_##dll##_##name = NULL; \
		if(fAsm_##dll##_##name == NULL) \
		{ \
		static Offsets fAsm_##name##_offsets = { __VA_ARGS__ }; \
		static int address = *(&fAsm_##name##_offsets._113c + D2Version::GetGameVersionID()); \
		fAsm_##dll##_##name = Patch::GetDllOffset(dll, address); \
		} \
		return &fAsm_##dll##_##name; \
	}

#define VARPTR(dll, name, type, ...) \
	type** Var_##dll##_##name(VOID) \
	{ \
		static DWORD fVar_##dll##_##name = NULL; \
		if(fVar_##dll##_##name == NULL) \
		{ \
		static Offsets fVar_##name##_offsets = { __VA_ARGS__ }; \
		static int address = *(&fVar_##name##_offsets._113c + D2Version::GetGameVersionID()); \
		fVar_##dll##_##name = Patch::GetDllOffset(dll, address); \
		} \
		return (type**)&fVar_##dll##_##name; \
	}

// This must appear before including D2Ptrs.h.
#define _DEFINE_PTRS
#include "D2Ptrs.h"
#undef _DEFINE_PTRS
