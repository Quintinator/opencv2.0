# Shape/Color Detector using OpenCV

This project is a shape and color detector implemented using OpenCV, a popular computer vision library. The detector is capable of identifying various shapes and colors in images or video streams.

## Features

- Detects shapes such as circles, rectangles, and triangles.
- Identifies colors such as green, yellow or orange
- Provides bounding boxes around detected shapes.
- Outputs the detected shape and color information.

## Eclipse installation
1. In Eclipse, go to File > Import….
2. Expand the C/C++ category and select Existing Code as Makefile Project. Click Next.
3. In the Import Existing Code dialog:
    Project Name: Enter a name for your project.
    Existing Code Location: Browse to the root directory of your existing C++ project that contains the Makefile.
    Toolchain for Indexer Settings: Choose the appropriate toolchain that matches your development environment. If you're unsure, you can select "Linux GCC" for Linux or "MinGW GCC" for Windows with MinGW installed. This setting is used for code indexing and auto-completion features in Eclipse, not for building the project.
4. Click Finish to import the project.

## Alternative installation
1. Run "make" in the CLI

## Usage

1. for interactive mode, just use the command .ShapeDetector
2. for batch mode use ./ShapeDetector batch.txt
3. In interactive mode you can specify the shape and color for example:
    - "Vierkant Geel"
    - "Halve Cirkel Groen"
4. To exit "exit"
5. To stop "stop"

## Available shapes and colors:

colors:
- Groen
- Geel
- Oranje
- Roze

shapes:
- vierkant
- rechthoek
- driehoek
- cirkel
- halve cirkel

## File explanation

1. minimal.supp is used by valgrind
2. batch.txt is an example batch file
