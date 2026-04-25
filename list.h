//
// Created by SHZheng_Li on 2026/4/19.
//

#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

struct list_head {
    struct list_head *next, *prev;
};



//宏 遍历链表
#define LIST_POSITION ((void *) 0x0)

#define LIST_HEAD_INIT(name) {&(name), &(name)}
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)
#define INIT_LIST_HEAD(ptr) \
    do {(ptr)->next = (ptr); (ptr)->prev = (ptr);}\
    while (0)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

//计算偏移量
// #define offsetof(type, member) ((size_t)(&((type *)0) -> member))

#define container_of(ptr, type, member) \
    ((char *)(ptr)) - offsetof(type, member)

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)



//链表插入
void __list_add(struct list_head *node, struct list_head *prev, struct list_head *next);
void list_add(struct list_head *node, struct list_head *head); //头插
void list_add_tail(struct list_head *node, struct list_head *head); //尾插

//链表删除
void __list_del(struct list_head *prev, struct list_head *next);
void list_del(struct list_head *entry);
void list_del_init(struct list_head *entry);

int list_empty(const struct list_head *head);

#endif //CLIENT_LIST_H
