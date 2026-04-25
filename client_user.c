//
// Created by SHZheng_Li on 2026/4/19.
//
#include "im_client.h"
#include "client_user.h"
#include "list.h"
//初始化用户结构体
int init_user_struct(USER_SELF *userSelf) {
    memset(userSelf, 0, sizeof(*userSelf));
    userSelf->self_id = -1;
    userSelf->self_socket_fd = -1;
    userSelf->self_stat = INIT_CSTAT;
    userSelf->self_msg_stat = MSG_SENDING;

    INIT_LIST_HEAD(&userSelf->self_friend_head);
    return 0;
}
//添加用户
int user_list_add(FRIEND_STAT *friend_stat, short friend_num) {
    CLIENT_FRIEND *cf, *cp;

    struct list_head *pos = NULL, *head = &user_self.self_friend_head;

    for (int i = 0; i < friend_num; i++) {
        printf("client: user %d, name = %s, id = %d, state = %d\n",
        i, friend_stat[i].friend_name, friend_stat[i].friend_id, friend_stat[i].friend_stat);
    }

    for (int i = 0; i < friend_num; i++) {
        list_for_each(pos, head) {
            cp = (CLIENT_FRIEND *) list_entry(pos, CLIENT_FRIEND, friend_node);
            if (cp->friend_id == friend_stat[i].friend_id) {
                strcpy(cp->friend_name, friend_stat[i].friend_name);
                cp->friend_stat = friend_stat[i].friend_stat;
                break;
            }
        }

        if (pos != head) {
            printf("client: user is in the friend list, update status!\n");
            break;
        }

        cf = (CLIENT_FRIEND *) malloc(sizeof(*cf));
        if (cf == NULL) {
            printf("client: malloc error!\n");
            continue;
        }

        printf("client: user not in the friend list, add this user now.\n");
        memset(cf, 0, sizeof(*cf));
        strncpy(cf->friend_name, friend_stat[i].friend_name, MAX_NAME_LEN -1);
        cf->friend_id = friend_stat[i].friend_id;
        cf->friend_stat = friend_stat[i].friend_stat;

        list_add(&cf->friend_node, head);
    }
    return 0;
}
//删除用户
int user_list_del(FRIEND_STAT *friend_stat, short friend_num) {
    CLIENT_FRIEND *cp;
    struct list_head *pos = NULL, *head = &user_self.self_friend_head;

    for (int i = 0; i < friend_num; i++) {
        list_for_each(pos, head) {
            cp = (CLIENT_FRIEND *)list_entry(pos, CLIENT_FRIEND, friend_node);
            if (cp->friend_id == friend_stat[i].friend_id) {
                break;
            }
        }
        if (pos == head) {continue;}
        cp = (CLIENT_FRIEND *) list_entry(pos, CLIENT_FRIEND, friend_node);
        list_del(&cp->friend_node);
        free(cp);
    }
    return 0;
}
//更新用户
int user_list_update(FRIEND_STAT *friend_stat, short friend_num) {
    for (int i = 0; i < friend_num; i++) {
        printf("client: user %d, name = %s, id = %d, state = %d\n",
        i, friend_stat[i].friend_name, friend_stat[i].friend_id, friend_stat[i].friend_stat);
    }
    return 0;
}
//输出用户
int user_list_output(FRIEND_STAT *friend_stat, short friend_num) {
    for (int i = 0; i < friend_num; i++) {
        printf("client: (all friend list)>> user %d, name = %s, id = %d, state = %d\n",
            i, friend_stat->friend_name, friend_stat->friend_id, friend_stat->friend_stat);
    }
    return 0;
}

