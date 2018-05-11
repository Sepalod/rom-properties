/***************************************************************************
 * ROM Properties Page shell extension. (libromdata)                       *
 * gcom_structs.h: Tiger game.com data structures.                         *
 *                                                                         *
 * Copyright (c) 2018 by David Korth.                                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

#ifndef __ROMPROPERTIES_LIBROMDATA_GCOM_STRUCTS_H__
#define __ROMPROPERTIES_LIBROMDATA_GCOM_STRUCTS_H__

#include "librpbase/common.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// Icon information.
// NOTE: Icons are 2bpp.
#define GCOM_ICON_BANK_W 256
#define GCOM_ICON_BANK_H 256
#define GCOM_ICON_BANK_SIZE ((GCOM_ICON_BANK_W * GCOM_ICON_BANK_H) / 4)
#define GCOM_ICON_W 64
#define GCOM_ICON_H 64

// NOTE: The official game.com emulator requires the header to be at 0x40000.
// Some ROMs have the header at 0, though.
#define GCOM_HEADER_ADDRESS 0x40000
#define GCOM_HEADER_ADDRESS_ALT 0

/**
 * Tiger game.com ROM header.
 *
 * All fields are in little-endian.
 * NOTE: Icon is rotated. (TODO: Figure out rotation.)
 *
 * NOTE: Strings are NOT null-terminated!
 */
#define GCOM_SYS_ID "TigerDMGC"
typedef struct PACKED _Gcom_RomHeader {
	uint8_t rom_size;		// [0x000] ROM size?
	uint8_t entry_point_bank;	// [0x001] Entry point: Bank number.
	uint16_t entry_point;		// [0x002] Entry point.
	uint8_t unknown1;		// [0x004] Unknown.
	char sys_id[9];			// [0x005] System identifier.

	struct {
		/**
		 * game.com ROM images are divided into 16 KB banks,
		 * each of which makes up a 2bpp 256x256 bitmap.
		 * The game's icon is specified by selecting a bank
		 * number and the icon's (x,y) coordinates.
		 *
		 * NOTE: Bitmaps are rotated 270 degrees and vertically flipped.
		 */

		uint8_t bank;	// [0x00E] Bank number. (16 KB; 256x256)
		uint8_t x;	// [0x00F] X coordinate within the bank.
		uint8_t y;	// [0x010] Y coordinate within the bank.
	} icon;

	char title[9];			// [0x011] Game title.
	uint16_t game_id;		// [0x01A] Game ID.
	uint8_t security_code;		// [0x01C] Security code.
	uint8_t padding[3];		// [0x01D] Padding.
} Gcom_RomHeader;
ASSERT_STRUCT(Gcom_RomHeader, 32);

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __ROMPROPERTIES_LIBROMDATA_GCOM_STRUCTS_H__ */