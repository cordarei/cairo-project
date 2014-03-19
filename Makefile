CXX=clang++
CXXFLAGS+=-std=c++11
CXXFLAGS+=-Wall -Wextra -pedantic
CXXFLAGS+=`pkg-config --cflags sdl2`
LDFLAGS+=`pkg-config --libs sdl2`

run: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o run main.cc

clean:
	rm -f run
