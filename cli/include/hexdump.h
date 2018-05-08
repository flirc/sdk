/*
 * hexdump.h
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

#include <stdio.h>
#include <sys/types.h>

#ifndef I__HEXDUMP_H__
	#define I__HEXDUMP_H__

#ifndef HEXDUMP_WIDTH
#define HEXDUMP_WIDTH	16
#endif /* HEXDUMP_WIDTH */

/* FUNCTION:    hexdump
 *
 * + DESCRIPTION:
 *   - prints out a pretty hex dump of binary data
 *
 * + PARAMETERS:
 *   + void *buf
 *     - buffer to dump
 *   + size_t size
 *     - size of buffer
 */
void hexdump(FILE *outf, void *buf, size_t size);

#endif /* I__HEXDUMP_H__ */
