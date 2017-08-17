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
 * cmds_shell.c - Shell interface for cmds
 */

#include <cmds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/* shell colors */
#define COLOR_NO	"\1\e[00;00m\2"
#define COLOR_BR 	"\1\e[00;01m\2"
#define COLOR_RD 	"\1\e[00;31m\2"
#define COLOR_GR 	"\1\e[00;32m\2"
#define COLOR_YL 	"\1\e[00;33m\2"
#define COLOR_BL 	"\1\e[00;34m\2"
#define COLOR_MG 	"\1\e[00;35m\2"
#define COLOR_CY 	"\1\e[00;36m\2"

/* shell prompt */
#ifndef CMDS_SHELL_PROMPT
#define CMDS_SHELL_PROMPT	__TARGET__ " $ "
#endif /* CMDS_SHELL_PROMPT */

CMDHANDLER(shell_handler)
{
	int status = 0;
	char *line;

	/* loop through lines */
	while((line = readline(CMDS_SHELL_PROMPT)) != NULL) {
		if(strlen(line)) {
			run_cmd_line(line, appdata);
			add_history(line);
		}
		free(line);
	}

	printf("\n");

	return status;
}
APPCMD(shell, &shell_handler,
	"enter a command shell",
	"usage: shell",
	NULL);
