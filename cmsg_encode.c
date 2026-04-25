//
// Created by SHZheng_Li on 2026/4/19.
//

#include "im_client.h"
#include "cmsg_encode.h"
#include "msg_cipher.h"

// 初始化消息头
// 参数：
// @msg_hdr: 存放消息头结构体的指针
// @type: 消息类型
// @stype: 消息子类型
// @len: 消息长度
void init_msg_hdr(MSG_HDR *msg_hdr, unsigned char type, unsigned char stype, int len) {
    msg_hdr->msg_type = htons((type << 8) | stype);
    msg_hdr->msg_len = htons(len);
}

// 初始化注册结构体
// 参数：
// @reg_msg: 存放注册结构体的指针
// @userSelf: 用户自身信息结构体指针
// 返回值：
// 注册结构体总大小（字节数），用于send()发送长度
int init_reg_struct(REG_MSG *reg_msg, USER_SELF *userSelf) {
    memset(reg_msg, 0, sizeof(*reg_msg));
    memcpy(reg_msg->rName, userSelf->self_name, MAX_NAME_LEN);
    strncpy(reg_msg->rPasswd, userSelf->self_passwd, MAX_PASSWORD_LEN -1);
    reg_msg->rPasswd[MAX_PASSWORD_LEN - 1] = '\0';

    return sizeof(*reg_msg);
}

//编码注册消息
int encode_reg_msg(char *buff, int *len, unsigned char stype) {
    *len = init_reg_struct((REG_MSG *) ((MSG_HDR *) buff)->msg_data, &user_self);

}

// 初始化登录结构体
// 参数：
// @login_msg: 存放登录结构体的指针
// @userSelf: 用户自身信息结构体指针
// 返回值：
// 成功返回登录消息结构体总大小（字节数），用于send()发送长度
int init_login_struct(LOGIN_MSG *login_msg, USER_SELF *userSelf) {
    memset(login_msg, 0, sizeof(*login_msg));
    login_msg->login_id = htonl(userSelf->self_id);
    strncpy(login_msg->login_passwd, userSelf->self_passwd, MAX_PASSWORD_LEN -1);
    login_msg->login_passwd[MAX_PASSWORD_LEN - 1] = '\0';

    return sizeof(*login_msg);
}

// 封装登录消息
// 参数：
// @buff: 存放编码消息的缓冲区
// @len: 存放编码消息长度
// @stype: 子类型
// 返回值：
// 编码消息长度
int encode_login_msg(const char *buff, int *len, unsigned char stype) {
    *len = init_login_struct((LOGIN_MSG *)((MSG_HDR *)buff)->msg_data, &user_self);
    init_msg_hdr((MSG_HDR *)buff, MSG_LOGIN, stype, sizeof(LOGIN_MSG));
    *len += sizeof(MSG_HDR);

    return *len;
}

// 初始化聊天结构体
// 参数：
// @chat_msg: 存放聊天结构体的指针
// @userSelf: 用户自身信息结构体指针
// @msg: 聊天消息
// @recv_id: 接收方id
// 返回值：
// 聊天结构体总大小（字节数），用于send()发送长度
int init_chat_struct(CHAT_MSG *chat_msg, USER_SELF *userSelf, char *msg, int recv_id) {
    memset(chat_msg, 0, sizeof(*chat_msg));
    chat_msg->chat_sid = htonl(userSelf->self_id);
    chat_msg->chat_rid = htonl(recv_id);

    msg_encipher(msg, CIPHER_KEY); //消息加密
    strcpy(chat_msg->chat_msg, msg);

    return sizeof(*chat_msg) + strlen(msg) +1;// +1是'\0'
}

