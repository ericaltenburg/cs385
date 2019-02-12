CXX      = g++ #compiler 
CXX_FILE = $(wildcard *.cpp) #can pick any .cpp file in package
TARGET   = $(patsubst %.cpp,%,$(CXX_FILE)) #turns into a .exe
CXXFLAGS = -g -std=c++11 -Wall -Werror -pedantic-errors -fmessage-length=0 #debugging, version of c++, warn on all, each warning turns into an error, won't let you compile if you have any warnings or error (strict compile), displays nicely

all:
	$(CXX) $(CXXFLAGS) $(CXX_FILE) -o $(TARGET) #make all
clean:
	rm -f $(TARGET) $(TARGET).exe 
