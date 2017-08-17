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

#include <dict.h>
#include <prjutil.h>
#include <logging.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


static inline int compute_hash(const char *key)
{
	int h = 0, i = 1;
	while(*key) {
		h += ((int)(*key++) * 7 * i++);
	}
	return h % DICT_HASH_SIZE;
}

struct dictionary *new_dict(void)
{
	struct dictionary *d;

	if((d = malloc(sizeof(*d))) == NULL) {
		logerror("could not allocate new dictionary : %s\n",
				strerror(errno));
		return NULL;
	}

	memset(d, 0, sizeof(*d));

	return d;
}

void delete_dict(struct dictionary *dict)
{
	int i;

	for(i = 0; i < DICT_HASH_SIZE; i++) {
		struct kvpair_mgr *kvm;
		struct hlist_node *n, *t;

		hlist_for_each_entry_safe(kvm, n, t, &dict->kvtable[i], node) {
			hlist_del(n);
			delete_kvpair_mgr(kvm);
			dict->entries--;
		}
	}

	if(dict->entries != 0)
		logwarn("dict->entries non-zero on free\n");

	free(dict);
}

struct kvpair *new_kvpair(const char *key, const void *value, size_t size)
{
	struct kvpair *kvp;
	/* key size + null termination */
	uint32_t keylen = strlen(key) + 1;
	/* size of key + null termination + value data size */
	uint32_t kvsize = keylen + size;

	if((kvp = malloc(sizeof(struct kvpair) + kvsize)) == NULL) {
		logerror("could not allocate new key/value : %s\n",
				strerror(errno));
		return NULL;
	}

	kvp->kvsize = kvsize;
	kvp->voffset = keylen;

	strcpy((char *)&kvp->kv[0], key);
	memcpy(kvpair_value(kvp, NULL), value, size);

	return kvp;
}

void delete_kvpair(struct kvpair *kvp)
{
	free(kvp);
}

struct kvpair_mgr *new_kvpair_mgr(struct kvpair *kvp,
		void (*free_cb)(struct kvpair *))
{
	struct kvpair_mgr *kvm;

	if((kvm = malloc(sizeof(*kvm))) == NULL) {
		logerror("could not allocate new key/value mgr: %s\n",
				strerror(errno));
		return NULL;
	}

	INIT_HLIST_NODE(&kvm->node);
	kvm->free = free_cb;
	kvm->kvp = kvp;

	return kvm;
}

void delete_kvpair_mgr(struct kvpair_mgr *kvm)
{
	if(kvm->free)
		kvm->free(kvm->kvp);
	free(kvm);
}

static struct kvpair_mgr *find_kvpair_with_hash(struct dictionary *dict,
		int hash, const char *key)
{
	struct kvpair_mgr *ret = NULL;
	struct kvpair_mgr *kvm;
	struct hlist_node *n;

	hlist_for_each_entry(kvm, n, &dict->kvtable[hash], node) {
		if(strcmp(key, kvpair_key(kvm->kvp)) == 0) {
			ret = kvm;
			break;
		}
	}

	return ret;
}

struct kvpair_mgr *find_kvpair(struct dictionary *dict, const char *key)
{
	int hash = compute_hash(key);
	return find_kvpair_with_hash(dict, hash, key);
}

struct kvpair_mgr *add_kvpair(struct dictionary *dict, struct kvpair *kvp,
		void (*free_cb)(struct kvpair *))
{
	struct kvpair_mgr *kvm;
	struct kvpair_mgr *kvm_old;
	int hash;

	if((kvm = new_kvpair_mgr(kvp, free_cb)) == NULL) {
		return NULL;
	}

	hash = compute_hash(kvpair_key(kvp));

	if((kvm_old = find_kvpair_with_hash(dict, hash, kvpair_key(kvp)))
			!= NULL) {
		dict->entries--;
		hlist_del(&kvm_old->node);
		delete_kvpair_mgr(kvm_old);
	}

	hlist_add_head(&kvm->node, &dict->kvtable[hash]);
	dict->entries++;

	return kvm;
}

void *dict_add_key(struct dictionary *dict, const char *key,
		const void *value, size_t size)
{
	void *ret = NULL;
	struct kvpair *kvp;

	if((kvp = new_kvpair(key, value, size)) == NULL) {
		goto exit;
	}

	if(add_kvpair(dict, kvp, &delete_kvpair) == NULL) {
		delete_kvpair(kvp);
		goto exit;
	}

	ret = kvpair_value(kvp, NULL);

exit:
	return ret;
}

