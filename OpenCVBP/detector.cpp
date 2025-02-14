#include "detector.hpp"

Detector::Detector(){};

Detector::~Detector(){};

void Detector::detectShapes(cv::Mat image)
{
    this->inputImage = image;

    preProcessImage();

    if (shape == "driehoek")
    {
        detectTriangle(color);
    }
    else if (shape == "vierkant")
    {
        detectSquare(color);
    }
    else if (shape == "rechthoek")
    {
        detectRectangle(color);
    }
    else if (shape == "cirkel")
    {
        detectCircle(color);
    }
    else if (shape == "halve cirkel")
    {
        detectHalfCircle(color);
    }

    if (!foundShape)
    {
        if (batchMode)
        {
            std::cout << "No " << shape << " with color " << color << " found"
                      << " - Time: " << (cv::getCPUTickCount() - shapesVector[0].getShapeClocktickBegin()) / cv::getTickFrequency() << " s" << std::endl;
        }
        else
        {
            std::string formattedLabel = "No " + shape + " with color " + color + " found" + " - Time: " + std::to_string((cv::getCPUTickCount() - shapesVector[0].getShapeClocktickBegin()) / cv::getTickFrequency()) + " s";
            cv::putText(inputImage, formattedLabel, cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 1);
        }
    }
}

void Detector::InteractiveMode()
{

    cv::VideoCapture capture(0, cv::CAP_ANY);
    if (!capture.isOpened())
    {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

    std::thread inputThread([this]
                            { this->inputThread(); });

    cv::Mat frame;
    inputThreadRunning = true;

    std::cout << "Enter 'shape color' (e.g., 'Cirkel Groen'), 'stop' to stop detection, or 'exit' to quit:" << std::endl;

    while (inputThreadRunning.load())
    {
        capture >> frame;
        if (frame.empty())
        {
            std::cerr << "Failed to capture image from webcam." << std::endl;
            break;
        }

        foundShape = false;

        if (inputThreadDetect.load())
        {
            detectShapes(frame);
        }

        if (!detectState)
        {
            std::string message = "Detection is not active";
            double fontScale = 1.5;
            unsigned short thickness = 2;
            cv::putText(frame, message, cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(0, 0, 255), thickness);
        }

        cv::resize(frame, frame, cv::Size(), 0.75, 0.75);
        cv::imshow("Webcam", frame);

        if (cv::waitKey(30) >= 0)
            break;
    }

    capture.release();
    cv::destroyAllWindows();

    if (inputThread.joinable())
    {
        inputThread.join();
    }
}

void Detector::BatchMode(std::string ShapeType, std::string ColorType)
{

    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

    batchMode = true;

    cv::Mat frame;
    cap >> frame;

    if (isValidShape(ShapeType) && isValidColor(ColorType))
    {
        this->shape = ShapeType;
        this->color = ColorType;
        detectShapes(frame);
    }
    cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
    cv::imshow("Detected Shapes", frame);
    cv::waitKey(2500);
}

void Detector::inputThread()
{
    std::string input;
    while (inputThreadRunning)
    {
        std::getline(std::cin, input);
        if (input == "stop")
        {
            inputThreadDetect = false;
            std::cout << "Detection stopped" << std::endl;
        }
        else if (input == "exit")
        {
            inputThreadRunning = false;
        }
        else
        {
            std::istringstream iss(input);
            std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

            if (words.size() < 2)
                continue;

            std::string color = words.back();
            words.pop_back();

            std::string shape = std::accumulate(std::next(words.begin()), words.end(), words[0],
                                                [](std::string a, std::string b)
                                                { return std::move(a) + ' ' + std::move(b); });

            std::transform(shape.begin(), shape.end(), shape.begin(), ::tolower);
            std::transform(color.begin(), color.end(), color.begin(), ::tolower);
            if (isValidShape(shape) && isValidColor(color))
            {
                inputThreadDetect = true;
                this->shape = shape;
                this->color = color;
            }
        }
    }
}

void Detector::preProcessImage()
{
    unsigned long long startTick = cv::getCPUTickCount();

    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(93, 14, 44), cv::Scalar(144, 255, 255), mask);

    cv::Mat filteredImage;
    cv::bitwise_and(inputImage, inputImage, filteredImage, mask);

    cv::Mat grayImage;
    cv::cvtColor(filteredImage, grayImage, cv::COLOR_BGR2GRAY);

    cv::Canny(grayImage, cannyOutputImage, 150, 200, 3);

    cv::findContours(cannyOutputImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    shapesVector.clear();
    for (size_t i = 0; i < contours.size(); i++)
    {
        shapesVector.push_back(Shape());
        shapesVector[i].setClocktickBegin(startTick);
    }

    if (contours.size() != shapesVector.size())
    {
        std::cerr << "Error: contours and shapes vector are not the same size" << std::endl;
    }
}

void Detector::labelShape(cv::Mat &image, short ID)
{
    foundShape = true;
    cv::Point position = shapesVector[ID].getShapePosition();

    double time = (shapesVector[ID].getShapeClocktickEnd() - shapesVector[ID].getShapeClocktickBegin()) / cv::getTickFrequency();

    std::string formattedLabel = shapesVector[ID].getShapeName() + " - " + shapesVector[ID].getShapeColor() +
                                 " - Pos: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")" + " - Time: " + std::to_string(time) + " s";

    if (batchMode)
    {
        std::cout << formattedLabel << std::endl;
    }
    else
    {
        unsigned short fontFace = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.35;
        unsigned short thickness = 1;
        cv::Size textSize = cv::getTextSize(formattedLabel, fontFace, fontScale, thickness, 0);
        cv::Point textOrg = cv::Point(position.x - textSize.width / 2, position.y + 0);

        textOrg.x = std::max(0, textOrg.x);
        textOrg.y = std::max(0, textOrg.y);
        textOrg.x = std::min(image.cols - textSize.width, textOrg.x);
        textOrg.y = std::min(image.rows - textSize.height, textOrg.y);

        cv::Mat overlay = image.clone();
        cv::rectangle(overlay, textOrg + cv::Point(0, 0), textOrg + cv::Point(textSize.width, -textSize.height - 0), cv::Scalar(255, 255, 255, 120), cv::FILLED);
        double alpha = 0.4;
        cv::addWeighted(overlay, alpha, image, 1 - alpha, 0, image);

        cv::putText(image, formattedLabel, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
    }
}

void Detector::detectTriangle(std::string ColorType)
{

    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        {
            continue;
        }

        if (approx.size() == 3)
        {
            cv::Rect boundingRect = cv::boundingRect(approx);

            setShape("Driehoek", cv::Point(boundingRect.x + boundingRect.width / 2, boundingRect.y + boundingRect.height / 2), cv::getCPUTickCount(), false, i);
            shapesVector[i].detectShapeColor(inputImage, contours[i]);

            if (shapesVector[i].getShapeColor() == ColorType)
            {
                cv::drawContours(inputImage, contours, (unsigned short)i, cv::Scalar(0, 255, 0), 2);
                labelShape(inputImage, i);
            }
        }
    }
}

