#include "shape.hpp"

Shape::Shape()
    : shape("Unkown"),
      color("Unkown"),
      position(cv::Point(0, 0)),
      clocktickBegin(0),
      clocktickEnd(0),
      correctShapeAndColor(false)
{
}

Shape::~Shape()
{
}

std::string Shape::getShapeName()
{
    return shape;
}

std::string Shape::getShapeColor()
{
    return color;
}

cv::Point Shape::getShapePosition()
{
    return position;
}

long long Shape::getShapeClocktickBegin()
{
    return clocktickBegin;
}

long long Shape::getShapeClocktickEnd()
{
    return clocktickEnd;
}

void Shape::setShapeName(std::string shape)
{
    this->shape = shape;
}

void Shape::setShapeColor(std::string color)
{
    this->color = color;
}

void Shape::setShapePosition(cv::Point position)
{
    this->position = position;
}

void Shape::setCorrectShapeAndColor(bool correctShapeAndColor)
{
    this->correctShapeAndColor = correctShapeAndColor;
}

void Shape::setClocktickBegin(long long clocktickBegin)
{
    this->clocktickBegin = clocktickBegin;
}

void Shape::setClocktickEnd(long long clocktickEnd)
{
    this->clocktickEnd = clocktickEnd;
}

void Shape::detectShapeColor(const cv::Mat &image, const std::vector<cv::Point> &contour)
{
    cv::Scalar avgBGRColor = getShapeColor(image, contour);
    cv::Mat bgrColor = cv::Mat(1, 1, CV_8UC3, avgBGRColor);
    cv::Mat hsvColor;
    cv::cvtColor(bgrColor, hsvColor, cv::COLOR_BGR2HSV);
    cv::Vec3b hsv = hsvColor.at<cv::Vec3b>(0, 0);
    cv::Scalar meanHSVColor(hsv[0], hsv[1], hsv[2]);
    std::string colorName = classifyColor(meanHSVColor);

    this->color = colorName;
}

std::string Shape::classifyColor(cv::Scalar meanHSVColor)
{
    unsigned short hue = static_cast<unsigned short>(meanHSVColor[0]);
    unsigned short saturation = static_cast<unsigned short>(meanHSVColor[1]);

    if (hue >= 108 && hue <= 170 && saturation >= 100)
    {
        return "roze";
    }
    if (hue > 94 && hue <= 179 && saturation <= 42)
    {
        return "oranje";
    }
    if (hue >= 38 && hue <= 110 && saturation >= 45)
    {
        return "groen";
    }
    if (hue >= 15 && hue <= 100)
    {
        return "geel";
    }

    return "Unknown";
}

cv::Scalar Shape::getShapeColor(const cv::Mat &image, const std::vector<cv::Point> &contour)
{
    cv::Moments m = cv::moments(contour);
    unsigned long centerX = int(m.m10 / m.m00);
    unsigned long centerY = int(m.m01 / m.m00);

    unsigned short sampleSize = 5;
    unsigned short halfSampleSize = sampleSize / 2;

    cv::Rect imageBounds(0, 0, image.cols, image.rows);
    cv::Scalar avgColor(0, 0, 0);
    unsigned short pixelsCounted = 0;

    for (unsigned long y = centerY - halfSampleSize; y <= centerY + halfSampleSize; ++y)
    {
        for (unsigned long x = centerX - halfSampleSize; x <= centerX + halfSampleSize; ++x)
        {
            if (imageBounds.contains(cv::Point(x, y)))
            {
                cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
                avgColor += cv::Scalar(color[0], color[1], color[2]);
                pixelsCounted++;
            }
        }
    }

    if (pixelsCounted > 0)
    {
        avgColor[0] /= pixelsCounted;
        avgColor[1] /= pixelsCounted;
        avgColor[2] /= pixelsCounted;
    }

    return avgColor;
}
