//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "cmsg_decode.h"
#include "msg_type.h"

// 对消息进行统一处理
// 获取消息的类型，子类型，消息长度
// 判断消息长度是否合法
// switch对不同的消息进行不同的处理

//解析服务器返回信息
int decode_server_resp(char *buff, int len) {
    unsigned char type;
    REG_MSG_RESP *reg_resp;
    LOGIN_MSG_RESP *login_resp;
    FRIEND_OP_RESP *friend_op_resp;
    FRIEND_STAT *friend_stat;
    CHAT_RESP *chat_resp;
    CHAT_MSG *chat_msg;

    short friend_num;
    MSG_HDR *hdr = (MSG_HDR *) buff;
    short msg_type, msg_len;
    unsigned char stype;

    msg_type = ntohs(hdr->msg_type);
    msg_len = ntohs(hdr->msg_len);
    stype = msg_type & 0xff;

    if (len != (sizeof(MSG_HDR) + msg_len)) {
        printf("client: resp msg len check error!\n");
        return -1;
    }

    type = (msg_type >> 8) & 0xff;

    switch (type & 0x7f) {
        case MSG_REG:
            // if (decode_reg_resp()) {
            //     handle_reg_resp();
            // }
            break;

        default:break;
    }
}