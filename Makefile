CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++2a -fPIC -fext-numeric-literals -flto -shared -lffts -Iinclude
CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++2a -fPIC -shared -lffts -Iinclude -ggdb -fno-omit-frame-pointer

all: pitch-tracker

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: pitch-tracker

pitch-tracker:
	$(CXX) $(CXX_FLAGS) -o  $@ \
		src/pitchtracker.cpp 

clean:
	-rm pitch-tracker


.PHONY: clean
