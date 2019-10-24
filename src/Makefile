CLASS = 4301
LFLAGS = -lm
CFLAGS = -g -Wall -std=c++11
CXX = g++

BUILD_ROOT=..
SRC_DIR=$(BUILD_ROOT)/src
BUILD_DIR=$(BUILD_ROOT)/build
TEST_DIR=$(BUILD_ROOT)/test

$(BUILD_DIR)/stage0: stage0.cpp
	$(CXX) -o $(BUILD_DIR)/stage0 $(CFLAGS) stage0.cpp	

clean:
	rm -f $(BUILD_DIR)/stage0
