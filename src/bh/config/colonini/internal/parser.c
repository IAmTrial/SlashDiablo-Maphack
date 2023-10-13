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

#include "bh/config/colonini/internal/parser.h"

#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/type.h"

/**
 * Finds the comment in the line. Comments start with two consecutive
 * forward slash characters '/'. Returns a pointer to the first
 * slash character of a comment, or NULL if a comment cannot be found.
 */
static const char* FindComment(const char* line) {
  char* first_slash;

  for (first_slash = strchr(line, '/');
      first_slash != NULL;
      first_slash = strchr(first_slash, '/')) {
    if (first_slash[1] == '/') {
      return first_slash;
    }
  }

  return NULL;
}

/**
 * External
 */

struct Colonini_Entry* ParseLine(
    struct Colonini_Entry* entry, const char* line) {
  const char* comment;

  comment = FindComment(line);

  return entry;
}

