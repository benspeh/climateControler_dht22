# Makefile for C++ version

# Compiler and flags
CXX = g++
CXXFLAGS = -W -Wall -std=c++11 -lrt

# Target executable
TARGET = dht_readLOOP

# Source files and dependencies
SOURCES = dht_readLOOP.cpp pi_dht_read.cpp bcm2837.cpp realtime.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $^

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS)
