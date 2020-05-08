CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++2a -fPIC -fext-numeric-literals -flto -shared -lffts -Iinclude
CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++2a -fPIC -shared -lffts -Iinclude -ggdb -fno-omit-frame-pointer

all: lib

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: lib

lib:
	@mkdir -p lib
	$(CXX) $(CXX_FLAGS) -o lib/libpitchtracker.so \
		src/pitchtracker.cpp \
		src/audio_stream.cpp 

install: lib
	cp include/pitchtracker.hpp /usr/local/include
	cp lib/libpitchtracker.so /usr/local/lib

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf lib


.PHONY: clean fmt install
