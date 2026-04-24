//
// Created by SHZheng_Li on 2026/4/19.
//

#ifndef CLIENT_MSG_CIPHER_H
#define CLIENT_MSG_CIPHER_H
#define CIPHER_KEY   13

void msg_encipher(char *msg, int key);  //消息加密
void msg_decipher(char *msg, int key);  //消息解密
#endif //CLIENT_MSG_CIPHER_H
