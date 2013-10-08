CXX = g++
CFLAGS = -Wall
LDFLAGS = -std=c++0x

LIBS = -lopengl32 -lglut32 -lglu32

run: compile
	.\main.exe

compile:
	$(CXX) $(CFLAGS) -o main.exe *.cpp $(LDFLAGS) $(LIBS)