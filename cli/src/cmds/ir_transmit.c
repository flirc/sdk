/**
 * COPYRIGHT 2012 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 *
 * @file
 * @brief   Flirc command file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>

#include <ll.h>
#include <flirc.h>
#include <prjutil.h>
#include <cmds.h>
#include <logging.h>
#include <timelib.h>

static inline int
enough_args(int arguments, int amount_expected)
{
	if (!((arguments) >= amount_expected)) {
		logerror("Not Enough Arguments\n");
		return -1;
	}

	return 0;
}

CMDHANDLER(send_ir_raw)
{
	const char *line;
	ssize_t len;
	uint16_t buf[100];
	int i = 0;
	int buf_size = 0;
	int rq;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help send_ir_raw", NULL);
		return argc;
	}

	if ((len = strlen(argv[0])) < 3) {
		logerror("invalid length\n");
		run_cmd_line("help send_ir_raw", NULL);
	}

	line = argv[0];

	while (len > 0) {
		buf[buf_size++] = strtol(&line[i], NULL, 10);
		while (line[i++] != ',' && len--);
	}

	if (IS_ODD(buf_size)) {
		logerror("invalid length, must be an even number of edges\n");
		return -1;
	}

	if (buf[0] != 0) {
		logerror("buffer incorrect\n");
		return -1;
	}

	rq = fl_transmit_raw(buf, buf_size);
	if (rq == -EWRONGDEV) {
		logerror("invalid firmware, please upgrade to v4.2.+\n");
		return argc;
	} else if (rq < 0) {
		logerror("error sending IR pattern\n");
		return argc;
	}
	return 1;
}

APPCMD(send_ir_raw, &send_ir_raw,
		"Send a packet over the IR transmitter",
		"usage: ir_send_raw [packet]\n"
		" packet	0,153,1231,131"
		" 		comma delimetd list, no spaces, must lead /w 0,",
		NULL);
