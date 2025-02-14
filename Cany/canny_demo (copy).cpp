#include <opencv2/opencv.hpp>

void on_trackbar(int, void*) {
    // This function is called when the trackbar position is changed.
}

int main() {
    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::namedWindow("Filtered Edge Detection", cv::WINDOW_AUTOSIZE);

    // Canny edge detection thresholds
    int lowerThreshold = 150;
    int upperThreshold = 200;

    int lowerHue = 93, upperHue = 144; 
    int lowerSaturation = 14, upperSaturation = 255;
    int lowerValue = 44, upperValue = 255;

    // Create trackbars for Canny thresholds
    cv::createTrackbar("Lower Threshold", "Filtered Edge Detection", &lowerThreshold, 255, on_trackbar);
    cv::createTrackbar("Upper Threshold", "Filtered Edge Detection", &upperThreshold, 255, on_trackbar);

    // Create trackbars for adjusting HSV thresholds
    cv::createTrackbar("Lower Hue", "Filtered Edge Detection", &lowerHue, 179, on_trackbar);
    cv::createTrackbar("Upper Hue", "Filtered Edge Detection", &upperHue, 179, on_trackbar);
    cv::createTrackbar("Lower Saturation", "Filtered Edge Detection", &lowerSaturation, 255, on_trackbar);
    cv::createTrackbar("Upper Saturation", "Filtered Edge Detection", &upperSaturation, 255, on_trackbar);
    cv::createTrackbar("Lower Value", "Filtered Edge Detection", &lowerValue, 255, on_trackbar);
    cv::createTrackbar("Upper Value", "Filtered Edge Detection", &upperValue, 255, on_trackbar);

    while (true) {
        cv::Mat frame, hsv, mask, filtered, detectedEdges;

        cap >> frame;
        if (frame.empty()) break; // End of video stream

        // Convert to HSV color space
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Threshold the HSV image to get only yellow colors
        cv::inRange(hsv, cv::Scalar(lowerHue, lowerSaturation, lowerValue), cv::Scalar(upperHue, upperSaturation, upperValue), mask);

        // Apply the mask to get the yellow parts of the image
        cv::bitwise_and(frame, frame, filtered, mask);

        // Convert filtered image to grayscale
        cv::Mat gray;
        cv::cvtColor(filtered, gray, cv::COLOR_BGR2GRAY);

        // Apply Canny edge detection on the filtered grayscale image
        cv::Canny(gray, detectedEdges, lowerThreshold, upperThreshold, 3);

        // Using Canny's output as a mask, display the result
        cv::Mat dst = cv::Mat::zeros(frame.size(), frame.type());
        frame.copyTo(dst, detectedEdges);

        // Show the result
        cv::imshow("Filtered Edge Detection", dst);

        if (cv::waitKey(30) >= 0) break; // Wait for a keystroke in the window
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
