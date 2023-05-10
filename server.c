#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    // 1. 클라이언트로부터 데이터 수신
    valread = read(sock, buffer, BUFFER_SIZE);

    printf("Received from client: %s\n", buffer);

    // 2. 아스키 코드로 변환
    printf("Converts the string received from the client into ASCII code: ");
    for (int i = 0; i < valread; i++) {
        buffer[i] = (char)(buffer[i] + 1);
        printf("%d ", (int)buffer[i]);
    }
    printf("\n");

    // 3. 변환된 데이터를 클라이언트에게 전송
    printf("ASCII codes sent to client\n");
    send(sock, buffer, valread, 0);
}




int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 소켓 옵션 설정
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // 3. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 5. listen
    listen(server_fd, 3);

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // 6. 클라이언트 연결 요청 수락
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        // 클라이언트 처리
        handle_client(new_socket);
    }

    return 0;
}