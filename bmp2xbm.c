/*
 * 1BPP BMP to XBM converter.
 * Public domain.
 */
/*
 * BUGS:
 * Can't load padded bitmaps. Can't save non-multitude-of-8 xbms: 
 * Because a single bit represents each pixel (black or white), each byte in the array contains the information for eight pixels, with the upper left pixel in the bitmap represented by the low bit of the first byte in the array. If the image width does not match a multiple of 8, the display mechanism ignores and discards the extra bits in the last byte of each row.
 */
#include <stdio.h>
#include <string.h>
#include <endian.h>
#include <stdint.h>
#include <malloc.h>

char *img;

uint16_t width,
		 height,
		 offset,
		 bpp;

void show_usage(const char *name) {
	fprintf(stdout, "Usage:\t%s INPUT-FILE [OUTPUT-FILE]\n", name);
}

int load_bitmap(const char *filename) {

#define BMP_HEADER_SIZE	54

	FILE *f;

	char header[BMP_HEADER_SIZE];
	char buffer[1024];

	int n;
	int x, y, pad;

	f = fopen(filename, "rb");

	if (!f) {
		fprintf(stderr, "Can't open '%s'\n", filename);
		return 1;
	}

	n = fread(header, sizeof(char), BMP_HEADER_SIZE, f);

	if (n < BMP_HEADER_SIZE || /* First 2 bytes are 0x424d */
		header[0] != 'B' || header[1] != 'M') {
			fprintf(stderr, "Invalid header in '%s'\n", filename);
			fclose(f);
			return 2; /* Invalid bitmap */
		}

	width = ((header[18+0] << 0) & 0xFF) |
			((header[18+1] << 8) & 0xFF) ;

	height =((header[22+0] << 0) & 0xFF) |
			((header[22+1] << 8) & 0xFF) ;

	offset =((header[10+0] << 0) & 0xFF) |
			((header[10+1] << 8) & 0xFF) ;

	bpp =   ((header[28+0] << 0) & 0xFF) |
			((header[28+1] << 8) & 0xFF) ;

	width = le16toh(width);
	height = le16toh(height);
	offset = le16toh(offset);
	bpp = le16toh(bpp);

	if (bpp != 1) {
		fprintf(stderr, "Not a monochrome bitmap '%s'\n", filename);
		fclose(f);
		return 3; /* Wrong kind of bitmap */
	}

	fseek(f, offset, SEEK_SET);

	img = malloc(sizeof(char) * width * height);

	x = 0;	
	y = 1;
	pad = 0;
	while ((n = fread(buffer, sizeof(char), 1024, f))) {
		int i;
		for (i = 0; i < n; i++) {
			int j;
			for (j = 0; j < 8; j++) {
				char nibble = ( buffer[i] & (0x1 << j) );
				img[(height - y) * width + (x + j)] = nibble;
			}
			x += 8;
			if (x >= width) {
				i += pad;
				x = 0;
				y++;
			}

		}
	}

	fclose(f);
	return 0;
}

int save_xbm(const char *filename) {

	const char name[] = "inline_font";

	int n, bit, x, y;

	FILE *f;

	f = fopen(filename, "wb");

	if (!f) return 1;

	fprintf(f, "#define %s_width %d\n", name, width); 
	fprintf(f, "#define %s_height %d\n", name, height); 
	fprintf(f, "static unsigned char %s_bits[] = {\n", name); 

	char t = 0; bit = 0;
	for (y = 0; y < height; y++)
	for (x = 0; x < width;  x++) {

			if (!img[y * width + x]) 
				t |= (0x01 << (7-bit));
			bit++;
			if (bit > 7) {
				fprintf(f, "0x%02hhx, ", t);
				bit = 0;
				t = 0;
				n++;
				if (n > 12) {
					fprintf(f, "\n");
					n = 0;
				}
			}
	}

	fseek(f, -2, SEEK_CUR);
	fprintf(f, "  };\n");
	return 0;
}

int main(int argc, char *argv[]) {

	char output[1024];

	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	if (argc > 2) {
		strcpy(output, argv[2]);
	} else {
		strcpy(output, "inline_font.h");
	}

	if (load_bitmap(argv[1])) {
		fprintf(stderr, "Unable to read file %s\n", argv[1]);
		return -1;
	}

	if (save_xbm(output)) {
		fprintf(stderr, "Unable to write file %s\n", output);
		return -2;
	}

	return 0;
}
