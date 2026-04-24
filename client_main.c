//
// Created by SHZheng_Li on 2026/4/18.
//

#include "client_user.h"
#include "im_client.h"
#include "cmsg_decode.h"
#include "cuser_cmd.h"

//socket库初始化
void socklib_init() {
    WSADATA wsaData;
    int ret;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        printf("client: windows socket load failed with error code: %d\n", ret);
        exit(-1);
    }
}

//清理函数
void sock_cleanup(int fd) {
    closesocket(fd);
    WSACleanup();
}

//客户端socket初始化
int client_socket_init(char *addr) {
    int socket_fd;
    struct sockaddr_in server;

    //socket初始化
    socklib_init();
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("client: socket error\n");
        return -1;
    }
    memset(&server, 0 , sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, addr, &server.sin_addr) <= 0) {
        printf("client: inet_pton error for %s\n",addr);
        closesocket(socket_fd);
        return -2;
    }

    //连接服务器
     if (connect(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
         printf("client: connect error!\n");
         return -3;
     }

    return 0;
}

//参数初始化
int init_args(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPV4 server address>\n", argv[0]);
        return -1;
    }

    //初始化client socket描述符
    user_self.self_socket_fd = client_socket_init(argv[1]);
    if (user_self.self_socket_fd < 0) {
        return -1;
    }

    return 0;
}

//子线程
void *client_cli_thread(void *arg) {
    char line[MAX_LINE_LEN];
    char user_prompt[MAX_PROMPT_LEN];

    while (1) {
        if (user_self.self_id != -1) {_snprintf(user_prompt, MAX_PROMPT_LEN, "%s <%s> <cmd:>",USER_PROM, user_self.self_name);}    // IM_client <name> <cmd:>
        else {_snprintf(user_prompt, MAX_PROMPT_LEN, "%s <unknown> <cmd:>",USER_PROM);}                    // IM_client <unknown> <cmd:>

        printf("%s", user_prompt);

        //读取用户输入
        memset(line, 0, MAX_LINE_LEN);
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        if (line[strlen(line) -1] == '\n') line[strlen(line) -1] = '\0';
        //-2为退出命令
        if (user_cmd_process(line) == -2) {
            printf("client: user logout...\n");
            break;
        }
    }

    closesocket(user_self.self_socket_fd);
    return NULL;
}

//客户端主循环处理函数
void client_main_loop() {
    pthread_t client_stdin_pid;
    pthread_create(&client_stdin_pid, NULL, client_cli_thread, NULL);
    int len;
    while ((len = recv(user_self.self_socket_fd, user_self.self_buff, MAX_MSG_SIZE, 0))> 0) {
        printf("client: recv msg len = %d",len);
        decode_server_resp(user_self.self_buff, len);
    }

    printf("client: recv len < 0, exiting...\n");
}

//主函数
// 参数：
// IPV4地址
int main(int argc, char *argv[]) {
    //用户结构初始化
    init_user_struct(&user_self);
    //参数初始化
    if (init_args(argc, argv));
    //客户端主循环处理函数, 主线程
    client_main_loop();
    //清理函数
    sock_cleanup(user_self.self_socket_fd);
}
