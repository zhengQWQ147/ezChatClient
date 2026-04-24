//
// Created by SHZheng_Li on 2026/4/20.
//
#include "im_client.h"
#include "cuser_cmd.h"
#include "client_user.h"
#include "msg_type.h"
#include "cmsg_encode.h"

//用户输入命令处理
// 参数：
// buff:用户输入的命令
// 返回值：
int user_cmd_process(char *buff) {
    char *token = NULL, *next_token = NULL, seps[] = "\t";
    token = strtok_s(buff, seps, &next_token);

    if (!token) return 0;

    if (user_stat_check(token) < 0) return -1;  //用户状态检查

    if (_stricmp(token, REG_CMD) == 0) {
        return reg_cmd_process(next_token);
    }
    if (_stricmp(token, LOGIN_CMD) == 0) {
        return login_cmd_process(next_token);
    }
}

//命令字段分解函数
// 参数：
// @next_token: 去除命令关键字的命令行字符串指针
// @p:各个字段的指针数组
// @len:字段数
// @strict:是否严格检查格式标志<1|0>
int cmdline_process(char *next_token, char **p, int len, int strict) {
    int n;  //字段数
    char *token = NULL, seps[] = "\t";
    if (strict) {n = len;}
    else {n = len -1;}

    for (int i = 0; i < n; i++) {
        token = strtok_s(NULL, seps, &next_token);
        p[i] = token;
        if (i != n-1 && !p[i]) return -1;   //缺少字段
    }
    if (strict && p[len -1]) return -1;  //多余字段
    if (!strict) {
        p[len -1] = next_token;
        if (next_token) {
            return -1;
        }
    }
    return 0;
}
//发送函数
// 参数：
// @buff:发送缓冲区
// @len:发送长度
int client_send(char *buff, int len) {
    int n;  //成功发送长度
    n = send(user_self.self_socket_fd, buff, len, 0);
    if (n != len) {
        closesocket(user_self.self_socket_fd);
        init_user_struct(&user_self);
    }
    return n;
}

int user_stat_check(char *token) {
    return 0;
}
//注册处理
int reg_cmd_process(char *next_token) {
    int len;
    char *p[REG_CMD_FLDS]; //用户名 密码 密码

    if (cmdline_process(next_token, p, REG_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, REG_C_USAGE);
        return -1;
    }

    if (strlen(p[0]) > MAX_NAME_LEN -1) {
        fprintf(stderr, "用户名长度不能超过%d\n%s",MAX_NAME_LEN, REG_C_USAGE);
        return -1;
    }

    if (strcmp(p[1], p[2]) != 0) {
        fprintf(stderr, "前后密码不一致\n%s",REG_C_USAGE);
        return -1;
    }

    if (strlen(p[1]) > MAX_PASSWORD_LEN -1) {
        fprintf(stderr, "密码长度不能超过%d\n%s",MAX_PASSWORD_LEN, REG_C_USAGE);
        return -1;
    }

    strcpy(user_self.self_name, p[0]);
    strcpy(user_self.self_passwd, p[1]);

    //清空缓存
    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    encode_reg_msg(user_self.self_buff, &len, 0);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;  //接收状态,等待服务器反馈

    return 0;
}
//登录处理
int login_cmd_process(char *next_token) {


    return 0;
}

int logout_cmd_process(char *next_token) {
    int len;
    char *p[LOGOUT_CMD_FLDS];
    if(cmdline_process(next_token, p, LOGOUT_CMD_FLDS, 1) == -1) {
        fprintf(stderr, "%s%s",INV_C_USAGE, EXIT_C_USAGE);
        return -1;
    }
    if (user_self.self_msg_stat == LOGIN_CSTAT) {
        memset(user_self.self_buff, 0, MAX_MSG_SIZE);
        encode_logout_msg(user_self.self_buff, &len, 0);
        send(user_self.self_socket_fd, user_self.self_buff, len, 0);
        user_self.self_msg_stat = MSG_RECVING;
    }
    return -2;
}
