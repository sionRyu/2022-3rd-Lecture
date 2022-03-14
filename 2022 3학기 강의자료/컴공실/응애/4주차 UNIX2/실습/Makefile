CC = gcc
CFLAGS = -g
  
INCLUDES =
SRCS = main.cpp Output.cpp String_Manipulation.cpp
OBJS = $(SRCS:.c=.o)

TARGET = fmt

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o $(TARGET)
	
