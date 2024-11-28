# Makefile for C++ version

# Compiler and flags
CXX = g++
CXXFLAGS = -W -Wall -std=c++14 -I../sharedLib/include
LDFLAGS = -L../sharedLib -lboost -lmqtt_cpp

# Target executable
TARGET = dht_climateModel_subscriber

# Source files
SOURCES = dht_subscriberLOOP.cpp climateModel_VPD.cpp -I./mqtt_cpp/include -I./boost

# Build the target executable directly
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)
