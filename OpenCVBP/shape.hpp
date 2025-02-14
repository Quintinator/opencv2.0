#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

class Shape
{
public:
    Shape();
    virtual ~Shape();
    std::string getShapeName();
    std::string getShapeColor();
    cv::Point getShapePosition();
    long long getShapeClocktickBegin();
    long long getShapeClocktickEnd();

    void setShapeName(std::string shape);
    void setShapeColor(std::string color);
    void setShapePosition(cv::Point position);
    void setClocktickBegin(long long clocktickBegin);
    void setClocktickEnd(long long clocktickEnd);
    void setCorrectShapeAndColor(bool correctShapeAndColor);

    /**
     * @brief Detects and sets the color of the shape based on the average color within its contour.
     *
     * This function calculates the average BGR color of the pixels within the shape's contour,
     * converts this average color from BGR to HSV color space, and then classifies the color
     * into predefined categories. The detected color name is set as the shape's color attribute.
     *
     * @param image The input image from which the shape's color is detected.
     * @param contour The contour defining the shape within the image.
     */
    void detectShapeColor(const cv::Mat &image, const std::vector<cv::Point> &contour);

private:
    /** The geometric shape type (e.g., "circle", "square"). */
    std::string shape;

    /** The color of the shape, classified into predefined categories (e.g., "red", "blue"). */
    std::string color;

    /** The position of the shape in the image, typically represented by the centroid of the shape. */
    cv::Point position;

    /** The timestamp (in CPU clock ticks) when the shape detection process began. */
    long long clocktickBegin;

    /** The timestamp (in CPU clock ticks) when the shape detection process ended. */
    long long clocktickEnd;

    /** Flag indicating whether the detected shape and its color match the specified criteria. */
    bool correctShapeAndColor;

    /**
     * @brief Classifies the given HSV color into a predefined set of color names.
     *
     * Based on the hue and saturation values of the HSV color, this function classifies the color
     * into one of the predefined categories such as "roze", "oranje", "groen", or "geel". If the color
     * does not match any predefined category, it is classified as "Unknown".
     *
     * @param meanHSVColor The mean HSV color obtained from the shape's area.
     * @return std::string The name of the classified color.
     */
    std::string classifyColor(cv::Scalar color);

    /**
     * @brief Calculates the average BGR color of the pixels within a shape's contour.
     *
     * This function computes the centroid of the shape's contour and samples pixels around
     * the centroid within a defined region. It calculates the average BGR color of these
     * sampled pixels. The sampling ensures that the calculated color represents the shape's
     * true color by averaging over a small area.
     *
     * @param image The input image from which the color is sampled.
     * @param contour The contour defining the shape within the image.
     * @return cv::Scalar The average BGR color of the sampled pixels.
     */
    cv::Scalar getShapeColor(const cv::Mat &image, const std::vector<cv::Point> &contour);
};

#endif
