/*
 * hexdump.c
 *
 * Copyright (C) 2010 Robert C. Curtis
 *
 * hexdump is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * hexdump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hexdump.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <hexdump.h>
#include <prjutil.h>

static void hexdump_line(FILE *outf, void *line, size_t size, size_t width)
{
	size_t i;

	for(i = 0; i < width; i++) {
		if(i < size) {
			fprintf(outf, "%02x ",
					*((unsigned char *)(line + i)) & 0xFF);
		} else {
			fprintf(outf, "   ");
		}
	}
	fprintf(outf, " ");
	for(i = 0; i < width; i++) {
		if(i < size) {
			char c = *((char *)(line + i));
			if((c > 31) && (c < 127)) {
				fprintf(outf, "%c", c);
			} else {
				fprintf(outf, ".");
			}
		} else {
			fprintf(outf, " ");
		}
	}
}

void hexdump(FILE *outf, void *buf, size_t size)
{
	size_t i;
	const char *addrfmt;

	if(size < (1 << 8)) {
		addrfmt = "%02x: ";
	} else if(size < (1 << 16)) {
		addrfmt = "%04x: ";
	} else {
		addrfmt = "%08x: ";
	}

	for(i = 0; i < size; i += HEXDUMP_WIDTH) {
		fprintf(outf, addrfmt, (unsigned int)i);
		hexdump_line(outf, buf + i, min_of(HEXDUMP_WIDTH, (size - i)),
				HEXDUMP_WIDTH);
		fprintf(outf, "\n");
	}
}
