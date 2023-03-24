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

#include "bh/config/colonini/internal/file_reader.h"

#include <stddef.h>
#include <windows.h>

#include "bh/config/colonini/type.h"

struct FileReader* FileReader_OpenFile(
    struct FileReader* file_reader, const wchar_t* path) {
  size_t i;

  size_t path_length;
  HANDLE file;

  /* Get the length of the path, up to MAX_PATH characters. */
  for (i = 0; i < MAX_PATH && path[i] != '\0'; ++i) {
    if (i >= MAX_PATH) {
      return NULL;
    }
  }
  path_length = i;

  memcpy(
      file_reader->path,
      path,
      (path_length + 1) * sizeof(file_reader->path[0]));

  file =
      CreateFileW(
          path,
          GENERIC_READ,
          0,
          NULL,
          OPEN_EXISTING,
          FILE_ATTRIBUTE_NORMAL,
          NULL);
  if (file == INVALID_HANDLE_VALUE) {
    return NULL;
  }
  file_reader->file = file;

  file_reader->status = FileReader_FileStatus_kGood;

  return file_reader;
}

void FileReader_CloseFile(struct FileReader* file_reader) {
  CloseHandle(file_reader);
}

int FileReader_IsEof(struct FileReader* file_reader) {
  return file_reader->status == FileReader_Status_kEof;
}

char* FileReader_ReadEntry(
    HANDLE file, char* buffer, size_t buffer_capacity, size_t* buffer_length) {
  /* TODO (Mir Drualga): Stub */
  return buffer;
}
