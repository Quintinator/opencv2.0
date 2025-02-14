#include <opencv2/opencv.hpp>
#include <iostream>

int preset = 0; // Global variable for preset selection

// Simple callback function that does nothing, to avoid the unsafe pointer issue.
void on_trackbar(int, void*) {
}

int main() {
    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::namedWindow("Filtered Edge Detection", cv::WINDOW_AUTOSIZE);

    // Initialize thresholds with default values
    int lowerThreshold = 150, upperThreshold = 200;
    int lowerHue = 93, upperHue = 144; 
    int lowerSaturation = 14, upperSaturation = 255;
    int lowerValue = 44, upperValue = 255;

    // Create trackbars
    cv::createTrackbar("Preset", "Filtered Edge Detection", &preset, 4, on_trackbar);
    cv::createTrackbar("Lower Threshold", "Filtered Edge Detection", &lowerThreshold, 255, NULL);
    cv::createTrackbar("Upper Threshold", "Filtered Edge Detection", &upperThreshold, 255, NULL);
    cv::createTrackbar("Lower Hue", "Filtered Edge Detection", &lowerHue, 179, NULL);
    cv::createTrackbar("Upper Hue", "Filtered Edge Detection", &upperHue, 179, NULL);
    cv::createTrackbar("Lower Saturation", "Filtered Edge Detection", &lowerSaturation, 255, NULL);
    cv::createTrackbar("Upper Saturation", "Filtered Edge Detection", &upperSaturation, 255, NULL);
    cv::createTrackbar("Lower Value", "Filtered Edge Detection", &lowerValue, 255, NULL);
    cv::createTrackbar("Upper Value", "Filtered Edge Detection", &upperValue, 255, NULL);

    // Main loop
    while (true) {
        cv::Mat frame, hsv, mask, filtered, detectedEdges, resized;

        cap >> frame;
        if (frame.empty()) break;

        // Directly handle preset logic here, before processing each frame
        switch (preset) {
            case 1: // "roze"
                lowerHue = 108; upperHue = 170; lowerSaturation = 100; upperSaturation = 255;
                break;
            case 2: // "oranje"
                lowerHue = 0; upperHue = 18; lowerSaturation = 42; upperSaturation = 255;
                break;
            case 3: // "groen"
                lowerHue = 38; upperHue = 110; lowerSaturation = 45; upperSaturation = 255;
                break;
            case 4: // "geel"
                lowerHue = 15; upperHue = 70; lowerSaturation = 0; upperSaturation = 255;
                break;
            default:
                // Default preset values are already set
                break;
        }


        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, cv::Scalar(lowerHue, lowerSaturation, lowerValue), cv::Scalar(upperHue, upperSaturation, upperValue), mask);
        cv::bitwise_and(frame, frame, filtered, mask);

        cv::Mat gray;
        cv::cvtColor(filtered, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, detectedEdges, lowerThreshold, upperThreshold, 3);

        cv::Mat dst = cv::Mat::zeros(frame.size(), frame.type());
        frame.copyTo(dst, detectedEdges);

        // Resize the result before displaying
        cv::resize(dst, resized, cv::Size(dst.cols / 2, dst.rows / 2));

        cv::imshow("Filtered Edge Detection", resized);

        if (cv::waitKey(30) >= 0) break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}

