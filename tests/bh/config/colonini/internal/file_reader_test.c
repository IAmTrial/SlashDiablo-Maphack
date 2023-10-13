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

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

#include <bh/config/colonini/internal/file_reader.h>

static wchar_t current_directory[MAX_PATH];
static wchar_t empty_path[MAX_PATH];
static wchar_t nonexistent_path[MAX_PATH];
static wchar_t one_line_path[MAX_PATH];
static wchar_t one_line_newline_path[MAX_PATH];
static wchar_t two_line_path[MAX_PATH];
static wchar_t two_line_newline_path[MAX_PATH];
static wchar_t continuation_path[MAX_PATH];
static wchar_t continuation_newline_path[MAX_PATH];

static void BeforeAllSetUp(void) {
  GetModuleFileNameW(NULL, current_directory, MAX_PATH);

  wmemcpy(empty_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(empty_path);
  PathCombineW(empty_path, empty_path, L"data/empty.cfg");

  wmemcpy(nonexistent_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(nonexistent_path);
  PathCombineW(
      nonexistent_path, nonexistent_path, L"data/won't exist\1\2\3\4\5\6.cfg");

  wmemcpy(one_line_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(one_line_path);
  PathCombineW(one_line_path, one_line_path, L"data/one_line.cfg");

  wmemcpy(one_line_newline_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(one_line_newline_path);
  PathCombineW(
      one_line_newline_path,
      one_line_newline_path,
      L"data/one_line_newline.cfg");

  wmemcpy(two_line_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(two_line_path);
  PathCombineW(two_line_path, two_line_path, L"data/two_line.cfg");

  wmemcpy(two_line_newline_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(two_line_newline_path);
  PathCombineW(
      two_line_newline_path,
      two_line_newline_path,
      L"data/two_line_newline.cfg");

  wmemcpy(continuation_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(continuation_path);
  PathCombineW(continuation_path, continuation_path, L"data/continuation.cfg");

  wmemcpy(continuation_newline_path, current_directory, MAX_PATH);
  PathRemoveFileSpecW(continuation_newline_path);
  PathCombineW(
      continuation_newline_path,
      continuation_newline_path,
      L"data/continuation_newline.cfg");
}

typedef void TestFunc(void);

static void OpenFile_Exists_SetSuccess(void) {
  struct FileReader reader;
  struct FileReader* open_result;

  open_result = FileReader_OpenFile(&reader, empty_path);

  assert(open_result == &reader);
  assert(reader.status == FileReader_Status_kGood);
  assert(wcscmp(reader.path, empty_path) == 0);
  assert(reader.file != INVALID_HANDLE_VALUE);
  assert(reader.buffer_length == 0);

  FileReader_CloseFile(&reader);
}

static void OpenFile_NotExists_SetFailure(void) {
  struct FileReader reader;
  struct FileReader* open_result;

  open_result = FileReader_OpenFile(&reader, nonexistent_path);

  assert(open_result == NULL);
  assert(reader.status == FileReader_Status_kOpenFail);
}

static void ReadLine_OneLine_Success(void) {
  static const char kExpected[] = "// this is a comment";
  enum {
    kLineCapacity = 256,
    kExpectedLength = (sizeof(kExpected) / sizeof(kExpected[0])) - 1
  };

  char* read_result;
  struct FileReader reader;
  char line[kLineCapacity];
  size_t line_length;

  FileReader_OpenFile(&reader, one_line_path);
  read_result =
      FileReader_ReadLine(&reader, line, kLineCapacity, &line_length);

  assert(read_result == line);
  assert(line_length == kExpectedLength);
  assert(memcmp(line, kExpected, kExpectedLength + 1) == 0);
  assert(FileReader_IsEof(&reader));

  FileReader_CloseFile(&reader);
}

static void ReadLine_OneLineNewline_Success(void) {
  static const char kExpected[] = "// this is a comment";
  enum {
    kLineCapacity = 256,
    kExpectedLength = (sizeof(kExpected) / sizeof(kExpected[0])) - 1
  };

  struct FileReader reader;

  char* read_result;
  char line[kLineCapacity];
  size_t line_length;

  char* empty_read_result;
  char empty_line[kLineCapacity];
  size_t empty_line_length;

  FileReader_OpenFile(&reader, one_line_newline_path);
  read_result =
      FileReader_ReadLine(&reader, line, kLineCapacity, &line_length);
  empty_read_result =
      FileReader_ReadLine(
          &reader, empty_line, kLineCapacity, &empty_line_length);

  assert(read_result == line);
  assert(line_length == kExpectedLength);
  assert(strcmp(line, kExpected) == 0);
  assert(empty_read_result == empty_line);
  assert(empty_line_length == 0);
  assert(strlen(empty_line) == 0);
  assert(FileReader_IsEof(&reader));

  FileReader_CloseFile(&reader);
}

static void ReadLine_TwoLine_Success(void) {
  static const char* const kExpected[] = {
    "// this is a comment",
    "key: value"
  };
  enum {
    kLineCapacity = 256,
  };

  struct FileReader reader;

  char line1[kLineCapacity];
  size_t line1_length;
  char* read1_result;

  char line2[kLineCapacity];
  size_t line2_length;
  char* read2_result;

  FileReader_OpenFile(&reader, two_line_path);
  read1_result =
      FileReader_ReadLine(&reader, line1, kLineCapacity, &line1_length);
  read2_result =
      FileReader_ReadLine(&reader, line2, kLineCapacity, &line2_length);

  assert(read1_result == line1);
  assert(strcmp(line1, kExpected[0]) == 0);
  assert(read2_result == line2);
  assert(strcmp(line2, kExpected[1]) == 0);
  assert(FileReader_IsEof(&reader));

  FileReader_CloseFile(&reader);
}

static void ReadLine_TwoLineNewline_Success(void) {
  static const char* const kExpected[] = {
    "// this is a comment",
    "key: value"
  };
  enum {
    kLineCapacity = 256,
  };

  struct FileReader reader;

  char line1[kLineCapacity];
  size_t line1_length;
  char* read1_result;

  char line2[kLineCapacity];
  size_t line2_length;
  char* read2_result;

  char empty_line[kLineCapacity];
  size_t empty_line_length;
  char* empty_read_result;

  FileReader_OpenFile(&reader, two_line_path);
  read1_result =
      FileReader_ReadLine(&reader, line1, kLineCapacity, &line1_length);
  read2_result =
      FileReader_ReadLine(&reader, line2, kLineCapacity, &line2_length);
  empty_read_result =
      FileReader_ReadLine(
          &reader, empty_line, kLineCapacity, &empty_line_length);

  assert(read1_result == line1);
  assert(strcmp(line1, kExpected[0]) == 0);
  assert(read2_result == line2);
  assert(strcmp(line2, kExpected[1]) == 0);
  assert(empty_read_result == empty_line);
  assert(strlen(empty_line) == 0);
  assert(FileReader_IsEof(&reader));

  FileReader_CloseFile(&reader);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &OpenFile_Exists_SetSuccess,
    &OpenFile_NotExists_SetFailure,
    &ReadLine_OneLine_Success,
    &ReadLine_OneLineNewline_Success,
    &ReadLine_TwoLine_Success,
    &ReadLine_TwoLineNewline_Success
  };

  enum {
    kTestsCount = sizeof(kTests) / sizeof(kTests[0])
  };

  size_t i;

  BeforeAllSetUp();

  printf("Running %u test(s).\n", kTestsCount);
  for (i = 0; i < kTestsCount; ++i) {
    kTests[i]();
  }
  printf("Ran %u test(s).\n", kTestsCount);

#endif

  return 0;
}
