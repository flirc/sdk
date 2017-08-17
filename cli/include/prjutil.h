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
 * prjutil.h - Commonly useful macros
 */

#ifndef I__PRJUTIL_H__
	#define I__PRJUTIL_H__

/*******************************************************************************
 * NULL
 * 	Global NULL definition
 */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
/******************************************************************************/

/*******************************************************************************
 * offsetof
 * 	The offset of member with structure of type
 *
 * type:	the structure type
 * member:	the name of the member within the struct.
 */
#ifndef offsetof
#define offsetof(TYPE, MEMBER)	__builtin_offsetof(TYPE, MEMBER)
#endif
/******************************************************************************/

/*******************************************************************************
 * container_of
 * 	Cast a member of a structure out to the containing structure
 *
 * ptr:		the pointer to the member.
 * type:	the type of the container struct this is embedded in.
 * member:	the name of the member within the struct.
 */
#ifndef container_of
#define container_of(ptr, type, member)	({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif
/******************************************************************************/

/*******************************************************************************
 * ARRAY_SIZE
 * 	Get the number of elements in an array
 *
 * array:	array variable
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array)	(sizeof((array))/sizeof((array)[0]))
#endif
/******************************************************************************/

/*******************************************************************************
 * max_of
 * 	Get the max of two values
 */
#ifndef max_of
#define max_of(x,y)		((y > x) ? y : x)
#endif
/******************************************************************************/

/*******************************************************************************
 * min_of
 * 	Get the min of two values
 */
#ifndef min_of
#define min_of(x,y)		((y < x) ? y : x)
#endif
/******************************************************************************/

/*******************************************************************************
 * __constructor
 * 	constructor attribute
 */
#ifndef __constructor
#ifdef __GNUC__
#define __constructor	__attribute__((constructor))
#endif
#endif
/******************************************************************************/

/*******************************************************************************
 * __packed
 * 	packed attribute
 */
#ifndef __packed
#ifdef __GNUC__
#define __packed	__attribute__((packed))
#endif
#endif
/******************************************************************************/

/*******************************************************************************
 * __unused
 * 	unused attribute
 */
#ifndef __unused
#ifdef __GNUC__
#define __unused	__attribute__((unused))
#endif
#endif
/******************************************************************************/

/*******************************************************************************
 * __printf_check
 * 	attribute for printf style functions. istr is the 1 based index of the
 * 	format string. iarg is the 1 based index of the arguments.
 */
#ifndef __printf_check
#ifdef __GNUC__
#define __printf_check(istr, iarg) \
			__attribute__((format (printf, istr, iarg)))
#endif
#endif
/******************************************************************************/

/*******************************************************************************
 * Odd Even Check Macros
 */
#ifndef IS_ODD
#define IS_ODD(x)               (x & 0x1)
#endif

#ifndef IS_EVEN
#define IS_EVEN(x)              (!(IS_ODD(x)))
#endif
/******************************************************************************/

#endif /* I__PRJUTIL_H__ */
