#ifndef DETECTOR_H
#define DETECTOR_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <atomic>
#include <thread>
#include <numeric>
#include <iterator>

#include <opencv2/opencv.hpp>
#include "shape.hpp"

/**
 * @class Detector
 * @brief Detects geometric shapes in images.
 *
 * Detector class provides functionalities to detect predefined geometric shapes
 * such as circles, squares, rectangles, triangles, and half-circles in given images.
 * It uses OpenCV functions for image processing and shape detection.
 */

class Detector
{
public:
    Detector();
    virtual ~Detector();

    /**
     * @brief Detects shapes in a given image based on specified shape and color criteria.
     *
     * The method processes the input image to detect predefined shapes (e.g., triangles, squares, etc.)
     * based on the internal state of the `shape` and `color` attributes. The detection algorithm
     * involves preprocessing the image, applying contour detection, and then identifying shapes
     * by analyzing the contours' geometry.
     *
     * @param image The input image in which to detect shapes.
     */
    void detectShapes(cv::Mat image);

    /**
     * @brief Initiates interactive mode for real-time shape detection from the webcam.
     *
     * In interactive mode, the user can input commands to specify the shape and color
     * of interest. The program captures video frames from the default camera and processes
     * them in real-time to detect the specified shapes. Results are displayed on the screen.
     */
    void InteractiveMode();

    /**
     * @brief Executes batch mode for detecting a specific shape and color in a single frame from the webcam.
     *
     * Batch mode is designed for processing a single image/frame to detect a specific shape and color,
     * as specified by the user. It captures a frame, processes it, and displays the result.
     *
     * @param ShapeType The shape to detect in the captured frame.
     * @param ColorType The color of the shape to detect.
     */
    void BatchMode(std::string ShapeType, std::string ColorType);

private:
    /**
     * @brief Handles user input in interactive mode.
     *
     * Continuously reads user input from the terminal and updates the detection
     * criteria based on the input. Supports commands to start/stop detection and
     * exit the program.
     */

    void inputThread();

    /**
     * @brief Processes the input image to prepare it for shape detection.
     *
     * Preprocessing includes converting the image to HSV color space, applying a mask
     * to isolate the color of interest, converting to grayscale, and finally applying
     * Canny edge detection. The result is used to identify contours that are analyzed
     * for shape detection.
     */
    void preProcessImage();

    /**
     * @brief Labels a detected shape on the image or terminal based on the operating mode.
     *
     * In interactive mode, draws the label directly on the image. In batch mode,
     * outputs the label information to the terminal. The label includes the shape's name,
     * color, position, and detection time.
     *
     * @param image Reference to the image where the label will be drawn (interactive mode).
     * @param ID The index of the shape in the `shapesVector`.
     */
    void labelShape(cv::Mat &image, short ID);

    /**
     * @brief Detects triangles in the input image based on contour approximation.
     *
     * This function iterates through detected contours in the input image, approximating each contour to a polygon.
     * If the approximated polygon has exactly three vertices and passes the convexity and area threshold checks,
     * it is identified as a triangle. The function then labels the detected triangle based on the specified color.
     * The computational intensity of this operation depends on the number of contours and the complexity of their shapes.
     *
     * @param ColorType The color filter to apply when detecting triangles. Only triangles matching this color are labeled.
     */
    void detectTriangle(std::string ColorType);

    /**
     * @fn void Detector::detectSquare(std::string ColorType)
     * @brief Detects squares in the input image by analyzing contour approximations.
     *
     * Similar to triangle detection, this function approximates contours to polygons. A polygon is identified as a square
     * if it has four vertices and the distances between consecutive vertices are relatively equal (adjustable through a ratio threshold).
     * This method involves calculating distances between vertices and checking for uniformity to identify squares.
     * Computational intensity is influenced by contour complexity and the accuracy threshold for square identification.
     *
     * @param ColorType The color filter to apply when detecting squares. Only squares matching this color are labeled.
     */
    void detectSquare(std::string ColorType);

