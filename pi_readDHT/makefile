CXX = g++
CXXFLAGS = -W -Wall -std=c++14 -I../sharedLib/mqtt_cpp/include -I../sharedLib/boost_1_86_0
LDFLAGS = -L../sharedLib -lboost_system -lpthread

# Target executable
TARGET = mqtt_dht22_publisher

# Source files
SOURCES = dht_publishLOOP.cpp pi_readDHT.cpp pi_powerDHT.cpp bcm2837.cpp realtime.cpp

# Build the target executable directly
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -f $(TARGET)
