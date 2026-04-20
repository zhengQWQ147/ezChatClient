//
// Created by SHZheng_Li on 2026/4/18.
//

#include "client_user.h"
#include "im_client.h"

//socket库初始化
void socklib_init() {
    WSADATA wsaData;
    int ret;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        printf("client: windows socket load failed with error code: %d\n", ret);
        exit(-1);
    }
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

//用户结构初始化
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

//客户端主循环处理函数
void client_main_loop() {

}

//清理函数
void sock_cleanup(int fd) {

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