void dict_remove_key(struct dictionary *dict, const char *key)
{
	struct kvpair_mgr *kvm = find_kvpair(dict, key);

	if(kvm) {
		hlist_del(&kvm->node);
		delete_kvpair_mgr(kvm);
	}
}

int dict_has_key(struct dictionary *dict, const char *key)
{
	if(find_kvpair(dict, key))
		return 1;

	return 0;
}

void *dict_value_for_key(struct dictionary *dict, const char *key, size_t *size)
{
	struct kvpair_mgr *kvm = find_kvpair(dict, key);

	if(kvm)
		return kvpair_value(kvm->kvp, size);

	return NULL;
}

size_t dict_serialized_size(struct dictionary *dict)
{
	int i;
	size_t sz = 0;
	struct kvpair_mgr *kvm;
	struct hlist_node *n;

	/* figure out the size of the serialized data */
	for(i = 0; i < DICT_HASH_SIZE; i++) {
		hlist_for_each_entry(kvm, n, &dict->kvtable[i], node) {
			sz += sizeof(*(kvm->kvp));
			sz += kvm->kvp->kvsize;
		}
	}

	return sz;
}

ssize_t dict_serialize_into(struct dictionary *dict, void *buf)
{
	int i;
	ssize_t sz;
	struct kvpair_mgr *kvm;
	struct hlist_node *n;

	for(i = 0, sz = 0; i < DICT_HASH_SIZE; i++) {
		hlist_for_each_entry(kvm, n, &dict->kvtable[i], node) {
			size_t kvsz = sizeof(*(kvm->kvp)) + kvm->kvp->kvsize;
			memcpy(buf + sz, kvm->kvp, kvsz);
			sz += kvsz;
		}
	}

	return sz;
}

void *dict_serialize(struct dictionary *dict, size_t *size)
{
	size_t sz;
	void *serd;

	/* figure out the size of the serialized data */
	sz = dict_serialized_size(dict);
	if(size)
		*size = sz;

	/* allocate serialized data buffer */
	if((serd = malloc(sz)) == NULL) {
		logerror("could not allocate serialized buffer : %s\n",
				strerror(errno));
		return NULL;
	}

	/* serialize into buffer */
	if(dict_serialize_into(dict, serd) < 0) {
		free(serd);
		return NULL;
	}

	return serd;
}

struct dictionary *dict_deserialize(void *serialized_buf, size_t size)
{
	size_t i;
	struct kvpair *kvp = serialized_buf;
	struct kvpair *new_kvp;
	struct dictionary *d = new_dict();

	if(d == NULL)
		return NULL;

	for(i = 0; i < size; i += (sizeof(*kvp) + kvp->kvsize),
			kvp = (serialized_buf + i)) {
		size_t value_size = 0;
		/* sanity check values */
		if(((sizeof(*kvp) + kvp->kvsize) > size) ||
				(kvp->voffset > kvp->kvsize)) {
			logerror("deserialize sanity check failed\n");
			goto exit_error;
		}
		/* generate an all new kvpair */
		kvpair_value(kvp, &value_size);
		if((new_kvp = new_kvpair(kvpair_key(kvp),
					kvpair_value(kvp, NULL),
					value_size)) == NULL) {
			goto exit_error;
		}
		/* add it to the dictionary */
		if(add_kvpair(d, new_kvp, &delete_kvpair) == NULL) {
			goto exit_error;
		}
	}

	return d;

exit_error:
	delete_dict(d);
	return NULL;
}

struct dictionary *dict_for_buf(void *serialized_buf, size_t size,
		void (*free_cb)(struct kvpair *))
{
	size_t i;
	struct kvpair *kvp = serialized_buf;
	struct dictionary *d = new_dict();

	if(d == NULL)
		return NULL;

	for(i = 0; i < size; i += (sizeof(*kvp) + kvp->kvsize),
			kvp = (serialized_buf + i)) {
		/* sanity check values */
		if(((sizeof(*kvp) + kvp->kvsize) > size) ||
				(kvp->voffset > kvp->kvsize)) {
			logerror("deserialize sanity check failed\n");
			goto exit_error;
		}
		/* add it to the dictionary */
		if(add_kvpair(d, kvp, free_cb) == NULL) {
			goto exit_error;
		}
	}

	return d;

exit_error:
	delete_dict(d);
	return NULL;
}
