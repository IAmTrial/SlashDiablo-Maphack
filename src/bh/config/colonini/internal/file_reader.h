/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#ifndef BH_CONFIG_COLONINI_INTERNAL_FILE_READER_H_
#define BH_CONFIG_COLONINI_INTERNAL_FILE_READER_H_

#include <stddef.h>
#include <wchar.h>
#include <windows.h>

#include "bh/config/colonini/type.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

enum FileReader_Status {
  FileReader_Status_kUnspecified = 0,

  FileReader_Status_kGood,
  FileReader_Status_kEof,
  FileReader_Status_kReadFail,
  FileReader_Status_kOpenFail
};

const char* FileReader_Status_ToDisplayName(enum FileReader_Status status);

struct FileReader {
  enum FileReader_Status status;
  wchar_t path[MAX_PATH];
  HANDLE file;

  char* buffer;
  size_t buffer_capacity;
  size_t buffer_length;
};

struct FileReader* FileReader_OpenFile(
    struct FileReader* reader, const wchar_t* path);

void FileReader_CloseFile(struct FileReader* reader);

int FileReader_IsEof(struct FileReader* reader);

char* FileReader_ReadLine(
    struct FileReader* reader,
    char* line,
    size_t line_capacity,
    size_t* line_length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_FILE_READER_H_ */
