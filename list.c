//
// Created by SHZheng_Li on 2026/4/19.
//

#include "list.h"

// 插入一个 新节点
// @node：新节点
// @prev：新节点 的前一个节点
// @next：新节点 的下一个节点
void __list_add(struct list_head *node, struct list_head *prev, struct list_head *next) {
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}
// 删除一个 节点
// @prev：节点 的前一个节点
// @next：节点 的下一个节点
void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

//头插节点
void list_add(struct list_head *node, struct list_head *head) {
    __list_add(node, head, head->next);
}

//尾插节点
void list_add_tail(struct list_head *node, struct list_head *head) {
    __list_add(node, head->prev, head);
}

//删除节点
void list_del(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = LIST_POSITION;    //置为空
    entry->prev = LIST_POSITION;    //置为空
}

void list_del_init(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

//判断链表是否为空
int list_empty(const struct list_head *head) {
    return head->next == head;
}