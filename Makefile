
CXXFLAGS=-std=c++1y
LIBS=-lSDL2

beep: beep.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

reader_test: reader_test.o note_reader.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

