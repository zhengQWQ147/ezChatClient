//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "client_handle.h"
#include "client_user.h"
// 处理注册消息
int handle_reg_resp(REG_MSG_RESP *reg_resp) {
    user_self.self_id = reg_resp->r_id;
    printf("client: recv reg msg from server, id = %d\n", reg_resp->r_id);
    if (reg_resp->r_id == -1) {
        fprintf(stderr, "%s reg failed, reason = %s\n", user_self.self_name, reg_resp->r_reason);
        return -1;
    }
    return 0;
}
// 处理登录消息
int handle_login_resp(LOGIN_MSG_RESP *login_resp) {
    printf("client: recv login resp from server...\n");
    if (login_resp->login_stat == LOGIN_OK) {
        user_self.self_stat = LOGIN_CSTAT;
        memset(user_self.self_name, 0, MAX_NAME_LEN);
        strncpy(user_self.self_name, login_resp->login_name, MAX_NAME_LEN);
        user_self.self_name[MAX_NAME_LEN -1] = '\0';
        return 0;
    }
    fprintf(stderr, "user %s login failed, reason: %s\n", user_self.self_name, login_resp->login_reason);
    return -1;
}
// 处理好友管理消息
int handle_friend_mgt_resp(unsigned short stype, FRIEND_OP_RESP *friend_op_resp, FRIEND_STAT *friend_stat, short friend_number) {
    printf("client: recv friend mgt resp, stype = %d, count = %d, from server...\n", stype, friend_number);
}
