//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "client_handle.h"

#include "client_log.h"
#include "client_user.h"
#include "msg_cipher.h"
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
    //好友管理信息子类型
    // #define F_LREG      0x01    //好友注册
    // #define F_ADD       0x02    //添加好友
    // #define F_DEL       0x03    //删除好友
    // #define F_ALST      0x04    //显示所有好友
    // #define F_STAT      0x05    //查询好友状态
    switch (stype) {
        case F_LREG:
            break;
        case F_ADD:
            user_list_add(friend_stat, friend_number);
            break;
        case F_DEL:
            user_list_del(friend_stat, friend_number);
            break;
        case F_ALST:
            user_list_output(friend_stat, friend_number);
            break;
        case F_STAT:
            user_list_update(friend_stat, friend_number);
            break;
        default:
            break;
    }
    return 0;
}
// 处理聊天消息
//  其他用户-->自己
//  自己发给其他用户-->服务器反馈是否成功
int handle_chat_msg_or_resp(CHAT_MSG *chat_msg, CHAT_RESP *chat_resp) {
    //服务器返回
    if (chat_resp) {
        return handle_chat_resp(chat_resp);
    }
    // 他人发送给自己
    return handle_chat_msg(chat_msg);
}
// 处理服务器反馈
int handle_chat_resp(CHAT_RESP *chat_resp) {
    printf("client: recv chat msg resp from server...\n");
    if (chat_resp->chat_stat == SEND_OK) {
        printf("client: chat msg send ok\n");
        return 0;
    }
    fprintf(stderr, "chat msg send error, resion: %s", chat_resp->chat_reason);
    return -1;
}
// 处理他人发送的消息
int handle_chat_msg(CHAT_MSG *chat_msg) {
    msg_decipher(chat_msg->chat_msg, CIPHER_KEY);
    printf("client: chat msg from %d: %s\n", chat_msg->chat_sid, chat_msg->chat_msg);

    client_log_save(chat_msg->chat_msg, chat_msg->chat_sid, chat_msg->chat_rid);

    return 0;
}
