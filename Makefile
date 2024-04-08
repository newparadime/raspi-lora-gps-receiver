PROJECT_ROOT=$(shell pwd)
BINARY_NAME=$(notdir $(PROJECT_ROOT))

OBJECT_DIR=obj
SOURCE_DIR=src
BINARY_DIR=bin

SOURCE_FILES:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJECT_FILES=$(SOURCE_FILES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)

TRIPLET?=arm-linux-gnueabihf
PREFIX=/usr/local/$(TRIPLET)
INCLUDE_DIRS=$(PREFIX)/include
LIBRARY_DIRS=$(PREFIX)/lib

CXX:=$(TRIPLET)-$(CXX)
CC:=$(TRIPLET)-$(CC)
LD:=$(TRIPLET)-$(LD)
CXXFLAGS+=$(INCLUDE_DIRS:%=-I%) -Wall -Werror -std=gnu++14 -O1 -fPIC
LDFLAGS=$(LIBRARY_DIRS:%=-L%)
LDLIBS=-llora -lwiringPi

.PHONY: default all clean

default: all

all: $(BINARY_DIR)/$(BINARY_NAME)

clean:
	$(RM) $(OBJECT_FILES) $(BINARY_DIR)/$(BINARY_NAME)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BINARY_DIR)/$(BINARY_NAME): $(OBJECT_FILES)
	mkdir -p $(@D)
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@
