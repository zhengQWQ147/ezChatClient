//
// Created by SHZheng_Li on 2026/4/20.
//
#ifndef CLIENT_CUSER_CMD_H
#define CLIENT_CUSER_CMD_H

#define MAX_LINE_LEN        256
#define MAX_PROMPT_LEN      64
#define USER_PROM           "IM_client"

// 用户命令
#define REG_CMD             "reg"               //注册 reg <username> <password> <password>
#define REG_CMD_FLDS        4                   //注册命令字段数
#define LOGIN_CMD           "login"             //登录 login <username> <password>
#define LOGIN_CMD_FLDS      3                   //登录命令字段数
#define FRIEND_LIST_CMD     "friend_list"       //获取好友列表
#define FRIEND_ADD_CMD      "add_friend"        //添加好友
#define FRIEND_DEL_CMD      "del_friend"        //删除好友
#define ALL_LIST_FRIEND_CMD "all_list_friend"   //获取所有好友列表
#define FRIEND_STAT_CMD     "friend_stat"       //获取好友状态
#define CHAT_CMD            "talk"              //聊天
#define EXIT_CMD            "exit"              //退出
#define HELP_CMD            "help"              //帮助
#define MSG_CMD             "msg"               //查询聊天记录
#define DEBUG_CMD           "debug"             //查看当前用户状态

#define REG_C_USAGE         "reg <username> <password> <password> 通过用户名和密码注册用户。\n"
#define FRIEND_LIST_C_USAGE "friend_list 列出当前用户的所有好友。\n"


#define INV_C_USAGE         "无法解析用户命令\n\n"
//命令处理函数
int user_cmd_process(char *buff);
//客户端发送命令
int client_send(char *buff, int len);
//命令字段分解函数
// 参数：
// @next_token: 去除命令关键字的命令行字符串指针
// @p: 各个字段的指针数组
// @len: 字段数
// @strict: 是否严格检查格式标志<1|0>
int cmdline_process(char *next_token, char **p, int len, int strict);
//用户状态检查
int user_stat_check(char *token);
//处理注册
int reg_cmd_process(char *next_token);
//处理登录
int login_cmd_process(char *next_token);

#endif //CLIENT_CUSER_CMD_H
