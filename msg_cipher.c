//
// Created by SHZheng_Li on 2026/4/19.
//
#include "msg_cipher.h"
#include "im_client.h"
//消息加密
// 参数：
// @msg: 待加密消息
// @key: 密钥
void msg_encipher(char *msg, int key) {
    //信息加解密 凯撒加密
    //检查key是否合法,不合法设为3
    if (key < 1 || key > 26) { key = 3;}
    if (msg[strlen(msg) - 1] == '\n') {
        msg[strlen(msg) - 1] = '\0';    //去除换行符
    }
    for (int i = 0; i < strlen(msg); i++) {
        if (msg[i] >= 'a' && msg[i] <= 'z') {
            msg[i] = (int)'a' + (msg[i] - 'a' + key)%26;
        }else if (msg[i] >= 'A' && msg[i] <= 'Z') {
            msg[i] = (int)'A' + (msg[i] - 'A' + key)%26;
        }
    }
}

//消息解密
// 参数：
// @msg: 待解密消息
// @key: 密钥
void msg_decipher(char *msg, int key) {
    //检查key是否合法,不合法设为3
    if (key < 1 || key > 26) { key = 3;}
    //解密
    for (int i = 0; i < strlen(msg); i++) {
        if (msg[i] >= 'a' && msg[i] <= 'z') {
            msg[i] = (int)'a' + (msg[i] - 'a' - key + 26)%26;
        }
        else if (msg[i] >= 'A' && msg[i] <= 'Z') {
            msg[i] = (int)'A' + (msg[i] - 'A' - key + 26)%26;
        }
    }
}