// 编码聊天消息
// 参数：
// @buff: 存放编码消息的缓冲区
// @len: 存放编码消息长度
// @stype: 子类型
// 返回值：
// 聊天消息长度
int encode_chat_msg(const char *buff, int *len, unsigned char stype, int recv_id, char *msg) {
    *len = init_chat_struct((CHAT_MSG *)((MSG_HDR *)buff)->msg_data, &user_self, msg, recv_id);
    init_msg_hdr((MSG_HDR *)buff, MSG_CHAT, stype, *len);
    *len += sizeof(MSG_HDR);

    return *len;
}

//初始化登出结构体
// 参数：
// @logout_msg: 存放登出结构体的指针
// @userSelf: 用户自身信息结构体指针
// 返回值：
// 登出结构体总大小（字节数），用于send()发送长度
int init_logout_struct(LOGOUT_MSG *logout_msg, USER_SELF *userSelf) {
    memset(logout_msg, 0, sizeof(*logout_msg));
    logout_msg->logout_id = htonl(userSelf->self_id);
    strncpy(logout_msg->logout_password, userSelf->self_passwd, MAX_PASSWORD_LEN -1);
    logout_msg->logout_password[MAX_PASSWORD_LEN - 1] = '\0';

    return sizeof(LOGOUT_MSG);
}

//编码登出消息
// 参数：
// @buff: 存放编码消息的缓冲区
// @len: 存放编码消息长度
// @stype: 子类型
// 返回值：
// 登出消息长度
int encode_logout_msg(const char *buff, int *len, unsigned char stype) {
    init_logout_struct((LOGOUT_MSG *)((MSG_HDR *)buff)->msg_data, &user_self);
    init_msg_hdr((MSG_HDR *)buff, MSG_LOGOUT, stype, sizeof(LOGOUT_MSG));
    *len = sizeof(MSG_HDR) + sizeof(LOGOUT_MSG);

    return *len;
}

//初始化好友管理结构体
// 参数：
// @friend_op: 存放好友管理结构体的指针
// @userSelf: 用户自身信息结构体指针
// @stype: 子类型
// @id: 好友id数组
// @count: 好友id个数
// 返回值：
// 好友管理结构体总大小（字节数），用于send()发送长度
int init_friend_mgt_struct(FRIEND_OP *friend_op, USER_SELF *userSelf, unsigned char stype, int *id, int count) {
// #define F_LREG      0x01    //好友注册
// #define F_ADD       0x02    //添加好友
// #define F_DEL       0x03    //删除好友
// #define F_ALST      0x04    //显示所有好友
// #define F_STAT      0x05    //查询好友状态
    int *p = friend_op->friend_fid;

    friend_op->friend_id = htonl(userSelf->self_id);
    for (int i = 0; i < count; i++, p++, id++) {
        *p = htonl(*id);
    }

    return sizeof(FRIEND_OP) + sizeof(int) * count;
}

//编码好友管理消息
// 参数：
// @buff: 存放编码消息的缓冲区
// @len: 存放编码消息长度
// @stype: 子类型
// @id: 好友id数组
// @count: 好友id个数
// 返回值：
// 好友管理消息长度
int encode_friend_mgt_msg(char *buff, int *len, unsigned char stype, int *id, int count) {
    *len = init_friend_mgt_struct((FRIEND_OP *)((MSG_HDR *)buff)->msg_data, &user_self, stype, id, count);
    init_msg_hdr((MSG_HDR *)buff, MSG_FRNDMGT, stype, *len);
    *len += sizeof(MSG_HDR);

    return *len;
}

// int decode_login_msg(char *buff, int len, LOGIN_MSG **login_msg) {
//     if (len != sizeof(LOGIN_MSG)) {
//         printf("server: login msg failed to check!");
//         return -1;
//     }
//     printf("server: recv login msg, len=%d", len);
//
//     *login_msg = (LOGIN_MSG *)(buff + sizeof(MSG_HDR));
//     (*login_msg)->login_id = ntohl((*login_msg)->login_id);
//
//     printf("server: recv login msg, len=%d, id=%d", len, (*login_msg)->login_id);
//
//     return 0;
//     //密码稍后再写
// }
