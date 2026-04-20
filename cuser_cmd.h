//
// Created by SHZheng_Li on 2026/4/20.
//

#ifndef CLIENT_CUSER_CMD_H
#define CLIENT_CUSER_CMD_H

// 用户命令
#define REG_CMD             "reg"               //注册reg <username> <password> <password>
#define LOGIN_CMD           "login"             //登录
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

//命令处理函数
int user_cmd_process(char *buff);
//用户状态检查
int user_stat_check(char *token);
//处理注册
int reg_cmd_process(char *next_token);
#endif //CLIENT_CUSER_CMD_H
