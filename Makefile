CC          = g++
CCFLAGS     = -Wall -c -g -O3
LIBS        =
INCLUDES    =
LDFLAGS     =

OBJECTS	    = bin-packing.o first-fit.o next-fit.o best-fit.o max-rest.o simple-heap.o
BIN         = bin-packing

$(BIN): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS)  -o $(BIN)

.cpp.o:
	$(CC) $(INCLUDES) $(CCFLAGS) $<

clean:
	rm -f *.o *.core $(BIN)
