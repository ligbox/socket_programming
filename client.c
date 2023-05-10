#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};

    // 메시지 입력
    printf("Enter command (R/W) and filename, or a string to convert to ASCII: ");
    fgets(message, BUFFER_SIZE, stdin);
    message[strcspn(message, "\n")] = 0; // 문자열 끝에 개행문자를 제거

    // 1. 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 서버의 IP 주소를 변환
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 4. 서버에 연결 요청
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // 5. 데이터 전송
    send(sock, message, strlen(message), 0);
    printf("Message sent: %s\n", message);

          // 5. 데이터 수신
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("ASCII code received from server: ");
    for (int i = 0; i < valread; i++) {
        printf("%d ", (int)buffer[i]);
    }
    printf("\n");

    // 6. 아스키 코드를 원래 문자로 변환
    for (int i = 0; i < valread; i++) {
        buffer[i] = (char)(buffer[i] - 1);
    }

    printf("Received from server: ");
    for (int i = 0; i < valread; i++) {
        printf("%c", (char)buffer[i]);
    }
    printf("\n");


    // 7. 소켓 닫기
    close(sock);

    return 0;
}