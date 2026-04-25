//
// Created by SHZheng_Li on 2026/4/20.
//
#include "cuser_cmd.h"

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
//注册处理
int reg_cmd_process(char *next_token) {
    int len;
    char *p[REG_CMD_FLDS]; //用户名 密码 密码

    if (cmdline_process(next_token, p, REG_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, REG_C_USAGE);
        return -1;
    }

    if (strlen(p[0]) > MAX_NAME_LEN -1) {
        fprintf(stderr, "用户名长度不能超过%d\n%s",MAX_NAME_LEN -1, REG_C_USAGE);
        return -1;
    }

    if (strcmp(p[1], p[2]) != 0) {
        fprintf(stderr, "前后密码不一致\n%s",REG_C_USAGE);
        return -1;
    }

    if (strlen(p[1]) > MAX_PASSWORD_LEN -1) {
        fprintf(stderr, "密码长度不能超过%d\n%s",MAX_PASSWORD_LEN -1, REG_C_USAGE);
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
    int id, len;
    char *p[LOGIN_CMD_FLDS];
    if (cmdline_process(next_token, p, LOGIN_CMD_FLDS, 1) < 0
    || !(id = atoi(p[0]))
    || strlen(p[1]) > MAX_NAME_LEN -1) {
        fprintf(stderr, "%s%s",INV_C_USAGE, LOGIN_C_USAGE);
        return -1;
    }

    user_self.self_id = id;
    memset(user_self.self_passwd, 0, MAX_PASSWORD_LEN);
    strncpy(user_self.self_passwd, p[1], MAX_PASSWORD_LEN);
    user_self.self_passwd[MAX_PASSWORD_LEN -1] = '\0';

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    encode_login_msg(user_self.self_buff, &len, 0);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;

    return 0;
}
//消息处理
int mesg_cmd_process(char *next_token) {
    char *p[MESG_CMD_FLDS];
    int from, to;
    FILE *fp;

    if (cmdline_process(next_token, p, MESG_CMD_FLDS, 1) < 0
    || !(from = atoi(p[0]))
    || !(to = atoi(p[1]))) {
        fprintf(stderr, "%s%s",INV_C_USAGE, MESG_C_USAGE);
        return -1;
    }

    fp = fopen(CLIENT_LOG_FILE, "r");
    if (fp == NULL) {
        fprintf(stderr, "open file: \"%s\" failed!\n", CLIENT_LOG_FILE);
        return -1;
    }

    client_log_export(fp, stdout, from, to);

    fclose(fp);
    return 0;
}
//处理好友列表
int friend_list_cmd_process(char *next_token) {
    int len;
    char *p[FRIEND_LIST_CMD_FLDS];

    if (cmdline_process(next_token, p, FRIEND_LIST_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, FRIEND_LIST_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    encode_friend_mgt_msg(user_self.self_buff, &len, F_LREG, NULL, 0);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;

    return 0;
}
//添加好友处理
int friend_add_cmd_process(char *next_token) {
    int len, id;
    char *p[FRIEND_ADD_CMD_FLDS];
    if (cmdline_process(next_token, p, FRIEND_ADD_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, FRIEND_ADD_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    id = atoi(p[0]);

    encode_friend_mgt_msg(user_self.self_buff, &len, F_ADD, &id, 1);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;
    return 0;
}
//删除好友处理
int friend_del_cmd_process(char *next_token) {
    int len, id;
    char *p[FRIEND_DEL_CMD_FLDS];
    if (cmdline_process(next_token, p, FRIEND_DEL_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, FRIEND_DEL_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    id = atoi(p[0]);

    encode_friend_mgt_msg(user_self.self_buff, &len, F_DEL, &id, 1);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;
    return 0;
}
//查询所有的在线用户
int all_list_cmd_process(char *next_token) {
    int len;
    char *p[ALL_LIST_CMD_FLDS];
    if (cmdline_process(next_token, p, ALL_LIST_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, USER_LIST_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);

    encode_friend_mgt_msg(user_self.self_buff, &len, F_ALST, NULL, 0);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;
    return 0;
}
// 处理好友状态查询
int friend_stat_cmd_process(char *next_token) {
    int len, id;
    char *p[FRIEND_STAT_CMD_FLDS];
    if (cmdline_process(next_token, p, FRIEND_STAT_CMD_FLDS, 1) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, FRIEND_STAT_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    id = atoi(p[0]);

    encode_friend_mgt_msg(user_self.self_buff, &len, F_STAT, &id, 1);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;
    return 0;
}
//处理聊天消息
int chat_cmd_process(char *next_token) {
    int len, recv_id;
    char *p[CHAT_CMD_FLDS];
    if (cmdline_process(next_token, p, CHAT_CMD_FLDS, 0) < 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, CHAT_C_USAGE);
        return -1;
    }

    memset(user_self.self_buff, 0, MAX_MSG_SIZE);
    recv_id = atoi(p[0]);

    if (recv_id == 0) {
        fprintf(stderr, "%s%s",INV_C_USAGE, CHAT_C_USAGE);
        return -1;
    }

    encode_chat_msg(user_self.self_buff, &len, 0, recv_id, p[1]);
    client_send(user_self.self_buff, len);

    user_self.self_msg_stat = MSG_RECVING;
    return 0;
}
//debug
int debug_cmd_process(char *next_token) {
    CLIENT_FRIEND *pos;

    printf("client: id = %d, name = %s, password = %s\n",user_self.self_id, user_self.self_name, user_self.self_passwd);
    printf("client: sock_fd = %d, user_stat = %d, friend_count = %d\n",user_self.self_socket_fd, user_self.self_stat, user_self.self_friend_count);

    user_friend_for_each_entry(pos, &user_self.self_friend_head, friend_node) {
        printf("client: friend id = %d, name = %s, stat = %d", pos->friend_id, pos->friend_name, pos->friend_stat);
    }

    return 0;
}
//检查用户状态
int user_stat_check(char *token) {
    if (_stricmp(token, REG_CMD)   != 0     &&
        _stricmp(token, LOGIN_CMD) != 0     &&
        _stricmp(token, HELP_CMD)  != 0     &&
        _stricmp(token, MESG_CMD)  != 0     &&
        _stricmp(token, EXIT_CMD)  != 0) {
        if (user_self.self_stat != LOGIN_CSTAT) {
            fprintf(stderr, "%s",INV_STAT_USAGE);
            return -1;
        }
    }
    return 0;
}
//登出处理
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
//help
int help_cmd_process(char *next_token) {
    printf("%s", INV_C_USAGE);
    return 0;
}

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
    if (_stricmp(token, FRIEND_LIST_CMD) == 0) {
        return friend_list_cmd_process(next_token);
    }
    if (_stricmp(token, FRIEND_ADD_CMD) == 0) {
        return friend_add_cmd_process(next_token);
    }
    if (_stricmp(token, FRIEND_DEL_CMD) == 0) {
        return friend_del_cmd_process(next_token);
    }
    if (_stricmp(token, ALL_LIST_CMD) == 0) {
        return all_list_cmd_process(next_token);
    }
    if (_stricmp(token, FRIEND_STAT_CMD) == 0) {
        return friend_stat_cmd_process(next_token);
    }
    if (_stricmp(token, CHAT_CMD) == 0) {
        return chat_cmd_process(next_token);
    }
    if (_stricmp(token, EXIT_CMD) == 0) {
        return logout_cmd_process(next_token);
    }
    if (_stricmp(token, MESG_CMD) == 0) {
        return mesg_cmd_process(next_token);
    }
    if (_stricmp(token, DEBUG_CMD) == 0) {
        return debug_cmd_process(next_token);
    }
    if (_stricmp(token, HELP_CMD) == 0) {
        return help_cmd_process(next_token);
    }
    fprintf(stderr, "%s%s",INV_C_USAGE, CLIENT_USAGE);
    return -1;
}