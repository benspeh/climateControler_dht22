# Compiler and flags
CXX = g++
CXXFLAGS = -W -Wall -std=c++14 -I../sharedLib/mqtt_cpp/include -I../sharedLib/boost_1_74_0/boost
LDFLAGS = -L../sharedLib -lboost_system -lpthread

# Target executable
TARGET = mqtt_dht22_subscriber

# Source files
SOURCES = dht_subcriberLOOP.cpp climateModel_VPD.cpp

# Build the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)
