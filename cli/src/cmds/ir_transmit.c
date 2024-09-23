/**
 * COPYRIGHT 2024 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmds.h>
#include <flirc/flirc.h>
#include <logging.h>
#include <timelib.h>

#ifndef IS_ODD
#define IS_ODD(x)			(x & 0x1)
#endif
#ifndef IS_EVEN
#define IS_EVEN(x)			(!(IS_ODD(x)))
#endif

static int sendRaw(uint16_t *data, int len, int ik, int repeats)
{
	uint16_t buf[100];
	int rq;
	int i;

	memcpy(&buf[1], &data[0], sizeof(uint16_t)*len);
	buf[0] = 0;
	len++;

	for (i = 1; i < len; i++) {
		if (i & 0x1)
			printf("+");
		else
			printf("-");
		printf("%i", buf[i]);

		if (i < len-1)
			printf(" ");
	}
	printf("\n");
	printf("-%d\n", ik);

	if ((rq = fl_transmit_raw(buf, len, ik, repeats)) < 0) {
		return rq;
	}

	return 0;
}

static void decode_pronto(const char *line, int repeats)
{
	uint16_t buf[100];
	ssize_t len = 0;
	int i = 0;

	if (line == NULL || !(strlen(line))) {
		logerror("invalid line buffer, len = %d\n", (int)strlen(line));
		return;
	}

	/* convert string to buf */
	while (i < strlen(line)) {
		buf[len++] = strtol(&line[i], NULL, 16);
		while ((line[i] != ' ' && line[i] != ',' && i < strlen(line))) {
			i++;
		};
		i++;
	}

	flirc_send_pronto(buf, len, repeats);
}

static void decode_raw(const char *line, int ik, int repeats)
{
	uint16_t buf[100];
	ssize_t len = 0;
	int i = 0;

	if (line == NULL || !(strlen(line))) {
		log_err("invalid line buffer, len = %d\n", (int)strlen(line));
		return;
	}

	/* skip first character (+|-) */
	line++;

	/* convert string to buf */
	while (i < strlen(line)) {
		buf[len++] = strtol(&line[i], NULL, 10);
		while ((line[i] != ' ' && line[i] != ',' && i < strlen(line))) {
			i++;
		};
		i += 2;
	}

	if (sendRaw(buf, len, ik, repeats) < 0) {
		log_err("Error sending pattern\n");
	}
}

CMDHANDLER(sendir)
{
	uint16_t buf[100];

	int repeat = 1;
	int ik_delay = 15000;

#if 0
	/* make sure we are in a compatible version */
	if (fl_version_compare(APPDATA->dev, 4, 7) < 0) {
		log_err("Error: version >= 4.7.x required\n");
		return -1;
	}
#endif

	if (dict_has_key(opts, "ik")) {
		const char *val = dict_str_for_key(opts, "ik");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("interkey delay must be >= 15000 (us)\n");
			return -1;
		}
		ik_delay = (uint32_t)strtol(val, NULL, 10);
		if (ik_delay <= 15000 || ik_delay >= 65000) {
			logerror("interkey delay must be between 65000"
					"and 15000 us");
			return -1;
		}
	}

	if (dict_has_key(opts, "repeat")) {
		const char *val = dict_str_for_key(opts, "repeat");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("must specify a repeat count\n");
			return -1;
		}
		repeat = (uint32_t)strtol(val, NULL, 10);
		if (repeat > 10) {
			logerror("must specify a repeat <= 10\n");
			return -1;
		}
	}

	/* kill the existing pattern and did not specify a new one */
	if (dict_has_key(opts, "kill") && !dict_has_key(opts, "pattern")) {
#if 0
		/* make sure we are in a compatible version */
		if (fl_version_compare(APPDATA->dev, 4, 8) < 0) {
			log_err("Error: version >= 4.8.x required\n");
			return -1;
		}
#endif
		printf("Killing IR transmitter....");
		if (fl_ir_transmit_kill() < 0) {
			printf("Error\n");
		}
		printf("Done!\n");
	} else if (dict_has_key(opts, "raw")) {
		if (dict_has_key(opts, "kill")) {
			printf("Killing IR transmitter....");
			if (fl_ir_transmit_kill() < 0) {
				printf("Error\n");
			}
			printf("Done!\n");
		}

		const char *val = dict_str_for_key(opts, "raw");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("must specify a pattern\n");
			return argc;
		}
		printf("Transmitting IR Pattern...\n");
		decode_raw(val, ik_delay, repeat);
		printf("Done!\n");
		return 0;
	} else if (dict_has_key(opts, "pronto")) {
		if (dict_has_key(opts, "kill")) {
			printf("Killing IR transmitter....");
			if (fl_ir_transmit_kill() < 0) {
				printf("Error\n");
			}
			printf("Done!\n");
		}

		const char *val = dict_str_for_key(opts, "pronto");
		if ((val == NULL) || (strlen(val) == 0)) {
			logerror("must specify a pattern\n");
			return argc;
		}

		printf("Transmitting IR Pattern...\n");
		decode_pronto(val, repeat);
		printf("Done!\n");
		return 0;
	} else if (dict_has_key(opts, "csv")) {
		if (dict_has_key(opts, "kill")) {
			printf("Killing IR transmitter....");
			if (fl_ir_transmit_kill() < 0) {
				printf("Error\n");
			}
			printf("Done!\n");
		}

		const char *val = dict_str_for_key(opts, "csv");
		if ((val == NULL) || (strlen(val) == 0)) {
			log_err("must specify a pattern\n");
			return argc;
		}

		int buf_size = 0;
		char *token;
		char *rest = (char *)val;

		while ((token = strtok_r(rest, " ,", &rest)))
			buf[buf_size++] = strtol(token, NULL, 10);

		/* if we begin with zero */
		if (buf[0] == 0) {
			/* we must have odd number of entries */
			if (IS_ODD(buf_size)) {
				log_err("Error: invalid length, not even\n");
				return -1;
			}
		} else if (IS_EVEN(buf_size)) {
				log_err("Error: invalid length, not odd\n");
				return -1;
		}

		if (buf_size < 3) {
			log_err("Error: buffer can't be less than 4\n");
			return -1;
		}

		printf("Transmitting IR Pattern...");
		if (fl_transmit_raw(buf, buf_size, ik_delay, repeat) < 0) {
			log_err("Error: could not transmit data\n");
		}

		printf("Done!\n");
		return 0;
	} else {
		printf("Error: must specify an option\n");
		return -1;
	}

	return 0;
}

