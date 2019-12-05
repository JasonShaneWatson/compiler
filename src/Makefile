CLASS = 4301
LFLAGS = -lm
CFLAGS = -g -Wall -std=c++11
CXX = g++

BUILD_ROOT=..
SRC_DIR=$(BUILD_ROOT)/src
BUILD_DIR=$(BUILD_ROOT)/build
TEST_DIR=$(BUILD_ROOT)/test2

$(BUILD_DIR)/stage0: stage1.cpp
	$(CXX) -o $(BUILD_DIR)/stage1 $(CFLAGS) stage1.cpp 

clean:
	$(BUILD_ROOT)/zzz_make_clean.sh $(BUILD_DIR)
