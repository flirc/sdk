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
 * cmds.c - Modular Command Support.
 */

#include <cmds.h>
#include <prjutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef CMDS_MAX_ARGUMENTS
#define CMDS_MAX_ARGUMENTS	64
#endif /* CMDS_MAX_ARGUMENTS */

LIST_HEAD(registered_cmds);

static const struct cmd *lookup_cmd(const char *name,
		struct list_head *cmd_list)
{
	struct cmd_mgr *pos;

	list_for_each_entry(pos, cmd_list, node) {
		if(strcmp(name, pos->cmd->name) == 0)
			return pos->cmd;
	}

	return NULL;
}

static int tokenize_cmd_string(char *argstr, char **argv, size_t max_args)
{
	size_t argc, i;
	size_t slen = strlen(argstr);
	int esc, start;

	for(i = 0, argc = 0, esc = 0, start = 1;
			argstr[i] && argc < (max_args - 1); i++) {
		switch(argstr[i]) {
		case ' ':
		case '\t':
			if(!esc) {
				argstr[i] = '\0';
				start = 1;
			} else {
				/* Don't tokenize */
				memmove(&argstr[i - 1], &argstr[i],
						slen + 1 - i);
				slen--;
				i--;
				esc = 0;
			}
			break;

		case '\\':
			if(!esc) {
				esc = 1;
			} else {
				/* Double backslash */
				memmove(&argstr[i - 1], &argstr[i],
						slen + 1 - i);
				slen--;
				i--;
				esc = 0;
			}
		default:
			if(esc && argstr[i] != '\\') {
				logwarn("invalid escape character: %c\n",
						argstr[i]);
				esc = 0;
			}
			if(start) {
				start = 0;
				argv[argc++] = &argstr[i];
			}
			break;
		}
	}

	argv[argc] = NULL;

	return (int)argc;
}

static const struct cmd_opt *findlongopt(const struct cmd *cmd_entry,
		const char *longopt)
{
	const struct cmd_opt *o;

	if(cmd_entry->options == NULL)
		return NULL;

	for(o = cmd_entry->options; o->name != NULL; o++) {
		if(o->longopt && (strcmp(o->longopt, &longopt[2]) == 0))
			return o;
	}

	return NULL;
}

static const struct cmd_opt *findshortopt(const struct cmd *cmd_entry,
		char shortopt)
{
	const struct cmd_opt *o;

	if(cmd_entry->options == NULL)
		return NULL;

	for(o = cmd_entry->options; o->name != NULL; o++) {
		if(o->shortopt == shortopt)
			return o;
	}

	return NULL;
}

static inline void addopt(const struct cmd_opt *opt, struct dictionary *optdict,
		const char *val)
{
	size_t val_len = val ? (strlen(val) + 1) : 0;
	dict_add_key(optdict, opt->name, val, val_len);
}

static void handle_longopt(const struct cmd *cmd_entry, struct dictionary *od,
		int argc, const char **argv, unsigned int *carg)
{
	const struct cmd_opt *opt;
	char *optstr = strdup(argv[*carg]);
	char *val = NULL;
	int i;

	if(optstr == NULL) {
		logerror("%s\n", strerror(errno));
		return;
	}

	/* Look for an = sign to separate opt from value */
	for(i = 0; i < strlen(optstr); i++) {
		if(optstr[i] == '=') {
			optstr[i] = '\0';
			val = &optstr[i + 1];
			break;
		}
	}

	if((opt = findlongopt(cmd_entry, optstr)) == NULL) {
		logwarn("`%s' doesn not take '%s' option\n", cmd_entry->name,
				optstr);
	} else {
		addopt(opt, od, val);
	}

	free(optstr);
}

static void handle_shortopt(const struct cmd *cmd_entry, struct dictionary *od,
		int argc, const char **argv, unsigned int *carg)
{
	const struct cmd_opt *opt;
	int i;

	for(i = 1; i < strlen(argv[*carg]); i++) {
		if((opt = findshortopt(cmd_entry, argv[*carg][i])) == NULL) {
			logwarn("`%s' doesn not take '-%c' option\n",
					cmd_entry->name, argv[*carg][i]);
		} else {
			addopt(opt, od, NULL);
		}
	}
}

static struct dictionary *cmd_getopt(const struct cmd *cmd_entry,
		int argc, const char **argv, unsigned int *carg)
{
	struct dictionary *od;

	if((od = new_dict()) == NULL) {
		return NULL;
	}

	for(;(*carg < argc) && argv[*carg][0] == '-'; (*carg)++) {
		if(strlen(argv[*carg]) < 2) {
			/* A single - is most likely an argument denoting
			 * 'stdin'. We should stop processing opts.
			 */
			logverbose("single -, stopping\n");
			break;
		}
		logverbose("opt: %s, carg = %d\n", argv[*carg], *carg);
		if(argv[*carg][1] == '-') {
			/* Handle Long Opt */
			handle_longopt(cmd_entry, od, argc, argv, carg);
		} else {
			/* Handle Short Opts */
			handle_shortopt(cmd_entry, od, argc, argv, carg);
		}
	}

	return od;
}

