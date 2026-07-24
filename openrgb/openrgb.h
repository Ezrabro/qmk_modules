/* OpenRGB QMK Protocol - Revision D - Community Module
 * Copyright 2020 Kasper
 * Copyright 2021 Neneya
 * Copyright 2025 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2026 Adam Honse <calcprogrammer1@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// This is changed only when the command IDs change,
// so OpenRGB can detect compatible firmware.
#define OPENRGB_PROTOCOL_VERSION 0x0D

// OpenRGB protocol command IDs
enum openrgb_command_id {
    OPENRGB_GET_PROTOCOL_VERSION        = 1,
    OPENRGB_GET_QMK_VERSION             = 2,
    OPENRGB_GET_DEVICE_INFO             = 3,
    OPENRGB_GET_MODE_INFO               = 4,
    OPENRGB_GET_LED_INFO                = 5,
    OPENRGB_GET_ENABLED_MODES           = 6,

    OPENRGB_SET_MODE                    = 7,
    OPENRGB_DIRECT_MODE_SET_SINGLE_LED  = 8,
    OPENRGB_DIRECT_MODE_SET_LEDS        = 9,
};

// OpenRGB protocol response codes
enum openrgb_responses {
    OPENRGB_FAILURE                     = 25,
    OPENRGB_SUCCESS                     = 50,
    OPENRGB_END_OF_MESSAGE              = 100,
};

// Direct mode color buffer
extern RGB g_openrgb_direct_mode_colors[RGB_MATRIX_LED_COUNT];

void openrgb_get_protocol_version(void);
void openrgb_get_qmk_version(void);
void openrgb_get_device_info(void);
void openrgb_get_mode_info(void);
void openrgb_get_led_info(uint8_t *data);
void openrgb_get_enabled_modes(void);

void openrgb_set_mode(uint8_t *data);
void openrgb_direct_mode_set_single_led(uint8_t *data);
void openrgb_direct_mode_set_leds(uint8_t *data);
