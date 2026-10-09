BIN := web-server 
BUILD_DIR := ./build
SRC_DIRS := ./server

SRCS := $(wildcard server/*.cpp)
# SRCS := $(filter-out server/foo.cpp, $(SRCS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC=clang++
CFLAGS:=$(CFLAGS) -O2 -Wall -Wextra -Iserver/ -MMD -MP -fcommon

$(BUILD_DIR)/$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
