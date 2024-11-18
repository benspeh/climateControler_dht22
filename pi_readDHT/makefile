# Makefile for C++ version

# Compiler and flags
CXX = g++
CXXFLAGS = -W -Wall -std=c++11 -lrt

# Target executable
TARGET = dht_readLOOP

# Source files
SOURCES = dht_readLOOP.cpp pi_readDHT.cpp bcm2837.cpp realtime.cpp

# Build the target executable directly
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -f $(TARGET)
