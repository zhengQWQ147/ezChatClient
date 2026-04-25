//
// Created by SHZheng_Li on 2026/4/20.
//
#ifndef CLIENT_CUSER_CMD_H
#define CLIENT_CUSER_CMD_H

#include "client_log.h"
#include "im_client.h"
#include "client_user.h"
#include "msg_type.h"
#include "cmsg_encode.h"


#define CLIENT_LOCAL    1
#define CLIENT_SERVER   2

typedef struct user_cmd {
        char *user_str;
        short local;
        short stype;
        int user_optcnt;
} USER_CMD;


#define MAX_LINE_LEN        256
#define MAX_PROMPT_LEN      64
#define USER_PROM           "IM_client"

//定义用户命令字段数
#define REG_CMD_FLDS            4
#define LOGIN_CMD_FLDS          3
#define FRIEND_LIST_CMD_FLDS    1
#define FRIEND_ADD_CMD_FLDS     2
#define FRIEND_DEL_CMD_FLDS     2
#define ALL_LIST_CMD_FLDS       1
#define FRIEND_STAT_CMD_FLDS    2
#define LOGOUT_CMD_FLDS         1
#define CHAT_CMD_FLDS           2
#define HELP_CMD_FLDS           1
#define DEBUG_CMD_FLDS          1
#define MESG_CMD_FLDS           3


// 用户命令
#define REG_CMD             "reg"               //注册 reg <username> <password> <password>
#define LOGIN_CMD           "login"             //登录 login <username> <password>
#define FRIEND_LIST_CMD     "friend_list"       //获取好友列表
#define FRIEND_ADD_CMD      "add_friend"        //添加好友
#define FRIEND_DEL_CMD      "del_friend"        //删除好友
#define ALL_LIST_CMD        "alist"             //获取所有在线用户列表
#define FRIEND_STAT_CMD     "stat"              //获取好友状态
#define CHAT_CMD            "talk"              //聊天
#define EXIT_CMD            "exit"              //退出
#define HELP_CMD            "help"              //帮助
#define MESG_CMD             "mesg"             //查询聊天记录
#define DEBUG_CMD           "debug"             //查看当前用户状态

#define REG_C_USAGE         "reg <username> <password> <password> 通过用户名和密码注册用户..\n"
#define LOGIN_C_USAGE       "login <id> <password> 通过id和密码登录..\n"
#define FRIEND_LIST_C_USAGE "friend_list 列出当前用户的所有好友..\n"
#define FRIEND_ADD_C_USAGE  "add <id> 通过用户id添加好友..\n"
#define FRIEND_DEL_C_USAGE  "del <id> 通过用户id删除好友..\n"
#define USER_LIST_C_USAGE   "alist 查询所有的在线用户..\n"
#define FRIEND_STAT_C_USAGE "stat <id> 查询好友状态..\n"
#define CHAT_C_USAGE        "talk <id> <msg> 发送消息给好友..\n"
#define EXIT_C_USAGE        "exit 用户退出..\n"
#define HELP_C_USAGE        "help 打印帮助信息..\n"
#define MESG_C_USAGE        "mesg <from> <to> 显示从用户<from>到用户<to>的聊天记录..\n"
#define DEBUG_C_USAGE       "debug 显示当前用户信息..\n"

#define CLIENT_C_USAGE      "客户端聊天软件命令帮助(<>为必选参数): \n"
#define CLIENT_USAGE            \
        CLIENT_C_USAGE          \
        REG_C_USAGE             \
        LOGIN_C_USAGE           \
        FRIEND_LIST_C_USAGE     \
        FRIEND_ADD_C_USAGE      \
        FRIEND_DEL_C_USAGE      \
        USER_LIST_C_USAGE       \
        FRIEND_STAT_C_USAGE     \
        CHAT_C_USAGE            \
        EXIT_C_USAGE            \
        HELP_C_USAGE            \
        MESG_C_USAGE            \
        DEBUG_C_USAGE           \


#define INV_C_USAGE         "无法解析用户命令\n\n"
#define INV_STAT_USAGE      "尚未登录！\n"


extern USER_CMD all_cmds[];


//命令字段分解函数
// 参数：
// @next_token: 去除命令关键字的命令行字符串指针
// @p: 各个字段的指针数组
// @len: 字段数
// @strict: 是否严格检查格式标志<1|0>
int cmdline_process(char *next_token, char **p, int len, int strict);
//客户端发送命令
int client_send(char *buff, int len);
//处理注册
int reg_cmd_process(char *next_token);
//处理登录
int login_cmd_process(char *next_token);
//消息处理
int mesg_cmd_process(char *next_token);
//处理好友列表
int friend_list_cmd_process(char *next_token);
//添加好友处理
int friend_add_cmd_process(char *next_token);
//删除好友处理
int friend_del_cmd_process(char *next_token);
//查询所有的在线用户
int all_list_cmd_process(char *next_token);
// 处理好友状态查询
int friend_stat_cmd_process(char *next_token);
//处理聊天消息
int chat_cmd_process(char *next_token);
//处理登出
int logout_cmd_process(char *next_token);
//help
int help_cmd_process(char *next_token);
//命令处理函数
int user_cmd_process(char *buff);
//debug
int debug_cmd_process(char *next_token);
//用户状态检查
int user_stat_check(char *token);




#endif //CLIENT_CUSER_CMD_H