START_CMD_OPTS(sendir_opts)
	CMD_OPT(ik, 'i', "ik", "set the interkey delay between rep. frames")
	CMD_OPT(repeat, 'r', "repeat", "number of times to repeat pattern")
	CMD_OPT(kill, 'k', "kill", "kill any currently running transmission")
	CMD_OPT(pronto, 'p', "pronto", "send a pronto pattern")
	CMD_OPT(raw, 'x', "raw", "+8248 -1291 +212 ...")
	CMD_OPT(csv, 'c', "csv", "8248,1291,212,...or 0,8248...or 8248, 1291..")
END_CMD_OPTS;

APPCMD_OPT(sendir, &sendir,
		"Send a packet over the IR transmitter",
		"usage: sendir <opts>\n"
		"  sendir --pronto=\"0000,006D,0022,0002,0154,00A9,0014,0014,"
		"0014,0014,0014,0040,0014,0014,0014,0014,0014,0014,0014,0014,"
		"0014,0014,0014,0040,0014,0040,0014,0014,0014,0040,0014,0040,"
		"0014,0040,0014,0040,0014,0040,0014,0014,0014,0014,0014,0014,"
		"0014,0014,0014,0014,0014,0014,0014,0040,0014,0014,0014,0040,"
		"0014,0040,0014,0040,0014,0040,0014,0040,0014,0040,0014,0014,"
		"0014,0040,0014,0608,0154,0056,0014,0E4F\" --repeat=4\n"
		"  sendir --raw=\"+8840 -4394 +520 -520 +520 -520 +520 -1664 "
		"+520 -520 +520 -520 +520 -520 +520 -520 +520 -520 +520 -1664 "
		"+520 -1664 +520 -520 +520 -1664 +520 -1664 +520 -1664 +520 "
		"-1664 +520 -1664 +520 -520 +520 -520 +520 -520 +520 -520 +520 "
		"-520 +520 -520 +520 -1664 +520 -520 +520 -1664 +520 -1664 "
		"+520 -1664 +520 -1664 +520 -1664 +520 -1664 +520 -520 +520 "
		"-1664 +520\" --ik=23000 --repeat=10\n"
		"  sendir --csv=\"4472,4472,572,1664,572,1664,572,1664,572,"
		"546,572,546,572,546,572,546,572,546,572,1664,572,1664,572,1664"
		",572,546,572,546,572,546,572,546,572,546,572,1664,572,546,572,"
		"546,572,1664,572,1664,572,546,572,546,572,1664,572,546,572,"
		"1664,572,1664,572,546,572,546,572,1664,572,1664,572,"
		"546,572\"\n\n",
		NULL, sendir_opts);
