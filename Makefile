TARGET=splashcount
CXX = g++
CXXFLAGS = -std=c++14 -O2 -Iexternal/lammpstrj-parser/include

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

.PHONY: clean

clean:
	rm -f $(TARGET)