    /**
     * @fn void Detector::detectRectangle(std::string ColorType)
     * @brief Detects rectangles by evaluating the aspect ratio and contour approximation.
     *
     * This function processes contours similarly to square detection but uses the aspect ratio of the bounding box
     * of the approximated polygon to distinguish rectangles from squares. Rectangles have an aspect ratio significantly
     * different from 1. The computational load is comparable to that of square detection, with additional checks for aspect ratio.
     *
     * @param ColorType The color filter to apply when detecting rectangles. Only rectangles matching this color are labeled.
     */
    void detectRectangle(std::string ColorType);

    /**
     * @fn void Detector::detectCircle(std::string ColorType)
     * @brief Detects circles based on circularity and aspect ratio criteria.
     *
     * Circles are detected by approximating contours and calculating each contour's circularity and aspect ratio.
     * A high circularity value and an aspect ratio close to 1 indicate a circle. Circularity is calculated using the
     * contour area and perimeter, making this method computationally more intensive due to the need for precise area
     * and perimeter calculations.
     *
     * @param ColorType The color filter to apply when detecting circles. Only circles matching this color are labeled.
     */
    void detectCircle(std::string ColorType);

    /**
     * @fn void Detector::detectHalfCircle(std::string ColorType)
     * @brief Detects half-circles by evaluating contour properties and excluding full circles.
     *
     * Half-circle detection is a more complex process that first identifies potential circles (or partial circles)
     * based on contour approximation and then filters out full circles by checking the contour's properties, such as
     * circularity and aspect ratio. Given the additional steps to distinguish half-circles from full circles, this function
     * can be more computationally intensive than detecting full circles.
     *
     * @param ColorType The color filter to apply when detecting half-circles. Only half-circles matching this color are labeled.
     */
    void detectHalfCircle(std::string ColorType);

    /**
     * @brief Sets the attributes of a detected shape.
     *
     * Populates the `Shape` object at the specified index in `shapesVector` with the shape's
     * name, position, detection end time, and whether the shape and color match the criteria.
     *
     * @param shape Name of the detected shape.
     * @param position Position of the shape in the image.
     * @param clocktickEnd CPU tick count at the end of detection.
     * @param correctShapeAndColor Flag indicating if the detected shape matches the search criteria.
     * @param ID Index of the shape in the `shapesVector`.
     */

    void setShape(std::string shape, cv::Point position, long long clocktickEnd, bool correctShapeAndColor, short ID);

    /**
     * @brief Checks if the specified shape is one of the predefined valid shapes.
     *
     * @param shape Name of the shape to validate.
     * @return True if the shape is valid, otherwise false.
     */
    bool isValidShape(std::string shape);

    /**
     * @brief Checks if the specified color is one of the predefined valid colors.
     *
     * @param color Name of the color to validate.
     * @return True if the color is valid, otherwise false.
     */
    bool isValidColor(std::string color);

    /** Stores detected shapes in the current frame. */
    std::vector<Shape> shapesVector;

    /** Indicates whether the detector is operating in batch mode. */
    bool batchMode = false;

    /** Holds the contours found in the input image for shape detection. */
    std::vector<std::vector<cv::Point>> contours;

    /** The result of applying Canny edge detection to the input image. */
    cv::Mat cannyOutputImage;

    /** The current image being processed by the detector. */
    cv::Mat inputImage;

    /** An optional debug image to visualize the effect of Canny edge detection. */
    cv::Mat cannyDebug;

    /** Flag indicating if the specified shape and color were found in the current frame. */
    bool foundShape = false;

    /** Indicates if the detector is actively looking for shapes. */
    bool detectState = false;

    /** Atomic flag controlling the state of the input handling thread. */
    std::atomic<bool> inputThreadRunning = true;

    /** Atomic flag indicating whether shape detection should be performed in the input thread. */
    std::atomic<bool> inputThreadDetect = false;

    /** The name of the shape to detect, as specified by the user. */
    std::string shape;

    /** The target color for shape detection, as specified by the user. */
    std::string color;
};

#endif