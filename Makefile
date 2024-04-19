CXX := g++
CXXFLAGS := -Wall
LDFLAGS := -s

# Lista plików źródłowych i docelowych
SOURCES := fib_heap.cpp
EXECUTABLES := fib_heap.x

# Pliki obiektowe
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all clean make

all: $(EXECUTABLES)

fib_heap.x: fib_heap.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.x: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

make: $(OBJECTS) $(EXECUTABLES)

clean:
	$(RM) $(EXECUTABLES) $(OBJECTS)