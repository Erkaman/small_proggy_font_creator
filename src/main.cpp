#include <stdio.h>

#include "lodepng.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define PRETTY_PRINT false // if true, pretty-print json with newlines.
#define START_CHAR 32
#define NUM_CHARS 95
#define BITMAP_WIDTH 256
#define BITMAP_HEIGHT 256
#define FONT_BITMAP_FILE_NAME "../font_bitmap.png"
#define TTF_FILE_NAME "../proggy_clean.ttf"
#define FONT_INFO_FILE_NAME "../font_info.json"
#define OVERSAMPLE_W 1
#define OVERSAMPLE_H 1

stbtt_packedchar char_desc_list[NUM_CHARS];
unsigned char temp_bitmap[BITMAP_WIDTH][BITMAP_HEIGHT];
unsigned char ttf_buffer[1 << 25];

float font_scale = 13.0f;

int main() {



    stbtt_pack_context pc;
    FILE *f;
    char filename[256];
    stbtt_packedchar c;

    /*
      Open and read TTF
     */
    f = fopen(TTF_FILE_NAME, "rb");
    if(f == NULL) {
	printf("Could not open %s for reading!", TTF_FILE_NAME);
	exit(1);
    }
    fread(ttf_buffer, 1, 1<<25, f);


    /*
      Pack font bitmap.
     */
    stbtt_PackBegin(&pc, temp_bitmap[0], BITMAP_WIDTH, BITMAP_HEIGHT, 0, 1, NULL);
    stbtt_PackSetOversampling(&pc, OVERSAMPLE_W, OVERSAMPLE_H);// 2x2 oversampling.
    stbtt_PackFontRange(&pc, ttf_buffer, 0, font_scale, START_CHAR, NUM_CHARS, char_desc_list);
    stbtt_PackEnd(&pc);

    // make most of the lower image white.
    for(int i = BITMAP_HEIGHT - 200; i < BITMAP_HEIGHT; ++i) {
	for(int j = 0; j < BITMAP_WIDTH; ++j) {
	    temp_bitmap[i][j] = 255;
	}
    }


    /*
      Save font bitmap to png.
     */
    unsigned error = lodepng_encode_file(FONT_BITMAP_FILE_NAME, temp_bitmap[0], BITMAP_WIDTH, BITMAP_HEIGHT, LCT_GREY, 8);
    if(error)  {
	printf("Failed to save bitmap to file %s: %s\n",  FONT_BITMAP_FILE_NAME,  lodepng_error_text(error) );
	exit(1);
    }


    /*
      Finally, save all info in charDescList to a json file.
     */
    f = NULL;
    f = fopen(FONT_INFO_FILE_NAME, "w");
    if(f == NULL) {
	printf("Could not open %s for writing!", FONT_INFO_FILE_NAME);
	exit(1);
    }

    fprintf(f, "module.exports = {\"chars\":[");

    if(PRETTY_PRINT)
	fprintf(f, "\n");

    for(int iCh = 0; iCh < (NUM_CHARS); ++iCh  ) {
	c = char_desc_list[iCh];
	fprintf(f, "{" );

	fprintf(f, "\"x0\":%d,", c.x0 );
	fprintf(f, "\"y0\":%d,", c.y0 );
	fprintf(f, "\"x1\":%d,", c.x1 );
	fprintf(f, "\"y1\":%d,", c.y1 );

	fprintf(f, "\"xoff\":%f,", c.xoff );
	fprintf(f, "\"yoff\":%f,", c.yoff );
	fprintf(f, "\"xadvance\":%f,", c.xadvance );
	fprintf(f, "\"xoff2\":%f,", c.xoff2 );
	fprintf(f, "\"yoff2\":%f", c.yoff2 );

	fprintf(f, "}" );

	if(iCh != (NUM_CHARS-1) ) {
	    fprintf(f, ",");
	}

	if(PRETTY_PRINT)
	    fprintf(f, "\n");
    }

    fprintf(f, "]}");
}