void Detector::detectSquare(std::string ColorType)
{
    for (size_t i = 0; i < contours.size(); i++)
    {
        int64 startTick = cv::getCPUTickCount();
        std::vector<cv::Point> approx;

        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        {
            continue;
        }

        if (approx.size() == 4)
        {

            double min_distance = std::numeric_limits<double>::max();
            double max_distance = 0.0;

            for (size_t j = 0; j < approx.size(); j++)
            {
                double distance = cv::norm(approx[j] - approx[(j + 1) % approx.size()]);

                if (distance < min_distance)
                {
                    min_distance = distance;
                }
                if (distance > max_distance)
                {
                    max_distance = distance;
                }
            }

            float ratio = max_distance / min_distance;
            if (ratio <= 1.3 && ratio >= 0.7)
            {
                cv::Rect boundingRect = cv::boundingRect(approx);
                setShape("Vierkant", cv::Point(boundingRect.x + boundingRect.width / 2, boundingRect.y + boundingRect.height / 2), startTick, false, i);
                shapesVector[i].detectShapeColor(inputImage, contours[i]);
                if (shapesVector[i].getShapeColor() == ColorType)
                {
                    cv::drawContours(inputImage, contours, (unsigned short)i, cv::Scalar(0, 255, 0), 2);
                    labelShape(inputImage, i);
                }
            }
        }
    }
}

