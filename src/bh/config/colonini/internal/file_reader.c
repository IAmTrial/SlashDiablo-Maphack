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

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "bh/config/colonini/type.h"

static const char kWinNewline[] = "\r\n";

enum {
  FileReader_kLineInitialCapacity = 2048,
  kWinNewlineLength = (sizeof(kWinNewline) / sizeof(kWinNewline[0])) - 1 
};

static struct FileReader* FileReader_DoubleBufferCapacity(
    struct FileReader* reader) {
  void* alloc_result;
  size_t new_capacity;

  new_capacity = reader->buffer_capacity * 2;
  alloc_result = realloc(reader->buffer, new_capacity);
  if (alloc_result == NULL) {
    goto error;
  }
  reader->buffer = alloc_result;
  reader->buffer_capacity = new_capacity;

  return reader;

error:
  return NULL;
}


/**
 * Finds and return the pointer to the newline character that ends the
 * entry.
 */
static char* FindLineEnd(char* str, size_t str_length, size_t* i_line_end) {
  char* line_end;

  line_end = memchr(str, '\n', str_length);
  if (line_end == NULL) {
    *i_line_end = str_length;
  } else {
    if (line_end[-1] == '\r') {
      line_end -= 1;
    }
    *i_line_end = line_end - str;
  }

  return line_end;
}

static struct FileReader* FileReader_ReadToBuffer(
    struct FileReader* reader,
    DWORD expected_read_count,
    DWORD* actual_read_count) {
  BOOL read_file_succeeded;

  if (FileReader_IsEof(reader)) {
    *actual_read_count = 0;
    return reader;
  }

  read_file_succeeded =
      ReadFile(
          reader->file,
          &reader->buffer[reader->buffer_length],
          expected_read_count,
          actual_read_count,
          NULL);
  if (!read_file_succeeded) {
    reader->status = FileReader_Status_kReadFail;
    goto error;
  }
  reader->buffer_length += *actual_read_count;

  if (expected_read_count > 0 && *actual_read_count == 0) {
    reader->status = FileReader_Status_kEof;
  } else {
    reader->status = FileReader_Status_kGood;
  }

  return reader;

error:
  return NULL;
}

/**
 * External
 */

const char* FileReader_Status_ToDisplayName(enum FileReader_Status status) {
  switch (status) {
    case FileReader_Status_kGood: {
      static const char kDisplayName[] = "Good";
      return kDisplayName;
    }

    case FileReader_Status_kEof: {
      static const char kDisplayName[] = "End of File";
      return kDisplayName;
    }

    case FileReader_Status_kReadFail: {
      static const char kDisplayName[] = "Read Fail";
      return kDisplayName;
    }

    case FileReader_Status_kOpenFail: {
      static const char kDisplayName[] = "Open Fail";
      return kDisplayName;
    }

    default: {
      assert(0 && "This should not happen.");
      return NULL;
    }
  }
}

struct FileReader* FileReader_OpenFile(
    struct FileReader* reader, const wchar_t* path) {
  size_t i;

  wchar_t* path_null;
  size_t path_length;

  /* Get the length of the path, up to MAX_PATH characters. */
  path_null = wmemchr(path, L'\0', MAX_PATH);
  if (path_null == NULL) {
    reader->status = FileReader_Status_kOpenFail;
    goto error;
  }
  path_length = path_null - path;

  /* Copy the path to the reader. */
  wmemcpy(reader->path, path, path_length + 1);

  reader->file =
      CreateFileW(
          path,
          GENERIC_READ,
          0,
          NULL,
          OPEN_EXISTING,
          FILE_ATTRIBUTE_NORMAL,
          NULL);
  if (reader->file == INVALID_HANDLE_VALUE) {
    reader->status = FileReader_Status_kOpenFail;
    return NULL;
  }

  reader->buffer_capacity = FileReader_kLineInitialCapacity;
  reader->buffer = malloc(reader->buffer_capacity * sizeof(reader->buffer[0]));
  if (reader->buffer == NULL) {
    reader->status = FileReader_Status_kOpenFail;
    goto error_close_file;
  }
  reader->buffer_length = 0;

  reader->status = FileReader_Status_kGood;

  return reader;

error_close_file:
  CloseHandle(reader->file);
  reader->file = NULL;

error:
  return NULL;
}

void FileReader_CloseFile(struct FileReader* reader) {
  free(reader->buffer);
  reader->buffer = NULL;

  CloseHandle(reader->file);
  reader->file = NULL;
}

int FileReader_IsEof(struct FileReader* reader) {
  return reader->status == FileReader_Status_kEof;
}

char* FileReader_ReadLine(
    struct FileReader* reader, char* line, size_t line_capacity, size_t* line_length) {
  BOOL read_file_succeeded;

  DWORD expected_read_count;
  DWORD actual_read_count;

  char* line_end;
  size_t i_next_line_start;

  /* Check the buffer for a newline, and read to buffer as needed. */
  do {
    struct FileReader* read_result;
    DWORD current_actual_read_count;

    /* If a newline is found, leave the loop. */
    line_end = FindLineEnd(reader->buffer, reader->buffer_length, line_length);
    if (line_end != NULL) {
      break;
    }

    /* If EOF, there is nothing left to read. */
    if (reader->status == FileReader_Status_kEof) {
      break;
    }

    /* If reader's buffer size is insufficient, double the capacity. */
    if (reader->buffer_length == reader->buffer_capacity - 1) {
      FileReader_DoubleBufferCapacity(reader);
    }

    /* Read to buffer. */
    read_result =
        FileReader_ReadToBuffer(
            reader,
            reader->buffer_capacity - reader->buffer_length - 1,
            &current_actual_read_count);
    if (read_result == NULL) {
      goto error;
    }
  } while (reader->status == FileReader_Status_kGood);

  /* If line capacity is insufficient, do not copy into line. */
  if (line_capacity < *line_length + 1) {
    return line;
  }

  memcpy(line, reader->buffer, *line_length);
  line[*line_length] = '\0';

  /* Move everything after newline into the start of buffer. */
  i_next_line_start = *line_length;
  if (memcmp(&reader->buffer[*line_length], kWinNewline, kWinNewlineLength) == 0) {
    i_next_line_start += kWinNewlineLength;
  } else if (reader->buffer[*line_length] == '\n') {
    i_next_line_start += 1;
  }
  reader->buffer_length -= i_next_line_start;

  memmove(
      reader->buffer,
      &reader->buffer[i_next_line_start],
      reader->buffer_length);
  reader->buffer[reader->buffer_length] = '\0';

  return line;

error:
  return NULL;
}
