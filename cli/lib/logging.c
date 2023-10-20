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

#include <logging.h>
#include <cmds.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef LOG_WITH_NSLOG
extern void _nslog_hook(const char *fmt, va_list ap);
#endif

static FILE *logfds[MAX_LOGLEVEL + 1];

static int loglevel = min_of(DEFAULT_LOGLEVEL, MAX_LOGLEVEL);


void set_log_level(int level)
{
	loglevel = min_of(level, MAX_LOGLEVEL);
}

void __log(int level, const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);

	if(level && (level <= loglevel)) {
#ifdef LOG_WITH_NSLOG
		_nslog_hook(fmt, argp);
#else
		vfprintf(logfds[level], fmt, argp);
#endif
	}

	va_end(argp);
}

/* We need an init constructor to initialize the file descriptor array with
 * non-constant values
 */
static void __constructor _init_logging(void)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(logfds); i++) {
		logfds[i] = stderr;
	}
}

#ifndef WITH_NO_CMDS
/* Some commands to handle changing logging behavior during run-time */
CMDHANDLER(setloglevel)
{
	int req_level;

	/* check arguments */
	if(!argc) {
		pcmderr("no level specified\n");
		return -1;
	}
	/* parse log level */
	req_level = atoi(argv[0]);
	if((req_level < 0) || (req_level > MAX_LOGLEVEL)) {
		pcmderr("invalid level specified. max: %d\n", MAX_LOGLEVEL);
		return -1;
	}

	loglevel = req_level;

	return 1;
}
APPCMD(loglevel, &setloglevel, "set the log level",
		"usage: loglevel <level>\n"
		"  0 - NONE\n"
		"  1 - ERROR\n"
		"  2 - WARNING\n"
		"  3 - INFO\n"
		"  4 - DEBUG\n"
		"  5 - VERBOSE",
		NULL);
#endif /* WITH_NO_CMDS */
