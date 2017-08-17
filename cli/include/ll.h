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
 * ll.h - Link List Implementation.
 */

#include <prjutil.h>

#ifndef I__LL_H__
	#define I__LL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* list_entry - get the struct for this entry
 * ptr:		the list_head pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the node within the struct
 */
#define list_entry(ptr, type, member)	container_of(ptr, type, member)

/* list_first_entry - get the struct for the first entry (assumes not empty)
 * ptr:		the head list_head pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the head within the struct
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/* list_for_each - iterate over a list
 * pos:		the list_head pointer to use as a loop cursor
 * head:	the head of the list
 */
#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)

/* list_for_each_safe - iterate over a list safe against removal of list entry
 * pos:		the the list_head pointer to use as a loop cursor.
 * n:		another list_head pointer to use as temporary storage
 * head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for(pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/* list_for_each_entry - iterate over a list of given type
 * pos:		the type pointer to use as a loop cursor
 * head:	the head of the list
 * member:	the name of the list_head within the type
 */
#define list_for_each_entry(pos, head, member) \
	for(pos = list_entry((head)->next, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = list_entry(pos->member.next, typeof(*pos), member))

/* list_for_each_entry_rev - iterate over a list of given type in reverse
 * pos:		the type pointer to use as a loop cursor
 * head:	the head of the list
 * member:	the name of the list_head within the type
 */
#define list_for_each_entry_rev(pos, head, member) \
	for(pos = list_entry((head)->prev, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = list_entry(pos->member.prev, typeof(*pos), member))

/* list_for_each_entry_safe - iterate over list of given type safe against
 *                            removal of list entry
 * pos:		the type pointer to use as a loop cursor
 * n:		another type pointer to use as temporary storage
 * head:	the head of the list
 * member:	the name of the list_head within the type
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = n, n = list_entry(n->member.next, typeof(*n), member))

/* list_for_each_entry_safe_rev - iterate over list of given type in reverse
 *                                safe against removal of list entry
 * pos:		the type pointer to use as a loop cursor
 * n:		another type pointer to use as temporary storage
 * head:	the head of the list
 * member:	the name of the list_head within the type
 */
#define list_for_each_entry_safe_rev(pos, n, head, member)		\
	for (pos = list_entry((head)->prev, typeof(*pos), member),	\
		n = list_entry(pos->member.prev, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = n, n = list_entry(n->member.prev, typeof(*n), member))


/* list_head - linked list type. embed in data structures you wish to list */
struct list_head {
	struct list_head *next, *prev;
};


/* INIT */
#define LIST_HEAD_INIT(name)	{ &(name), &(name) }
#define LIST_HEAD(name)		struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *head)
{
	head->next = head;
	head->prev = head;
}


/* TESTS */

static inline int list_is_last(const struct list_head *entry,
		const struct list_head *head)
{
	return entry->next == head;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prev);
}


/* ADD */

/* __list_add
 * 	Insert new entry between two entries.
 */
static inline void __list_add(struct list_head *n, struct list_head *prev,
		struct list_head *next)
{
	next->prev = n;
	n->next = next;
	n->prev = prev;
	prev->next = n;
}

/* list_add
 * 	Insert new entry at beginning of list.
 */
static inline void list_add(struct list_head *n, struct list_head *head)
{
	__list_add(n, head, head->next);
}

/* list_add_tail
 * 	Insert new entry at end of list (before head).
 */
static inline void list_add_tail(struct list_head *n, struct list_head *head)
{
	__list_add(n, head->prev, head);
}

/* DELETE */

/* __list_del
 * 	Delete a list entry.
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/* list_del
 * 	Delete a list entry.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/* list_del_init
 * 	Delete a list entry and make it a new head.
 */
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}


/* REPLACE */

/* list_replace
 * 	replace an old entry with a new one
 */
static inline void list_replace(struct list_head *old, struct list_head *n)
{
	n->next = old->next;
	n->next->prev = n;
	n->prev = old->prev;
	n->prev->next = n;
}

/* list_replace_init
 * 	replace an old entry with a new one and make old a new list head
 */
static inline void list_replace_init(struct list_head *old, struct list_head *n)
{
	list_replace(old, n);
	INIT_LIST_HEAD(old);
}


/* MOVE */

/* list_move
 * 	delete from one list and add to another
 */
static inline void list_move(struct list_head *entry,
		struct list_head *new_head)
{
	__list_del(entry->prev, entry->next);
	list_add(entry, new_head);
}

/* list_move_tail
 * 	delete from one list and add to another's end
 */
static inline void list_move_tail(struct list_head *entry,
		struct list_head *new_head)
{
	__list_del(entry->prev, entry->next);
	list_add_tail(entry, new_head);
}

/* list_swap
 * 	swap the position of two list nodes
 */
static inline void list_swap(struct list_head *n1, struct list_head *n2)
{
	struct list_head tmp;

	/* First insert tmp into n1 */
	tmp.next = n1->next;
	tmp.prev = n1->prev;
	tmp.prev->next = &tmp;
	tmp.next->prev = &tmp;

	/* Now insert n1 into n2 */
	n1->next = n2->next;
	n1->prev = n2->prev;
	n1->prev->next = n1;
	n1->next->prev = n1;

	/* Now insert n2 into tmp */
	n2->next = tmp.next;
	n2->prev = tmp.prev;
	n2->prev->next = n2;
	n2->next->prev = n2;
}


/* CUT */

static inline void __list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	struct list_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/* list_cut_position - Moves the initial part of @head to, up to and including
 * 			@entry, from @head to @list.
 * list		a new list to add all the removed entries
 * head		a list with entries
 * entry	an entry within head, could be the head itself and if so we
 * 		won't cut the list
 */
static inline void list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	if(list_empty(head))
		return;
	if(list_is_singular(head) && (head->next != entry && head != entry))
		return;
	if(entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}


/*******************************************************************************
 * HLIST
 * Doubly linked lists with a single pointer list head. Mostly useful for hash
 * tables where the two pointer list head is too wasteful. You lose the
 * ability to access the tail in O(1).
 */

struct hlist_node {
	struct hlist_node *next, **pprev;
};

struct hlist_head {
	struct hlist_node *first;
};


/* INIT */

#define HLIST_HEAD_INIT		{ .first = NULL }
#define HLIST_HEAD(name)	struct hlist_head name = HLIST_HEAD_INIT
#define INIT_HLIST_HEAD(ptr)	((ptr)->first = NULL)
static inline void INIT_HLIST_NODE(struct hlist_node *h)
{
	h->next = NULL;
	h->pprev = NULL;
}


/* TEST */

static inline int hlist_unhashed(const struct hlist_node *h)
{
	return !h->pprev;
}

static inline int hlist_empty(const struct hlist_head *h)
{
	return !h->first;
}


/* DELETE */

static inline void __hlist_del(struct hlist_node *n)
{
	struct hlist_node *next = n->next;
	struct hlist_node **pprev = n->pprev;
	*pprev = next;
	if(next)
		next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
	__hlist_del(n);
	n->next = NULL;
	n->pprev = NULL;
}


/* ADD */

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
	struct hlist_node *first = h->first;
	n->next = first;
	if(first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

static inline void hlist_add_before(struct hlist_node *n,
		struct hlist_node *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

static inline void hlist_add_after(struct hlist_node *n,
		struct hlist_node *prev)
{
	n->next = prev->next;
	prev->next = n;
	n->pprev = &prev->next;

	if(n->next)
		n->next->pprev = &n->next;
}


/* hlist_entry - get the struct for this entry
 * ptr:		the node pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the node within the struct
 */
#define hlist_entry(ptr, type, member)	container_of(ptr, type, member)

/* hlist_for_each - iterate over a list
 * pos:		the node pointer to use as a loop cursor
 * head:	the head of the list
 */
#define hlist_for_each(pos, head) \
	for(pos = (head)->first; pos; pos = pos->next)

/* hlist_for_each_safe - iterate over a list safe against removal of list entry
 * pos:		the the node pointer to use as a loop cursor.
 * n:		another node pointer to use as temporary storage
 * head:	the head for your list.
 */
#define hlist_for_each_safe(pos, n, head) \
	for(pos = (head)->first; pos && ({ n = pos->next; 1; }); pos = n)

/* hlist_for_each_entry - iterate over a list of given type
 * tpos:	the type pointer to use as a loop cursor
 * pos:		the node pointer to use as a loop cursor
 * head:	the head of the list
 * member:	the name of the node within the type
 */
#define hlist_for_each_entry(tpos, pos, head, member)			\
	for(pos = (head)->first;					\
		pos && 							\
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1; }); \
		pos = pos->next)

/* hlist_for_each_entry_safe - iterate over list of given type safe against
 *                             removal of list entry
 * tpos:	the type pointer to use as a loop cursor
 * pos:		the node pointer to use as a loop cursor
 * n:		another node pointer to use as temporary storage
 * head:	the head of the list
 * member:	the name of the node within the type
 */
#define hlist_for_each_entry_safe(tpos, pos, n, head, member)		\
	for(pos = (head)->first; pos && ({ n = pos->next; 1; }) &&	\
		({ tpos = hlist_entry(pos, typeof(*tpos), member); 1; }); \
		pos = n)

#ifdef __cplusplus
}
#endif
#endif /* I__LL_H__ */
