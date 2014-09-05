CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++ -g -Wall -c
LDFLAGS=
INC=-I/opt/local/include
SOURCES=Async.cpp main.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=async

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(INC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(INC) $(CFLAGS) $< -o $@

depend: $(SOURCES)
	makedepend $(INC) $^

# DO NOT DELETE THIS LINE -- make depend needs it