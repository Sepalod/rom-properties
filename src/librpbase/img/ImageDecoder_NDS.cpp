/***************************************************************************
 * ROM Properties Page shell extension. (librpbase)                        *
 * ImageDecoder_GCN.cpp: Image decoding functions. (GameCube)              *
 *                                                                         *
 * Copyright (c) 2016-2017 by David Korth.                                 *
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

#include "ImageDecoder.hpp"
#include "ImageDecoder_p.hpp"

namespace LibRpBase {

/**
 * Convert a Nintendo DS CI4 image to rp_image.
 * @param width Image width.
 * @param height Image height.
 * @param img_buf CI4 image buffer.
 * @param img_siz Size of image data. [must be >= (w*h)/2]
 * @param pal_buf Palette buffer.
 * @param pal_siz Size of palette data. [must be >= 16*2]
 * @return rp_image, or nullptr on error.
 */
rp_image *ImageDecoder::fromNDS_CI4(int width, int height,
	const uint8_t *img_buf, int img_siz,
	const uint16_t *pal_buf, int pal_siz)
{
	// Verify parameters.
	if (!img_buf || !pal_buf)
		return nullptr;
	else if (width < 0 || height < 0)
		return nullptr;
	else if (img_siz < ((width * height) / 2) || pal_siz < 0x20)
		return nullptr;

	// NDS CI4 uses 8x8 tiles.
	if (width % 8 != 0 || height % 8 != 0)
		return nullptr;

	// Calculate the total number of tiles.
	const int tilesX = (width / 8);
	const int tilesY = (height / 8);

	// Create an rp_image.
	rp_image *img = new rp_image(width, height, rp_image::FORMAT_CI8);

	// Convert the palette.
	// TODO: Optimize using pointers instead of indexes?
	uint32_t *palette = img->palette();
	assert(img->palette_len() >= 16);
	if (img->palette_len() < 16) {
		// Not enough colors...
		delete img;
		return nullptr;
	}

	palette[0] = 0; // Color 0 is always transparent.
	img->set_tr_idx(0);
	for (int i = 1; i < 16; i++) {
		// NDS color format is BGR555.
		palette[i] = ImageDecoderPrivate::BGR555_to_ARGB32(le16_to_cpu(pal_buf[i]));
	}

	// NOTE: rp_image initializes the palette to 0,
	// so we don't need to clear the remaining colors.

	for (int y = 0; y < tilesY; y++) {
		for (int x = 0; x < tilesX; x++) {
			// Blit the tile to the main image buffer.
			ImageDecoderPrivate::BlitTile_CI4_LeftLSN<8, 8>(img, img_buf, x, y);
			img_buf += ((8 * 8) / 2);
		}
	}

	// Image has been converted.
	return img;
}

}
