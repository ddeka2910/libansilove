//
//  artworx.c
//  AnsiLove/C
//
//  Copyright (c) 2011-2018 Stefan Vogt, Brian Cassidy, and Frederic Cambus.
//  All rights reserved.
//
//  This source code is licensed under the BSD 2-Clause License.
//  See the LICENSE file for details.
//

#include "../ansilove.h"

int ansilove_artworx(struct input *inputFile, struct output *outputFile)
{
	// libgd image pointers
	gdImagePtr canvas;

	// create ADF instance
	canvas = gdImageCreate(640, (((inputFile->length - 192 - 4096 -1) / 2) / 80) * 16);

	// error output
	if (!canvas) {
		perror("Can't allocate buffer image memory");
		return -1;
	}

	// ADF color palette array
	uint32_t adf_colors[16] = { 0, 1, 2, 3, 4, 5, 20, 7, 56, 57, 58, 59, 60, 61, 62, 63 };

	uint32_t loop;
	uint32_t index;

	// process ADF palette
	for (loop = 0; loop < 16; loop++)
	{
		index = (adf_colors[loop] * 3) + 1;
		gdImageColorAllocate(canvas, (inputFile->buffer[index] << 2 | inputFile->buffer[index] >> 4),
		    (inputFile->buffer[index + 1] << 2 | inputFile->buffer[index + 1] >> 4),
		    (inputFile->buffer[index + 2] << 2 | inputFile->buffer[index + 2] >> 4));
	}

	gdImageColorAllocate(canvas, 0, 0, 0);

	// process ADF
	uint32_t column = 0, row = 0;
	uint32_t character, attribute, foreground, background;
	loop = 192 + 4096 + 1;

	while (loop < inputFile->length)
	{
		if (column == 80)
		{
			column = 0;
			row++;
		}

		character = inputFile->buffer[loop];
		attribute = inputFile->buffer[loop+1];

		background = (attribute & 240) >> 4;
		foreground = attribute & 15;

		drawchar(canvas, inputFile->buffer+193, 8, 16, column, row, background, foreground, character);

		column++;
		loop += 2;
	}

	// create output file
	output(canvas, outputFile->fileName, outputFile->retina, outputFile->retinaScaleFactor);

	return 0;
}
