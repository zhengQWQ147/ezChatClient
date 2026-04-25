#pragma once

#define SERVER_FORWARD_FLAG (0x1 << 15)

//消息结构体
typedef struct msg_header {
    unsigned short msg_type; //消息类型
    unsigned short msg_len;  //消息长度
    char msg_data[0];        //消息内容（可变长度），虚拟消息内容 !指针!
} MSG_HDR;

//所有的消息类型
#define MSG_REG     0x01    //用户注册消息
#define MSG_LOGIN   0x02    //用户登录消息
#define MSG_FRNDMGT 0x03    //好友管理消息
#define MSG_CHAT    0x04    //聊天消息
#define MSG_LOGOUT  0x05    //登出消息

//好友管理信息子类型
#define F_LREG      0x01    //好友注册
#define F_ADD       0x02    //添加好友
#define F_DEL       0x03    //删除好友
#define F_ALST      0x04    //显示所有用户
#define F_STAT      0x05    //查询好友状态

//注册消息 与 注册回复消息
#define MAX_NAME_LEN        16     //最大用户名长度
#define MAX_PASSWORD_LEN    6      //最大密码长度
//注册消息结构体
typedef struct reg_msg_data {
    char rName[MAX_NAME_LEN];
    char rPasswd[MAX_PASSWORD_LEN];
} REG_MSG;

#define MAX_ERR_LEN 80
//注册回复消息结构体
typedef struct reg_msg_resp {
    //将uid返回客户端，-1表示错误
    int r_id;
    char r_reason[0];   //错误原因，指针
} REG_MSG_RESP;



//登录消息 与 登录回复消息

//登录消息结构体
typedef struct login_msg_data {
    int login_id;
    char login_passwd[MAX_PASSWORD_LEN];
} LOGIN_MSG;

//登录回复消息结构体
#define LOGIN_OK    1   //登录成功
#define LOGIN_ERR   2   //登录失败
typedef struct login_msg_resp {
    int login_stat;         //登录状态 1表示成功 2表示失败
    char login_name[MAX_NAME_LEN];
    char login_reason[0];   //错误原因，指针
} LOGIN_MSG_RESP;


//好友操作 与 好友操作回复

//好友操作结构体
typedef struct friend_op_data {
    int friend_id;      //用户本身
    int friend_fid[0];  //表示需要操作的用户id数组
} FRIEND_OP;

//好友状态
#define USER_ONLINE     1
#define USER_OFFLINE    2

//好友状态结构体
typedef struct friend_stat {
    char friend_name[MAX_NAME_LEN];
    int friend_id;
    int friend_stat;
} FRIEND_STAT;


//好友操作结果
#define OP_ALL_OK   1   //全部成功
#define OP_PART_OK  2   //部分成功
#define OP_ALL_FAIL 3   //全部失败

//好友操作回复结构体
typedef struct friend_op_resp {
    short friend_stat;  //好友在线状态
    short friend_num;   //成功的个数
    FRIEND_STAT friend_ok_friend[0];    //好友状态
} FRIEND_OP_RESP;


//聊天信息 与 聊天反馈消息

//聊天信息结构体
typedef struct chat_msg {
    int chat_sid;   //自身者id
    int chat_rid;   //对方id
    char chat_msg[0]; //聊天内容,指针
} CHAT_MSG;

//聊天消息发送结果
#define SEND_OK     1
#define SEND_FAIL   2

//聊天反馈消息结构体
typedef struct chat_resp {
    int chat_stat;       //返回聊天发送结果
    char chat_reason[0];    //错误原因，指针
} CHAT_RESP;


//登出消息结构体
typedef struct logout_msg {
    int logout_id;                          //登出用户id
    char logout_password[MAX_PASSWORD_LEN]; //登出密码,防止伪造登出
} LOGOUT_MSG;