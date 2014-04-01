CXX=clang++
CXXFLAGS+=-std=c++11
CXXFLAGS+=-Wall -Wextra -pedantic
CXXFLAGS+=`pkg-config --cflags xcb`
LDFLAGS+=`pkg-config --libs xcb`

run: main.cc sdl.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o run main.cc

clean:
	rm -f run
