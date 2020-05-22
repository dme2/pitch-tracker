CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++2a -fPIC -fext-numeric-literals -flto -shared -lffts -Iinclude
CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++2a -fPIC -shared -lffts -Iinclude -ggdb -fno-omit-frame-pointer

all: pitch-tracker tests

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: pitch-tracker

tests:
	$(CXX) $(CXX_FLAGS) -o buffer_test \ 
		tests/buffer_test.cpp

pitch-tracker:
	$(CXX) $(CXX_FLAGS) -o  $@ \
		src/pitchtracker.cpp 

clean:
	-rm pitch-tracker buffer-test


.PHONY: clean
