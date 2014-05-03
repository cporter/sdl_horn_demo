
CXXFLAGS=-std=c++1y
LIBS=-lSDL2

beep: beep.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)


