CC          = g++
CCFLAGS     = -Wall -c 
LIBS        = 
INCLUDES    = 
LDFLAGS     = 

MYOBJECTS   = bin-packing.o first-fit.o next-fit.o best-fit.o max-rest.o
BIN         = bin-packing

$(BIN): $(MYOBJECTS) Makefile
	$(CC) $(MYOBJECTS) $(LIBS) $(LDFLAGS)  -o $(BIN)
	./$(BIN)

.cpp.o:
	$(CC) $(INCLUDES) $(CCFLAGS) $<

clean:
	rm -f *.o *.core $(BIN)