void Detector::detectRectangle(std::string ColorType)
{
    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        {
            continue;
        }

        cv::Rect boundingRect = cv::boundingRect(approx);
        double aspectRatio = (double)boundingRect.width / boundingRect.height;
        double adjustedAspectRatio = aspectRatio > 1 ? aspectRatio : 1 / aspectRatio;
        if ((approx.size() == 4 && adjustedAspectRatio > 1.1) || adjustedAspectRatio < 0.9)
        {
            setShape("Rechthoek", cv::Point(boundingRect.x + boundingRect.width / 2, boundingRect.y + boundingRect.height / 2), cv::getCPUTickCount(), false, i);
            shapesVector[i].detectShapeColor(inputImage, contours[i]);

            if (shapesVector[i].getShapeColor() == ColorType)
            {
                cv::drawContours(inputImage, contours, (unsigned short)i, cv::Scalar(0, 255, 0), 2);
                labelShape(inputImage, i);
            }
        }
    }
}

void Detector::detectCircle(std::string ColorType)
{

    for (size_t i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        {
            continue;
        }

        cv::Rect boundingRect = cv::boundingRect(approx);
        double aspectRatio = (double)boundingRect.width / boundingRect.height;
        double area = cv::contourArea(contours[i]);
        double arcLength = cv::arcLength(contours[i], true);
        double circularity = 4 * M_PI * area / (arcLength * arcLength);

        if (circularity > 0.8 && std::abs(aspectRatio - 1) < 0.2)
        {
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contours[i], center, radius);

            setShape("Cirkel", cv::Point(center.x, center.y), cv::getCPUTickCount(), false, i);
            shapesVector[i].detectShapeColor(inputImage, contours[i]);

            if (shapesVector[i].getShapeColor() == ColorType)
            {
                cv::circle(inputImage, center, static_cast<unsigned long>(radius), cv::Scalar(0, 255, 0), 2);
                labelShape(inputImage, i);
            }
        }
    }
}

void Detector::detectHalfCircle(std::string ColorType)
{
    for (size_t i = 0; i < contours.size(); i++)
    {

        std::vector<cv::Point> approx;

        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
        {
            continue;
        }

        cv::Rect boundingRect = cv::boundingRect(approx);
        double aspectRatio = (double)boundingRect.width / boundingRect.height;
        double area = cv::contourArea(contours[i]);
        double arcLength = cv::arcLength(contours[i], true);
        double circularity = 4 * M_PI * area / (arcLength * arcLength);

        if (circularity > 0.8 && std::abs(aspectRatio - 1) < 0.2)
        {
            continue;
        }

        if (approx.size() > 4)
        {
            cv::Rect boundingRect = cv::boundingRect(approx);
            setShape("Halve Cirkel", cv::Point(boundingRect.x + boundingRect.width / 2, boundingRect.y + boundingRect.height / 2), cv::getCPUTickCount(), false, i);
            shapesVector[i].detectShapeColor(inputImage, contours[i]);

            if (shapesVector[i].getShapeColor() == ColorType)
            {
                cv::drawContours(inputImage, contours, (unsigned short)i, cv::Scalar(0, 255, 0), 2);
                labelShape(inputImage, i);
            }
        }
    }
}

void Detector::setShape(std::string shape, cv::Point position, long long clocktickEnd, bool correctShapeAndColor, short ID)
{
    shapesVector[ID].setShapeName(shape);
    shapesVector[ID].setShapePosition(position);
    shapesVector[ID].setClocktickEnd(clocktickEnd);
    shapesVector[ID].setCorrectShapeAndColor(correctShapeAndColor);
}

bool Detector::isValidShape(std::string shape)
{
    if (shape != "cirkel" && shape != "halve cirkel" && shape != "vierkant" && shape != "driehoek" && shape != "rechthoek")
    {
        std::cerr << "Invalid shape: " << shape << std::endl;
        detectState = false;
        return false;
    }
    detectState = true;
    return true;
}

bool Detector::isValidColor(std::string color)
{
    if (color != "roze" && color != "groen" && color != "geel" && color != "oranje")
    {
        std::cerr << "Invalid color: " << color << std::endl;
        detectState = false;
        return false;
    }
    detectState = true;
    return true;
}