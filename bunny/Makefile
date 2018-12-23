CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: bunny

bunny: main.o parser.o
	$(CC) -o ./bunny $^ -I $(INC) $(LIBGL)

main.o: main.cpp parser.h
parser.o: parser.cpp parser.h
clean:
	rm -f *.o bunny
