/*
 * ll.h - Link List Implementation.
 *
 * Copyright (C) 2009 Robert C. Curtis
 *
 * ll.h is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * ll.h is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ll.h. If not, see <http://www.gnu.org/licenses/>.
 */

/****************************************************************************/

#include <prjutil.h>

/* DESCRIPTION: This is a doubly-linked circular list implementation heavily
 * borrowed from the Linux Kernel's list.h
 */

#ifndef I__LL_H__
	#define I__LL_H__

/* list_entry - get the struct for this entry
 * ptr:		the ll_t pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the node within the struct
 */
#define list_entry(ptr, type, member)	container_of(ptr, type, member)

/* list_first_entry - get the struct for the first entry (assumes not empty)
 * ptr:		the head ll_t pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the head within the struct
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/* list_last_entry - get the struct for the last entry (assumes not empty)
 * ptr:		the head ll_t pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the head within the struct
 */
#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

/* list_next_entry - get the struct for the next entry (assumes not empty)
 * ptr:		the head ll_t pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the head within the struct
 */
#define list_next_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/* list_prev_entry - get the struct for the previous entry (assumes not empty)
 * ptr:		the head ll_t pointer
 * type:	the type of structure this is embedded in
 * member:	the name of the head within the struct
 */
#define list_prev_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

/* list_for_each - iterate over a list
 * pos:		the ll_t pointer to use as a loop cursor
 * head:	the head of the list
 */
#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)

/* list_for_each_safe - iterate over a list safe against removal of list entry
 * pos:		the the ll_t pointer to use as a loop cursor.
 * n:		another ll_t pointer to use as temporary storage
 * head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for(pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/* list_for_each_entry - iterate over a list of given type
 * pos:		the type pointer to use as a loop cursor
 * head:	the head of the list
 * member:	the name of the ll_t within the type
 */
#define list_for_each_entry(pos, head, member) \
	for(pos = list_entry((head)->next, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = list_entry(pos->member.next, typeof(*pos), member))

/* list_for_each_entry_safe - iterate over list of given type safe against
 *                            removal of list entry
 * pos:		the type pointer to use as a loop cursor
 * n:		another type pointer to use as temporary storage
 * head:	the head of the list
 * member:	the name of the ll_t within the type
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
		&pos->member != (head);					\
		pos = n, n = list_entry(n->member.next, typeof(*n), member))


/* ll_t - linked list type. embed in data structures you wish to list */
typedef struct list_head {
	struct list_head *next, *prev;
} ll_t;


/* INIT */
#define LIST_HEAD_INIT(name)	{ &(name), &(name) }
#define LIST_HEAD(name)		ll_t name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(ll_t *head)
{
	head->next = head;
	head->prev = head;
}


/* TESTS */

static inline int list_is_last(const ll_t *entry, const ll_t *head)
{
	return entry->next == head;
}

static inline int list_is_first(const ll_t *entry, const ll_t *head)
{
	return entry->prev == head;
}

static inline int list_empty(const ll_t *head)
{
	return head->next == head;
}

static inline int list_is_singular(const ll_t *head)
{
	return !list_empty(head) && (head->next == head->prev);
}


/* ADD */

/* __list_add
 * 	Insert new entry between two entries.
 */
static inline void __list_add(ll_t *n, ll_t *prev, ll_t *next)
{
	next->prev = n;
	n->next = next;
	n->prev = prev;
	prev->next = n;
}

/* list_add
 * 	Insert new entry at beginning of list.
 */
static inline void list_add(ll_t *n, ll_t *head)
{
	__list_add(n, head, head->next);
}

/* list_add_tail
 * 	Insert new entry at end of list (before head).
 */
static inline void list_add_tail(ll_t *n, ll_t *head)
{
	__list_add(n, head->prev, head);
}

/* DELETE */

/* __list_del
 * 	Delete a list entry.
 */
static inline void __list_del(ll_t *prev, ll_t *next)
{
	next->prev = prev;
	prev->next = next;
}

/* list_del
 * 	Delete a list entry.
 */
static inline void list_del(ll_t *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/* list_del_init
 * 	Delete a list entry and make it a new head.
 */
static inline void list_del_init(ll_t *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}


/* REPLACE */

/* list_replace
 * 	replace an old entry with a new one
 */
static inline void list_replace(ll_t *old, ll_t *n)
{
	n->next = old->next;
	n->next->prev = n;
	n->prev = old->prev;
	n->prev->next = n;
}

/* list_replace_init
 * 	replace an old entry with a new one and make old a new list head
 */
static inline void list_replace_init(ll_t *old, ll_t *n)
{
	list_replace(old, n);
	INIT_LIST_HEAD(old);
}


/* MOVE */

/* list_move
 * 	delete from one list and add to another
 */
static inline void list_move(ll_t *entry, ll_t *new_head)
{
	__list_del(entry->prev, entry->next);
	list_add(entry, new_head);
}

/* list_move_tail
 * 	delete from one list and add to another's end
 */
static inline void list_move_tail(ll_t *entry, ll_t *new_head)
{
	__list_del(entry->prev, entry->next);
	list_add_tail(entry, new_head);
}

/* list_swap
 * 	swap the position of two list nodes
 */
static inline void list_swap(ll_t *n1, ll_t *n2)
{
	ll_t tmp;
	tmp.next = n1->next;
	tmp.prev = n1->prev;
	n1->next = n2->next;
	n1->prev = n2->prev;
	n2->next = tmp.next;
	n2->prev = tmp.prev;
	n1->next->prev = n1;
	n1->prev->next = n1;
	n2->next->prev = n2;
	n2->prev->next = n2;
}


/* CUT */

static inline void __list_cut_position(ll_t *list, ll_t *head, ll_t *entry)
{
	ll_t *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/* list_cut_position - Moves the initial part of @head to, up to an including
 * 			@entry, from @head to @list.
 * list		a new list to add all the removed entries
 * head		a list with entries
 * entry	an entry within head, could be the head itself and if so we
 * 		won't cut the list
 */
static inline void list_cut_position(ll_t *list, ll_t *head, ll_t *entry)
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

typedef struct hlist_node {
	struct hlist_node *next, **pprev;
} hl_node_t;

typedef struct hlist_head {
	hl_node_t *first;
} hl_head_t;


/* INIT */

#define HLIST_HEAD_INIT		{ .first = NULL }
#define HLIST_HEAD(name)	hl_head_t name = HLIST_HEAD_INIT
#define INIT_HLIST_HEAD(ptr)	((ptr)->first = NULL)
static inline void INIT_HLIST_NODE(hl_node_t *h)
{
	h->next = NULL;
	h->pprev = NULL;
}


/* TEST */

static inline int hlist_unhashed(const hl_node_t *h)
{
	return !h->pprev;
}

static inline int hlist_empty(const hl_head_t *h)
{
	return !h->first;
}


/* DELETE */

static inline void __hlist_del(hl_node_t *n)
{
	hl_node_t *next = n->next;
	hl_node_t **pprev = n->pprev;
	*pprev = next;
	if(next)
		next->pprev = pprev;
}

static inline void hlist_del(hl_node_t *n)
{
	__hlist_del(n);
	n->next = NULL;
	n->pprev = NULL;
}


/* ADD */

static inline void hlist_add_head(hl_node_t *n, hl_head_t *h)
{
	hl_node_t *first = h->first;
	n->next = first;
	if(first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

static inline void hlist_add_before(hl_node_t *n, hl_node_t *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

static inline void hlist_add_after(hl_node_t *n, hl_node_t *prev)
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

#endif /* I__LL_H__ */
