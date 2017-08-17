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
 * cmds.h - Modular Command Support.
 *
 * DESCRIPTION: This header file describes data structures and functions for a
 * modular command system.
 */

#include <prjutil.h>
#include <logging.h>
#include <ll.h>
#include <dict.h>

#ifndef I__CMDS_H__
	#define I__CMDS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Option Data Structure */
struct cmd_opt {
	const char *name;	/* Name of opt. This will be dictionary key. */
	const char *description;
	char shortopt;		/* \0 for none */
	const char *longopt;	/* NULL for none */
};

#define START_CMD_OPTS(name)		static const struct cmd_opt name[] = {
#define CMD_OPT(name, shorto, longo, desc)	{#name, desc, shorto, longo},
#define END_CMD_OPTS			{NULL, NULL, 0, NULL}}

/* Command Data Structure */
struct cmd {
	/* The name of the command */
	const char *name;

	/* The handler function for the command.
	 * 	int argc	- number of arguments passed to handler
	 * 	char **argv	- arguments passed to handler
	 * 	struct cmd *cmd	- pointer to command structure
	 * 	void *appdata	- optional data passed by the the application
	 */
	int (*handler)(int argc, const char **argv, const struct cmd *cmd,
			void *appdata, struct dictionary *opts);

	/* A _short_ summary of the command. shown with list of commands */
	const char *summary;

	/* Text shown when help for a command is requested. */
	const char *help;

	/* pointer to private data structure for command */
	void *priv;

	/* pointer to option list */
	const struct cmd_opt *options;
};

struct cmd_mgr {
	struct list_head node;
	const struct cmd *cmd;
};

/* Add a Command to Section - use this for defining commands */
#define APPCMD(name,handler,summary,help,priv) \
		_APPCMD(name,handler,summary,help,priv,NULL)
#define APPCMD_OPT(name,handler,summary,help,priv,opt) \
		_APPCMD(name,handler,summary,help,priv,opt)
#define _APPCMD(name,handler,summary,help,priv,opt) \
	static const struct cmd cmd_entry_ ## name = { \
		#name, \
		handler, \
		summary, \
		help, \
		priv, \
		opt }; \
	static void __constructor REGFUNC__ ## name(void) { \
		static struct cmd_mgr c; \
		c.cmd = &cmd_entry_ ## name; \
		_register_cmd(&c); \
	}


/* Command handler prototype
 * 	RETURN negative for error, otherwise number of arguments used
 */
#define CMDHANDLER(name) \
		static int name(int argc, const char **argv, \
				const struct cmd *cmd, void *appdata, \
				struct dictionary *opts)

#define THISCMD cmd->name
/* Standard error print */
#define pcmderr(fmt, args...) logerror("%s: " fmt, THISCMD, ## args)


/* Run Command[s] Error Handling
 * 	run_cmd[s] will return 0 on success. A positive error code, n, means
 * 	that command number n encountered an error. A negative error code
 * 	means that the cmds framework encountered an error.
 * 		ENOSYS		- could not find command
 * 		ENOMEM		- could not allocate enough memory
 * 		EINVAL		- invalid argument
 * 	Note that 'command n' actually means argv[n].
 */

/* FUNCTION:    run_cmds
 *
 * + DESCRIPTION:
 *   - parses and runs a mixed argument list of commands and parameters
 *
 * + PARAMETERS:
 *   + int argc
 *     - number of arguments
 *   + const char **argv
 *     - argument list
 *   + void *appdata
 *     - appdata pointer to pass to each command run
 *
 * + RETURNS: int
 *   - see Error Handling above
 */
int run_cmds(int argc, const char **argv, void *appdata);

/* FUNCTION:    run_cmd
 *
 * + DESCRIPTION:
 *   - runs a single command with arguments
 *
 * + PARAMETERS:
 *   + const char *name
 *     - name of command
 *   + int argc
 *     - number of arguments
 *   + const char **argv
 *     - argument list
 *   + void *appdata
 *     - appdata pointer to pass to command
 *
 * + RETURNS: int
 *   - see Error Handling above
 */
int run_cmd(const char *name, int argc, const char **argv, void *appdata);

/* FUNCTION:    run_cmd_line
 *
 * + DESCRIPTION:
 *   - parse a string as a command line
 *
 * + PARAMETERS:
 *   + const char *cmd_line
 *     - command line string
 *   + void *appdata
 *     - appdata to pass to command
 *
 * + RETURNS: int
 *   - see Error Handling above
 */
int run_cmd_line(const char *cmd_line, void *appdata);


/* FUNCTION:    _register_cmd
 *
 * + DESCRIPTION:
 *   - adds a command to the command list. this is a low-level function that
 *   users should normally not need to call.
 *
 * + PARAMETERS:
 *   + struct cmd_mgr *rcmd
 *     - command structure to register
 */
void _register_cmd(struct cmd_mgr *rcmd);

#ifdef __cplusplus
}
#endif
#endif /* I__CMDS_H__ */
