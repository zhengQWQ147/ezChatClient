//
// Created by SHZheng_Li on 2026/4/19.
//

#ifndef CLIENT_CMSG_ENCODE_H
#define CLIENT_CMSG_ENCODE_H

#include "msg_type.h"
#include "client_user.h"

void init_msg_hdr(MSG_HDR *msg_hdr, unsigned char type, unsigned char stype, int len);                              //初始化消息头
int init_reg_struct(REG_MSG *reg_msg, USER_SELF *userSelf);                                                         //初始化注册结构体
int encode_reg_msg(char *buff, int *len, unsigned char stype);                                                      //编码注册消息
int init_login_struct(LOGIN_MSG *login_msg, USER_SELF *userSelf);                                                   //初始化登录结构体
int encode_login_msg(const char *buff, int *len, unsigned char stype);                                              //编码登录消息
int init_chat_struct(CHAT_MSG *chat_msg, USER_SELF *userSelf, char *msg, int recv_id);                              //初始化聊天结构体
int encode_chat_msg(const char *buff, int *len, unsigned char stype, int recv_id, char *msg);                       //编码聊天消息
int init_logout_struct(LOGOUT_MSG *logout_msg, USER_SELF *userSelf);                                                //初始化登出结构体
int encode_logout_msg(const char *buff, int *len, unsigned char stype);                                             //编码登出消息
int init_friend_mgt_struct(FRIEND_OP *friend_op, USER_SELF *userSelf, unsigned char stype, int *id, int count);     //初始化好友管理结构体
int encode_friend_mgt_msg(char *buff, int *len, unsigned char stype, int *id, int count);                           //编码好友管理消息
#endif //CLIENT_CMSG_ENCODE_H
