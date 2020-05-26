CXX		?= g++
CC              ?= gcc
CXX_FLAGS 	:= -ansi -pedantic -fPIC -Wall -O3 -std=c++2a -fext-numeric-literals -flto -lffts -Iinclude -t -L /usr/local/lib/libffts.a

CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++2a  -lffts -Iinclude -ggdb -fno-omit-frame-pointer

all: pitch-tracker

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: pitch-tracker

pitch-tracker:
	$(CXX) $(CXX_FLAGS) -o  $@ \
		src/pitchtracker.cpp \
		include/RtAudio.cpp

clean:
	-rm pitch-tracker

.PHONY: clean
