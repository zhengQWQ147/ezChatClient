//
// Created by SHZheng_Li on 2026/4/20.
//

#ifndef CLIENT_CLIENT_LOG_H
#define CLIENT_CLIENT_LOG_H
//聊天记录结构
// 发送者  id
// 接受者  id
// 发送时间 时间字符串
// 消息内容 字符串

//使用文本文件
// 发送者、接受这、发送时间、消息内容
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FILE_MSG_SEPS "/"   //消息分割符
#define MSG_FIELDS 4        //每行字段数
#define CLIENT_LOG_FILE "msg_backup.txt"    //备份文件

void client_log_save(char *msg, int from, int to);                 //备份函数
void client_log_export(FILE *sfp, FILE *dfp, int from, int to);     //导出函数 sfp源文件 dfp导出文件

#endif //CLIENT_CLIENT_LOG_H
