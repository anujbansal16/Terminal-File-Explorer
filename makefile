CC = g++
CFLAGS = -Wall -std=c++0x
DEPS = listDir.h utility.h commandModeOperations.h
OBJ = main.o listDir.o utility.o commandModeOperations.cpp
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

FileExplorer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean
clean :
		-rm *.o $(objects) FileExplorer

