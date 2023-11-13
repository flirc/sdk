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

#include <stdio.h>
#include <prjutil.h>

#ifndef I__LOGGING_H__
	#define I__LOGGING_H__

#define LOGLEVEL_NONE		0
#define LOGLEVEL_ERROR		1
#define LOGLEVEL_WARNING	2
#define LOGLEVEL_INFO		3
#define LOGLEVEL_DEBUG		4
#define LOGLEVEL_VERBOSE	5

#ifndef MAX_LOGLEVEL
#define MAX_LOGLEVEL		LOGLEVEL_VERBOSE
#endif /* MAX_LOGLEVEL */

#ifndef DEFAULT_LOGLEVEL
#define DEFAULT_LOGLEVEL	LOGLEVEL_WARNING
#endif /* DEFAULT_LOGLEVEL */


/* LOGERROR */
#if MAX_LOGLEVEL >= LOGLEVEL_ERROR
#define logerror(fmt, args...)		__log(LOGLEVEL_ERROR, \
						"[E] %s %s(%d): " fmt, \
						__FILE__, \
						__FUNCTION__, __LINE__, ## args)

#define log_error(fmt, args...)		logerror(fmt, ## args)
#define log_err(fmt, args...)		logerror(fmt, ## args)
#else
#define logerror(fmt, args...)
#define log_error(fmt, args...)
#define log_err(fmt, args...)
#endif

/* LOGWARN */
#if MAX_LOGLEVEL >= LOGLEVEL_WARNING
#define logwarn(fmt, args...)		__log(LOGLEVEL_WARNING, \
						"[W] %s %s(%d): " fmt, \
						__FILE__, \
						__FUNCTION__, __LINE__, ## args)
#define log_warn(fmt, args...)		logwarn(fmt, ## args)
#else
#define logwarn(fmt, args...)
#define log_warn(fmt, args...)
#endif

/* LOGINFO */
#if MAX_LOGLEVEL >= LOGLEVEL_INFO
#define loginfo(fmt, args...)		__log(LOGLEVEL_INFO, \
						"[I] %s %s(%d): " fmt, \
						__FILE__, \
						__FUNCTION__, __LINE__, ## args)
#define log_info(fmt, args...)		loginfo(fmt, ## args)
#else
#define loginfo(fmt, args...)
#define log_info(fmt, args...)
#endif

/* LOGDEBUG */
#if MAX_LOGLEVEL >= LOGLEVEL_DEBUG
#define logdebug(fmt, args...)		__log(LOGLEVEL_DEBUG, \
						"[D] %s %s(%d): " fmt, \
						__FILE__, \
						__FUNCTION__, __LINE__, ## args)
#define log_debug(fmt, args...)		logdebug(fmt, ## args)
#else
#define logdebug(fmt, args...)
#define log_debug(fmt, args...)
#endif

/* LOGVERBOSE */
#if MAX_LOGLEVEL >= LOGLEVEL_VERBOSE
#define logverbose(fmt, args...)	__log(LOGLEVEL_VERBOSE, \
						"[V] %s %s(%d): " fmt, \
						__FILE__, \
						__FUNCTION__, __LINE__, ## args)
#define log_verbose(fmt, args...)	logverbose(fmt, ## args)
#else
#define logverbose(fmt, args...)
#define log_verbose(fmt, args...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __log(int level, const char *fmt, ...) __printf_check(2, 3);
void set_log_level(int level);

#ifdef __cplusplus
}
#endif

#endif /* I__LOGGING_H__ */
