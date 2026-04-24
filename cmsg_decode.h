//
// Created by SHZheng_Li on 2026/4/20.
//

#ifndef CLIENT_CMSG_DECODE_H
#define CLIENT_CMSG_DECODE_H

//解析服务器返回信息
int decode_server_resp(char *buff, int len);
//解析好友管理信息
int decode_friend_mgt_resp(char *buff, int len);

#endif //CLIENT_CMSG_DECODE_H
