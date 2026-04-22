//
// Created by SHZheng_Li on 2026/4/19.
//

#ifndef CLIENT_CLIENT_USER_H
#define CLIENT_CLIENT_USER_H

#include "msg_type.h"
#include "list.h"

typedef struct client_friend {
    char friend_name[MAX_NAME_LEN]; //好友昵称
    int friend_id;                  //好友id
    struct list_head friend_node;   //链表节点
    int friend_stat;                //好友状态
} CLIENT_FRIEND;

//遍历链表,返回client_friend结构体指针
#define ufriend_for_each_entry(pos, head, member) \
for (   \
        pos = (CLIENT_FRIEND *) list_entry((head)->next, CLIENT_FRIEND, member); \
        &pos->member != (head); \
        pos = (CLIENT_FRIEND *) list_entry(pos->member.next, CLIENT_FRIEND, member) \
    )

#define INIT_CSTAT      0
#define LOGIN_CSTAT     1
#define MAX_MSG_SIZE    512
#define MSG_SENDING     0
#define MSG_RECVING     1

//用户自身信息结构体
typedef struct user_self {
    int self_id;                            //自己的id
    char self_name[MAX_NAME_LEN];           //自己的昵称
    char self_passwd[MAX_PASSWORD_LEN];     //密码
    int self_socket_fd;                     //socket状态
    int self_stat;                          //登录状态
    int self_msg_count;                     //消息数量
    struct list_head self_msg_head;         //消息链表头结点
    struct list_head self_friend_head;      //好友链表头结点
    int self_friend_count;                  //好友数量
    char self_buff[MAX_MSG_SIZE];           //消息缓存
    char self_msg_stat;                     //消息状态
    char self_msg_type;                     //消息类型
} USER_SELF;

extern struct user_self user_self;
int init_user_struct(USER_SELF *userSelf);                 //初始化用户结构体
int user_list_add(FRIEND_STAT *friend_stat, short friend_num);     //添加用户
int user_list_del(FRIEND_STAT *friend_stat, short friend_num);     //删除用户
int user_list_update(FRIEND_STAT *friend_stat, short friend_num);     //更新用户
int user_list_output(FRIEND_STAT *friend_stat, short friend_num);     //输出用户

#endif //CLIENT_CLIENT_USER_H
