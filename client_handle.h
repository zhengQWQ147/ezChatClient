//
// Created by SHZheng_Li on 2026/4/20.
//

#ifndef CLIENT_CLIENT_HANDLE_H
#define CLIENT_CLIENT_HANDLE_H

#include "msg_type.h"
// 处理注册消息
int handle_reg_resp(REG_MSG_RESP *reg_resp);
// 处理登录消息
int handle_login_resp(LOGIN_MSG_RESP *login_resp);
// 处理好友管理消息
int handle_friend_mgt_resp(unsigned short stype, FRIEND_OP_RESP *friend_op_resp, FRIEND_STAT *friend_stat, short friend_number);

#endif //CLIENT_CLIENT_HANDLE_H
