OBJECTS = array.o main.o rangearray.o

CC = g++

TARGET = test

$(TARGET): $(OBJECTS) Array.h RangeArray.h
	$(CC) -o $(TARGET) $(OBJECTS)

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)
