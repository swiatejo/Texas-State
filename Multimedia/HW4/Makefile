# generic c++ makefile

TARGET		=	quantize#				  executable
SRCS		=	quantize.cpp#			  source files
OBJS	 	= 	$(SRCS:.cpp=.0)# object files

.PHONY:
	all:	$(TARGET)

$(TARGET): 	$(SRCS)
	g++ -o $(TARGET) $(SRCS)


clean:
	@rm $(TARGET)  
