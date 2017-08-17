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
 * dict.h - Associative Array Implementation.
 */

#include <ll.h>

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#ifndef I__DICT_H__
	#define I__DICT_H__

#ifndef DICT_HASH_SIZE
#define DICT_HASH_SIZE		16
#endif /* DICT_HASH_SIZE */

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Data Structures *****************************/
/* Key/Value Pair Data Structure */
struct kvpair {
	/* total key/value data size */
	uint32_t kvsize;
	/* value data offset */
	uint32_t voffset;

	/* key/value
	 *     The format is <KEY><NULL><VALUE> where kvsize includes the
	 *     key string, null terminator, and value. voffset points to the
	 *     beginning of value.
	 */
	uint8_t kv[];
} __packed;

static inline const char *kvpair_key(struct kvpair *kvp)
{
	return (const char *)&kvp->kv[0];
}

static inline void *kvpair_value(struct kvpair *kvp, size_t *size)
{
	if(size)
		*size = (size_t)(kvp->kvsize - kvp->voffset);
	return (void *)&kvp->kv[kvp->voffset];
}

/* Key/Value Pair Management Data Structure */
struct kvpair_mgr {
	/* hash table node */
	struct hlist_node node;

	/* free handler - called when key/value is deleted */
	void (*free)(struct kvpair *);

	struct kvpair *kvp;
};

/* Dictionary Data Structure */
struct dictionary {
	/* hash table */
	struct hlist_head kvtable[DICT_HASH_SIZE];

	/* entry count */
	unsigned int entries;
};


/******************************* API Functions ******************************/
/* FUNCTION:    new_dict
 *
 * + DESCRIPTION:
 *   - creates a new empty dictionary
 *
 * + RETURNS: struct dictionary *
 *   - pointer to new dictionary, NULL on failure
 */
struct dictionary *new_dict(void);

/* FUNCTION:    delete_dict
 *
 * + DESCRIPTION:
 *   - deletes a dictionary. will delete each key/value pair beforehand.
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - pointer to dictionary
 */
void delete_dict(struct dictionary *dict);

/* FUNCTION:    dict_add_key
 *
 * + DESCRIPTION:
 *   - add a key/value to a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 *   + const void *value
 *     - value pointer
 *   + size_t size
 *     - value size
 *
 * + RETURNS: void *
 *   - pointer to value, NULL on failure
 */
void *dict_add_key(struct dictionary *dict, const char *key,
		const void *value, size_t size);

/* FUNCTION:    dict_remove_key
 *
 * + DESCRIPTION:
 *   - remove a key from a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 */
void dict_remove_key(struct dictionary *dict, const char *key);

/* FUNCTION:    dict_has_key
 *
 * + DESCRIPTION:
 *   - returns true if a dictionary has a key
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 *
 * + RETURNS: int
 *   - boolean
 */
int dict_has_key(struct dictionary *dict, const char *key);

/* FUNCTION:    dict_value_for_key
 *
 * + DESCRIPTION:
 *   - look up a value for a key in a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 *   + size_t *size
 *     - size of found value
 *
 * + RETURNS: void *
 *   - pointer to value, NULL if not found
 */
void *dict_value_for_key(struct dictionary *dict, const char *key,
		size_t *size);

/* FUNCTION:    dict_serialized_size
 *
 * + DESCRIPTION:
 *   - calculates the serialized size of a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *
 * + RETURNS: size_t
 *   - total serialized size
 */
size_t dict_serialized_size(struct dictionary *dict);

/* FUNCTION:    dict_serialize_into
 *
 * + DESCRIPTION:
 *   - serializes a dictionary into a provided buffer
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + void *buf
 *     - destination buffer. must be at least the size returned by
 *     dict_serialized_size().
 *
 * + RETURNS: size_t
 *   - size of serialized data, negative on failure
 */
ssize_t dict_serialize_into(struct dictionary *dict, void *buf);

/* FUNCTION:    dict_serialize
 *
 * + DESCRIPTION:
 *   - serializes a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + size_t *size
 *     - final size of serialized data
 *
 * + RETURNS: void *
 *   - pointer to serialized data, NULL on failure
 */
void *dict_serialize(struct dictionary *dict, size_t *size);

/* FUNCTION:    dict_deserialize
 *
 * + DESCRIPTION:
 *   - deserializes a buffer into a dictionary. this routine copies all of the
 *   original data into a standalone dictionary.
 *
 * + PARAMETERS:
 *   + void *serialized_buf
 *     - serialized buffer
 *   + size_t size
 *     - size of buffer
 *
 * + RETURNS: struct dictionary *
 *   - new dictionary, NULL on failure
 */
struct dictionary *dict_deserialize(void *serialized_buf, size_t size);

/* FUNCTION:    dict_for_buf
 *
 * + DESCRIPTION:
 *   - creates a new dictionary for a serialized dictionary buffer. this
 *   routine doesn't copy any of the original serialized data; it just uses
 *   it in place.
 *
 * + PARAMETERS:
 *   + void *serialized_buf
 *     - serialized buffer
 *   + size_t size
 *     - size of buffer
 *   + void (*free_cb)(struct kvpair *)
 *     - function to call when each kvpair is released. can be NULL.
 *
 * + RETURNS: struct dictionary *
 *   - new dictionary, NULL on failure
 */
struct dictionary *dict_for_buf(void *serialized_buf, size_t size,
		void (*free_cb)(struct kvpair *));
/*************************** Convenience Functions **************************/
/* FUNCTION:    dict_add_string
 *
 * + DESCRIPTION:
 *   - add a string to a dictionary
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 *   + const char *str
 *     - value string
 *
 * + RETURNS: char *
 *   - pointer to value string, NULL on failure
 */
static inline char *dict_add_string(struct dictionary *dict, const char *key,
		const char *str)
{
	return (char *)dict_add_key(dict, key, str, strlen(str) + 1);
}

/* FUNCTION:    dict_str_for_key
 *
 * + DESCRIPTION:
 *   - get a string value for a key
 *
 * + PARAMETERS:
 *   + struct dictionary *dict
 *     - dictionary pointer
 *   + const char *key
 *     - key string
 *
 * + RETURNS: char *
 *   - pointer to value string, NULL for no value
 */
static inline char *dict_str_for_key(struct dictionary *dict, const char *key)
{
	size_t sz = 0;
	char *ret = dict_value_for_key(dict, key, &sz);
	return sz ? ret : NULL;
}

/**************************** Low Level Functions ***************************/
/* These are used internally to the dict library, but can also be used for
 * more advanced and optimized implementations.
 */
struct kvpair *new_kvpair(const char *key, const void *value, size_t size);
void delete_kvpair(struct kvpair *kvp);

struct kvpair_mgr *new_kvpair_mgr(struct kvpair *kvp,
		void (*free_cb)(struct kvpair *));
void delete_kvpair_mgr(struct kvpair_mgr *kvpair_mgr);

struct kvpair_mgr *find_kvpair(struct dictionary *dict, const char *key);

struct kvpair_mgr *add_kvpair(struct dictionary *dict, struct kvpair *kvp,
		void (*free_cb)(struct kvpair *));

#ifdef __cplusplus
}
#endif
#endif /* I__DICT_H__ */
