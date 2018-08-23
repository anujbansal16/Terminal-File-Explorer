CC = g++
CFLAGS = -Wall -std=c++0x
DEPS = listDir.h utility.h
OBJ = main.o listDir.o utility.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

FileExplorer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean
clean :
		-rm *.o $(objects) FileExplorer

