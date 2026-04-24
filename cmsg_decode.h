//
// Created by SHZheng_Li on 2026/4/20.
//

#ifndef CLIENT_CMSG_DECODE_H
#define CLIENT_CMSG_DECODE_H

#include "im_client.h"
#include "msg_type.h"

//解析服务器返回信息
int decode_server_resp(char *buff, int len);
//解析注册回复
int decode_reg_resp(char *buff, int len, REG_MSG_RESP **reg_msg_resp);
//解析登录回复
int decode_login_resp(char *buff, int len, LOGIN_MSG_RESP **login_msg_resp);
//解析聊天消息
int decod_chat_msg_or_resp(char *buff, int len, CHAT_RESP **chat_msg_resp, CHAT_MSG **chat_msg);
//解析好友管理信息
int decode_friend_mgt_resp(char *buff, int len, FRIEND_OP_RESP **friend_mgt_resp, FRIEND_STAT **friend_stat, short *friend_number);

#endif //CLIENT_CMSG_DECODE_H
