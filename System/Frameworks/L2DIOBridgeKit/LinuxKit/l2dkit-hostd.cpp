#include <iostream>

#if defined(__linux__)
    #include <fstream>
    #include <string>
    #include <nlohmann/json.hpp>  
    #include <sys/socket.h>
    #include <unistd.h>
    #include <cstring>
    #include <linux/vm_sockets.h>

    using json = nlohmann::json;

    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }

        std::string value;
        std::getline(file, value);
        return value;
    }

    int createVsockServer(uint32_t port) {
        int sock = socket(AF_VSOCK, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            return -1;
        }

        sockaddr_vm addr{};
        addr.svm_family = AF_VSOCK;
        addr.svm_cid = VMADDR_CID_ANY;  // accept from any guest
        addr.svm_port = port;

        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            close(sock);
            return -1;
        }

        if (listen(sock, 1) < 0) {
            perror("listen");
            close(sock);
            return -1;
        }

        return sock;
    }

    void sendVsock(int clientSock, const std::string& data) {
        write(clientSock, data.c_str(), data.size());
    }

    void reportBattery() {
        json msg;
        msg["type"] = "battery_status";
        msg["capacity"] = readFile("/sys/class/power_supply/BAT0/capacity");
        msg["status"]   = readFile("/sys/class/power_supply/BAT0/status");
        std::string packet = msg.dump();

        sendVsock(packet);
    }

    int main() {
        while (true) {
            reportBattery();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
#else
    int main() {
        std::cerr << "This program is only supported on Linux." << std::endl;
        return 1;
    }
#endif