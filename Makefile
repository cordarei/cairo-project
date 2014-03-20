CXX=clang++
CXXFLAGS+=-std=c++11
CXXFLAGS+=-Wall -Wextra -pedantic
CXXFLAGS+=`pkg-config --cflags sdl2`
LDFLAGS+=`pkg-config --libs sdl2`

run: main.cc sdl.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o run main.cc sdl.cc

clean:
	rm -f run