int run_cmds(int argc, const char **argv, void *appdata)
{
	unsigned int carg = 0;
	int ret = 0;
	struct dictionary *opts;

	/* loop over all argv entries */
	while(carg < argc) {
		const struct cmd *cmd_entry =
			lookup_cmd(argv[carg++], &registered_cmds);
		if(cmd_entry == NULL) {
			logerror("could not find command '%s'\n",
					argv[carg-1]);
			return -ENOSYS;
		}
		logverbose("running command: %s\n", cmd_entry->name);

		if((opts = cmd_getopt(cmd_entry, argc, argv, &carg)) == NULL) {
			logerror("could not create option dictionary\n");
			return -ENOMEM;
		}

		/* call the command handler */
		if((ret = (cmd_entry->handler)(argc-carg, &argv[carg],
						cmd_entry, appdata, opts)) < 0)
			return carg;
		carg += ret;
		delete_dict(opts);
	}

	return 0;
}

int run_cmd(const char *name, int argc, const char **argv, void *appdata)
{
	struct dictionary *opts;
	unsigned int carg = 0;
	const struct cmd *cmd_entry = lookup_cmd(name, &registered_cmds);
	if(cmd_entry == NULL) {
		logerror("could not find command '%s'\n", name);
		return -ENOSYS;
	}

	logverbose("running command: %s\n", cmd_entry->name);

	if((opts = cmd_getopt(cmd_entry, argc, argv, &carg)) == NULL) {
		logerror("could not create option dictionary\n");
		return -ENOMEM;
	}

	/* call the command handler */
	if((cmd_entry->handler)(argc-carg, &argv[carg], cmd_entry, appdata,
				opts) < 0) {
		delete_dict(opts);
		return 1;
	}
	delete_dict(opts);

	return 0;
}

int run_cmd_line(const char *cmd_line, void *appdata)
{
	int status = 0;
	char *s;
	int argc;
	char **argv;

	if((s = malloc(strlen(cmd_line) + 1)) == NULL) {
		logerror("could not allocate command line buffer");
		status = -ENOMEM;
		goto exit1;
	}
	if((argv = malloc(CMDS_MAX_ARGUMENTS * sizeof(argv[0]))) == NULL) {
		logerror("could not allocate argv");
		status = -ENOMEM;
		goto exit2;
	}
	strcpy(s, cmd_line);

	argc = tokenize_cmd_string(s, argv, CMDS_MAX_ARGUMENTS);

	if(argc) {
		status = run_cmd(argv[0], argc - 1, (const char **)&argv[1],
				appdata);
	} else {
		logerror("command line empty\n");
		status = -EINVAL;
	}

	free(argv);
exit2:
	free(s);
exit1:
	return status;
}

void _register_cmd(struct cmd_mgr *rcmd)
{
	struct cmd_mgr *pos;

	/* add command in alphabetical order */
	list_for_each_entry(pos, &registered_cmds, node) {
		if(strcmp(rcmd->cmd->name, pos->cmd->name) <= 0) {
			list_add_tail(&rcmd->node, &pos->node);
			return;
		}
	}
	list_add_tail(&rcmd->node, &registered_cmds);
}

CMDHANDLER(help)
{
	if(argc > 0) {
		const struct cmd_opt *o;
		const struct cmd *cmd_entry = lookup_cmd(argv[0],
				&registered_cmds);
		if(cmd_entry == NULL) {
			pcmderr("command `%s' not found.\n", argv[0]);
			return -1;
		}
		printf("Help for `%s' command:\n", cmd_entry->name);
		printf("%s\n", cmd_entry->help);
		if(cmd_entry->options) {
		printf("OPTIONS:\n");
			for(o = cmd_entry->options; o->name != NULL; o++) {
				printf("  ");
				if(o->shortopt && o->longopt)
					printf("-%c, --%-13s",
							o->shortopt,
							o->longopt);
				else if(o->shortopt)
					printf("-%c                 ",
							o->shortopt);
				else if(o->longopt)
					printf("--%-17s", o->longopt);
				printf("  %s\n", o->description);
			}
		}

	} else {
		struct cmd_mgr *pos;
		int j;

		printf("Commands:\n");
		list_for_each_entry(pos, &registered_cmds, node) {
			printf("  %s ", pos->cmd->name);
			for(j = 0; j < (int)(14 - strlen(pos->cmd->name));
					j++)
				printf(" ");
			printf("%s\n", pos->cmd->summary);
		}

	}
	return argc;
}
APPCMD(help, &help, "Show this help. Also try `help <command>`",
	"usage: help [command]\n"
	"  `help' with no arguments will show a list of all commands.\n"
	"  `help <command>' will show a detailed help for `command'",
	NULL);
