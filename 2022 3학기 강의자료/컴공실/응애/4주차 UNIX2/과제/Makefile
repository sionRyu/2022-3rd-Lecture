CC = gcc
CFLAGS = -g -O3 -w
  
INCLUDES =
OBJS = main.o apps.o io.o

TARGET = s00000

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)
	
