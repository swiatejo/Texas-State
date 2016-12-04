#makefile for c

TARGET  =       pairwar#                             executable
SRCS            =       pairwar.c#                         source files
OBJS            =       $(SRCS:.c=.0)# object files

.PHONY:
	 all:    $(TARGET)

$(TARGET):      $(SRCS)
	gcc  $(SRCS) -o $(TARGET) -lpthread


clean:
	 @rm $(TARGET) 

