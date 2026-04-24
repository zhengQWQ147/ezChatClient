//
// Created by SHZheng_Li on 2026/4/20.
//
#include "cuser_cmd.h"
#include "client_log.h"

void client_log_save(char *msg, int from, int to) {
    FILE *fp;
    char buff[MAX_LINE_LEN];
    time_t ticks = time(NULL);
    int n = 0;      //buff内写入内容的大小

    fp = fopen(CLIENT_LOG_FILE, "a+");
    if (fp == NULL) {
        perror("client_log open for append error!\n");
        return;
    }
    n += _snprintf(buff +n, sizeof(buff) -n, "%d/", from);
    n += _snprintf(buff +n, sizeof(buff) -n, "%d/", to);    //写入缓冲区buff
    n += _snprintf(buff +n, sizeof(buff) -n, "%.24s/", ctime(&ticks));
    n += _snprintf(buff +n, sizeof(buff) -n, "%s\n", msg);

    buff[n] = '\0';

    fputs(buff, fp);//写入文件

    fclose(fp);
}

void client_log_export(FILE *sfp, FILE *dfp, int from, int to) {

    char buff[MAX_LINE_LEN];
    char *token = NULL, *next_token = NULL, seps[] = "/";//使用strtok_s()分割
    char *p[MSG_FIELDS] = {NULL};

    int msg_src, msg_dst;

    memset(buff, 0, sizeof(buff));

    while (fgets(buff, MAX_LINE_LEN, sfp) != NULL) {
        token = strtok_s(buff, seps, &next_token);
        for (int i = 0; i < MSG_FIELDS; i++) {
            p[i] = token;
            token = strtok_s(NULL, seps, &next_token);
        }
        if (from != 0) {
            msg_src = atoi(p[0]); //atoi字符串变数字 "10" -> 10
        }
        if (to != 0) {
            msg_dst = atoi(p[1]);
        }
        // from 和 to 为 0 ，要导出所有消息
        // 过滤来自某个用户发送的消息
        // 过滤发送至某个用户的消息
        // 过滤 发送者是from 且 接收者是to 的消息
        if (from == 0 && to == 0
            || (from != 0 && to == 0 && msg_src == from)
            || (from == 0 && to != 0 && msg_dst == to)
            || (from != 0 && to != 0 && msg_src == from && msg_dst == to))
        {
            fprintf(dfp, "client-log: <%s> -> <%s>, Time: %s, msg: %s",p[0], p[1], p[2], p[3]);  //向dfp写入信息
        }
        memset(buff, 0, sizeof(buff));  //清空缓冲区buff
    }
}