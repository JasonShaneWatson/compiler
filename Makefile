CLASS = 4301
LFLAGS = -lm
CFLAGS = -g -Wall -std=c++11
CXX = g++

BUILD_ROOT=.
SRC_DIR=$(BUILD_ROOT)/src
BUILD_DIR=$(BUILD_ROOT)/build
TEST_DIR=$(BUILD_ROOT)/test2

stage1:
	$(MAKE) --no-print-directory -C $(SRC_DIR)

test: stage1
	cd $(TEST_DIR); ./test.sh
	
clean:
	$(BUILD_ROOT)/zzz_make_clean.sh $(BUILD_DIR)
