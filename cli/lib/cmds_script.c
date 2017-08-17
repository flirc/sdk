/*
 * Copyright 2011 Robert C. Curtis. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBERT C. CURTIS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ROBERT C. CURTIS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of Robert C. Curtis.
 */

/*
 * cmds_script.c - Script interface for cmds
 */

#include <cmds.h>
#include <getline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CMDS_SCRIPT_LINE_LENGTH
#define CMDS_SCRIPT_LINE_LENGTH		1024
#endif /* CMDS_SCRIPT_LINE_LENGTH */

CMDHANDLER(script_handler)
{
	int status = 1;
	char *line;
	ssize_t len;
	int line_num = 1;
	FILE *fp = stdin;

	/* check arguments */
	if(!argc) {
		pcmderr("no file specified\n");
		status = -1;
		goto exit1;
	}
	/* try and open script file */
	if(strcmp(argv[0], "-") != 0) {
		if((fp = fopen(argv[0], "r")) == NULL) {
			perror("could not open script file");
			status = -1;
			goto exit1;
		}
	}
	/* allocate line buffer */
	if((line = malloc(CMDS_SCRIPT_LINE_LENGTH)) == NULL) {
		perror("could not allocate line buffer");
		status = -1;
		goto exit2;
	}

	/* loop through lines */
	while((len = fgetline(fp, line, CMDS_SCRIPT_LINE_LENGTH)) >= 0) {
		if((len > 0) && (line[0] != '#')) {
			if(run_cmd_line(line, appdata) != 0) {
				pcmderr("an error occured with line #%d\n",
						line_num);
				status = -1;
				break;
			}
		}
		line_num++;
	}

	/* clean up and exit */
	free(line);
exit2:
	if(fp != stdin)
		fclose(fp);
exit1:
	return status;
}
APPCMD(script, &script_handler,
	"run a command script",
	"usage: script <file>\n"
	"  a - for <file> will read from stdin",
	NULL);
