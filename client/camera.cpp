#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0);

    cv::namedWindow("camera", cv::WINDOW_AUTOSIZE);
    cv::Mat frame;
    while (true) {
        cap >> frame;
        cv::imshow("camera", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
}
