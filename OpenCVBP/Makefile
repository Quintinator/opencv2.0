# Define the C++ compiler
CXX=g++

# Define any compile-time flags
CXXFLAGS=-Wall -Wextra -std=c++17 `pkg-config --cflags opencv4`

# Define any directories containing header files other than /usr/include
# INCLUDES=

# Define library paths in addition to /usr/lib
# LFLAGS=

# Define any libraries to link into executable:
LIBS=`pkg-config --libs opencv4`

# Define the C++ source files
SRCS=main.cpp detector.cpp shape.cpp batchParse.cpp

# Define the C++ object files
OBJS=$(addprefix build/,$(SRCS:.cpp=.o))

# Define the executable file
MAIN=ShapeDetector

# Directory for object files
BUILDDIR=build

# Make flags for verbose output
export MAKEFLAGS += --print-directory

#
# The following part of the Makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file.
#

.PHONY: depend clean cppcheck

all:    $(BUILDDIR) $(MAIN)
	@echo  ShapeDetector has been compiled

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(MAIN): $(OBJS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

cppcheck:
	cppcheck --enable=all --inconclusive --force --inline-suppr --std=c++17 --suppress=missingIncludeSystem $(SRCS)

clean:
	$(RM) -r $(BUILDDIR) *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it