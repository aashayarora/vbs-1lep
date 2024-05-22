CXX = g++
CPPFLAGS = -g -Wall -O2

MYINCLUDES = -I include/

ROOTFLAGS = $(shell root-config --libs --cflags)
ROOTFLAGS += -lTMVA -lMinuit -lFoam -lMathMore -lXMLIO

MAKEFLAGS := --jobs=8

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,build/%.o,$(SRCS))

TARGET = runTMVA

BUILD_DIR = build/

all: oogabooga

oogabooga: $(OBJS)
	$(CXX) $(CPPFLAGS) $(MYINCLUDES) $(ROOTFLAGS) $(OBJS) -o bin/$(TARGET)

build/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(MYINCLUDES) $(ROOTFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.DELETE_ON_ERROR:

clean:
	$(RM) $(OBJS) bin/$(TARGET)

clearplots:
	$(RM)

.PHONY: all oogabooga clean clearplots
