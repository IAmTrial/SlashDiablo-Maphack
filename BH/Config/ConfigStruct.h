/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
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

#ifndef BH_CONFIG_CONFIG_ENTRIES_H_
#define BH_CONFIG_CONFIG_ENTRIES_H_

#include <windows.h>

#include <string>
#include <unordered_map>

struct ConfigEntries {
  // Stats display
  KeyPress character_stats_key;
  KeyToggle stats_on_right_toggle;
  std::unordered_map<std::string, std::string> stat_screen_entries;

  // Bnet
  bool is_autofill_last_game_enabled;
  bool is_autofill_last_password_enabled;
  bool is_autofill_next_game_enabled;
  bool is_autofill_description_enabled;
  int fail_to_join_timeout_millis;

  // Chat color
  std::unordered_map<std::string, int /* enum ColorCode */> whisper_colors;

  // Game filter
  int game_list_refresh_interval_millis;
  bool is_show_difficult_enabled;
  bool is_show_game_server_enabled;

  // Item
  ToggleKey show_ethereal_toggle;
  ToggleKey show_sockets_toggle;
  ToggleKey show_item_level_toggle;
  ToggleKey show_rune_numbers_toggle;
  ToggleKey alt_item_style_toggle;
  ToggleKey color_modifier_toggle;
  ToggleKey shorten_item_names_toggle;
  ToggleKey always_show_items_toggle;
  ToggleKey advanced_item_display_toggle;
  ToggleKey advanced_item_display_toggle;
  ToggleKey item_drop_notifications_toggle;
  ToggleKey item_close_notifications_toggle;
  ToggleKey item_detailed_notifications_toggle;
  ToggleKey allow_unknown_items_toggle;
  ToggleKey verbose_notifications_toggle;
  ToggleKey suppress_invalid_stats_toggle;
  ToggleKey always_show_item_stat_ranges_toggle;
  int filter_level;
  int ping_level;
  int run_details_ping_level;
  int stat_range_color;
};

#endif /* BH_CONFIG_CONFIG_ENTRIES_H_ */
