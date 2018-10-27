//
//  output.c
//  AnsiLove/C
//
//  Copyright (c) 2011-2018 Stefan Vogt, Brian Cassidy, and Frederic Cambus.
//  All rights reserved.
//
//  This source code is licensed under the BSD 2-Clause License.
//  See the LICENSE file for details.
//

#include <gd.h>
#include "ansilove.h"
#include "output.h"

int output(struct ansilove_ctx *ctx, struct ansilove_options *options, gdImagePtr im_Source) {
	// XXX Error handling
	// XXX The caller must invoke gdFree()
	// XXX Allow combining DOS aspect ratio and Retina

	if (!options->scale_factor && !options->dos) {
		ctx->png.buffer = gdImagePngPtr(im_Source, &ctx->png.length);
	} else if (options->dos) {
		gdImagePtr im_DOS;

		im_DOS = gdImageCreateTrueColor(im_Source->sx,
		    im_Source->sy * 1.35);

		if (!im_DOS) {
			ctx->error = ANSILOVE_GD_ERROR;
			return -1;
		}

		gdImageCopyResampled(im_DOS, im_Source, 0, 0, 0, 0,
		    im_DOS->sx, im_DOS->sy, im_Source->sx, im_Source->sy);

		gdImageDestroy(im_Source);

		ctx->png.buffer = gdImagePngPtr(im_DOS, &ctx->png.length);
	} else {
		gdImagePtr im_Retina;

		// make the Retina image scale_factor as large as im_Source
		im_Retina = gdImageCreate(im_Source->sx * options->scale_factor,
		    im_Source->sy * options->scale_factor);

		if (!im_Retina) {
			ctx->error = ANSILOVE_GD_ERROR;
			return -1;
		}

		gdImageCopyResized(im_Retina, im_Source, 0, 0, 0, 0,
		    im_Retina->sx, im_Retina->sy, im_Source->sx, im_Source->sy);

		// create retina output image
		gdImageDestroy(im_Source);
		ctx->png.buffer = gdImagePngPtr(im_Retina, &ctx->png.length);
	}

	return 0;
}
