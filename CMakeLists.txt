cmake_minimum_required(VERSION 4.2)
project(client C)

set(CMAKE_C_STANDARD 11)

add_executable(client
        msg_type.h
        client_main.c
        list.h
        list.c
        cmsg_encode.c
        cmsg_encode.h
        client_user.c
        client_user.h
        im_client.h
        msg_cipher.h
        msg_cipher.c
        client_config.h
        client_handle.c
        client_handle.h
        client_log.c
        client_log.h
        cmsg_decode.c
        cmsg_decode.h
        cuser_cmd.c
        cuser_cmd.h
)

link_libraries(client, "ws2_32.lib")
