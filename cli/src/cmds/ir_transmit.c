/**
 * COPYRIGHT 2019 Flirc, Inc. All rights reserved.
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

#include <flirc/flirc.h>
#include <cmds.h>
#include <logging.h>

static inline int
enough_args(int arguments, int amount_expected)
{
	if (!((arguments) >= amount_expected)) {
		logerror("Not Enough Arguments\n");
		return -1;
	}

	return 0;
}

CMDHANDLER(sendir)
{
	const char *line;
	ssize_t len;
	uint16_t buf[100];
	int i = 0;
	int buf_size = 0;

	int repeat = 0;
	int ik_delay = 0;
	int rq;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help sendir", NULL);
		return argc;
	}

	if ((len = strlen(argv[0])) < 3) {
		logerror("invalid length\n");
		run_cmd_line("help sendir", NULL);
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

	if (dict_has_key(opts, "ik")) {
		const char *val = dict_str_for_key(opts, "ik");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("must specify an interkey delay >= 15000\n");
			return -1;
		}
		ik_delay = (uint32_t)strtol(val, NULL, 10);
	}

	if (dict_has_key(opts, "repeat")) {
		const char *val = dict_str_for_key(opts, "repeat");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("must specify a repeat count\n");
			return -1;
		}
		repeat = (uint32_t)strtol(val, NULL, 10);
	}

	rq = fl_transmit_raw(buf, buf_size, ik_delay, repeat);

	if (rq == -EWRONGDEV) {
		logerror("invalid firmware, please upgrade to v4.2.+\n");
		return argc;
	} else if (rq < 0) {
		logerror("error sending IR pattern\n");
		return argc;
	}

	return 1;
}

START_CMD_OPTS(sendir_opts)
	CMD_OPT(ik, 'i', "ik", "set the interkey delay between rep. frames")
	CMD_OPT(repeat, 'r', "repeat", "number of times to repeat pattern")
END_CMD_OPTS;

APPCMD_OPT(sendir, &sendir,
		"Send a packet over the IR transmitter",
		"usage: ir_send_raw [packet]\n"
		" packet     0,153,1231,131"
		" 	     comma delimetd list, no spaces, must lead /w 0,",
		NULL, sendir_opts);
