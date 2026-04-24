//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "cmsg_decode.h"

#include "client_handle.h"
#include "client_user.h"
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
    unsigned char stype;

    short msg_type = (short) ntohs(hdr->msg_type);
    short msg_len = (short) ntohs(hdr->msg_len);
    stype = msg_type & 0xff;

    if (len != (sizeof(MSG_HDR) + msg_len)) {
        printf("client: resp msg len check error!\n");
        return -1;
    }

    type = (msg_type >> 8) & 0xff;

    switch (type & 0x7f) {
        case MSG_REG:
            if (decode_reg_resp(buff, msg_len, &reg_resp) >= 0) {
                handle_reg_resp(reg_resp);
            }
            break;
        case MSG_LOGIN:
            if (decode_login_resp(buff, msg_len, &login_resp) >= 0) {
                handle_login_resp(login_resp);
            }
            break;
        case MSG_FRNDMGT:
            if (decode_friend_mgt_resp(buff, msg_len, &friend_op_resp, &friend_stat, &friend_num) >= 0) {
                handle_friend_mgt_resp(stype, friend_op_resp, friend_stat, friend_num);
            }
            break;
        case MSG_CHAT:
            if (decod_chat_msg_or_resp(buff, msg_len, &chat_resp, &chat_msg) >= 0) {
                handle_chat_msg_or_resp(chat_msg, chat_resp);
            }
            break;
            
        default:break;
    }
}
//解析注册回复
int decode_reg_resp(char *buff, int len, REG_MSG_RESP **reg_msg_resp) {
    MSG_HDR *hdr = (MSG_HDR *) buff;
    REG_MSG_RESP *rp = (REG_MSG_RESP *)(hdr +1);

    if (len > sizeof(REG_MSG_RESP)) {
        if (len > sizeof(REG_MSG_RESP) + MAX_ERR_LEN || len != sizeof(REG_MSG_RESP) + strlen(rp->r_reason) +1) {
            printf("client: reg resp len check error! invalid msg len = %d\n",len);
            return -1;
        }else if (len != sizeof(REG_MSG_RESP)) {
            printf("client: reg resp2 len check error! invalid msg len = %d\n",len);
            return -1;
        }
        rp->r_id = (int)ntohl(rp->r_id);
        *reg_msg_resp = rp;
        return 0;
    }
}
//解析登录回复
int decode_login_resp(char *buff, int len, LOGIN_MSG_RESP **login_msg_resp) {
    MSG_HDR *hdr = (MSG_HDR *) buff;
    *login_msg_resp = (LOGIN_MSG_RESP *) (hdr + 1);
    if (len > sizeof(LOGIN_MSG_RESP)) {
        if (len > sizeof(LOGIN_MSG_RESP) + MAX_ERR_LEN || len != sizeof(LOGIN_MSG_RESP) + strlen((*login_msg_resp)->login_reason) +1) {
            printf("client: login resp len check error! invalid msg len = %d\n",len);
            return -1;
        }else if (len != sizeof(LOGIN_MSG_RESP)) {
            printf("client: login resp2 len check error! invalid msg len = %d\n",len);
            return -1;
        }
    }

    (*login_msg_resp)->login_stat = (int)ntohl((*login_msg_resp)->login_stat);
    return 0;
}
//解析聊天消息
int decod_chat_msg_or_resp(char *buff,int len, CHAT_RESP **chat_msg_resp, CHAT_MSG **chat_msg) {
    MSG_HDR *hdr = (MSG_HDR *) buff;
    short msg_type = (short)ntohs(hdr->msg_type);

    //MSG_CHAT:     0000 0100 0000 0000
    //(0x01 << 15): 1000 0000 0000 0000
    //server: msg_type | SERVER_FORWARD_FLAG : 1000 0100 0000 0000 //或运算标记用户发送消息，回复消息不进行标记: 0000 0100 0000 0000
    //server->client: 1000 0100 0000 0000
    if (msg_type & SERVER_FORWARD_FLAG) {
        printf("client: real chat msg, len = %d\n", len);
        *chat_msg_resp = NULL;
        *chat_msg = (CHAT_MSG *) (hdr + 1);
    }else {
        printf("client: chat resp msg, len = %d\n", len);
        *chat_msg_resp = (CHAT_RESP *) (hdr + 1);
        *chat_msg = NULL;
    }

    if (*chat_msg_resp) {
        if (len > sizeof(CHAT_RESP)) {
            if (len > sizeof(CHAT_RESP) + MAX_ERR_LEN || len != sizeof(CHAT_RESP) + strlen((*chat_msg_resp)->chat_reason) +1) {
                printf("client: chat resp len check error! invalid msg len = %d\n",len);
                return -1;
            }else if (len != sizeof(CHAT_RESP)) {
                printf("client: chat resp2 len check error! invalid msg len = %d\n",len);
                return -1;
            }
        }
    }else {
        if (len > MAX_MSG_SIZE || len < sizeof(CHAT_MSG)) {
            printf("client: chat msg len check error! invalid msg len = %d\n",len);
            return -1;
        }
        (*chat_msg)->chat_sid = (int)ntohl((*chat_msg)->chat_sid);
        (*chat_msg)->chat_rid = (int)ntohl((*chat_msg)->chat_rid);
    }

    return 0;
}
//解析好友管理信息
int decode_friend_mgt_resp(char *buff, int len, FRIEND_OP_RESP **friend_mgt_resp, FRIEND_STAT **friend_stat, short *friend_number) {
    MSG_HDR *hdr = (MSG_HDR *) buff;
    FRIEND_OP_RESP *friend_resp = (FRIEND_OP_RESP *) (hdr + 1);
    FRIEND_STAT *ps = (FRIEND_STAT *) (friend_resp + 1);

    *friend_number = (short)((len - sizeof(FRIEND_OP_RESP)) / sizeof(FRIEND_STAT));

    if (len > sizeof(FRIEND_OP_RESP)) {

        if ((len - sizeof(FRIEND_OP_RESP)) % sizeof(FRIEND_STAT) != 0 ||
            *friend_number != ntohs(friend_resp->friend_num)) {
            printf("client: friend mgt resp len check error! invalid len = %d, friend number = %d\n",len, *friend_number);
            return -1;
        }else if (len != sizeof(FRIEND_OP_RESP)) {
            printf("client: friend mgt resp2 len check error! invalid len = %d\n",len);
            return -1;
        }
    }

    friend_resp->friend_stat = (short)ntohs(friend_resp->friend_stat);
    *friend_mgt_resp = friend_resp;
    *friend_stat = ps;

    for (int i = 0; i < *friend_number; i++, ps++) {
        ps->friend_id = (int)ntohl(ps->friend_id);
        ps->friend_stat = (int)ntohl(ps->friend_stat);
    }

    return 0;
}
