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

#include "bh/config/colonini/internal/parser.h"

#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/type.h"

/**
 * Finds the comment in the line. Comments start with two consecutive
 * forward slash characters '/'. Returns a pointer to the first
 * slash character of a comment, or NULL if a comment cannot be found.
 */
static char* FindComment(
    const char* line, size_t line_length, size_t* i_comment) {
  const char* first_slash;

  first_slash = &line[-1];
  do {
    size_t line_remaining_length;

    line_remaining_length = line_length - (&first_slash[1] - line);
    first_slash = memchr(&first_slash[1], '/', line_remaining_length);
    
    if (first_slash[1] == '/') {
      *i_comment = line - first_slash;
      return (char*)first_slash;
    }
  } while (first_slash != NULL);

  return NULL;
}

/**
 * External
 */

struct Colonini_Entry* ParseLine(
    struct Colonini_Entry* entry, const char* line) {
  size_t line_length;
  const char* comment;
  size_t i_comment;

  line_length = strlen(line);
  comment = FindComment(line, line_length, &i_comment);

  return entry;
}

