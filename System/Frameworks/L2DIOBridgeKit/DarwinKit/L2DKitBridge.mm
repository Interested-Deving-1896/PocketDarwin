#import <Foundation/Foundation.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>

constexpr const char* HOST_IP = "10.0.2.2";
constexpr int HOST_PORT = 5000;

int connectToHost() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(HOST_PORT);

    if (inet_pton(AF_INET, HOST_IP, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

void CrudeLogging() {
    NSData *data = [nsMessage dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];

    if (json && !error) {
        NSLog(@"Parsed JSON: %@", json);
    }
}

void handleMessage(const std::string& message) {
    @autoreleasepool {
        NSString *nsMessage = [NSString stringWithUTF8String:message.c_str()];

        NSLog(@"L2DKit received: %@", nsMessage);

        // Here is where you would:
        // - Parse JSON with NSJSONSerialization
        // - Call IOKit
        // - Update system state
    }
}

void runDaemon() {
    while (true) {
        int sock = connectToHost();
        if (sock < 0) {
            sleep(3);
            continue;
        }

        char buffer[1024];

        while (true) {
            ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) {
                close(sock);
                break;
            }

            buffer[bytes] = '\0';
            handleMessage(std::string(buffer));
        }
    }
}

int main() {
    @autoreleasepool {
        runDaemon();
    }
    return 0;
}
