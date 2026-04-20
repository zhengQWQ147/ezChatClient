//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "cuser_cmd.h"

//用户输入命令处理
// 参数：
// buff:用户输入的命令
// 返回值：
int user_cmd_process(char *buff) {
    char *token = NULL, *next_token = NULL, seps[] = "\t";
    token = strtok_s(buff, seps, &next_token);

    if (!token) return 0;

    if (user_stat_check(token) < 0) return -1;  //用户状态检查

    if (_stricmp(token, REG_CMD)) {
        return reg_cmd_process(next_token);
    }
}


int user_stat_check(char *token) {
    return 0;
}

int reg_cmd_process(char *next_token) {
    return 0;
}