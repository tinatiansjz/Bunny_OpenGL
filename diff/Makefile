CC = g++ -g
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: diff

diff: main.o parser.o parser.h
	$(CC) -o ./diff $^ -I $(INC) $(LIBGL)

# main_bak.o: main_bak.cpp parser.h
# diff.o: diff.cpp parser.h
# parser.o: parser.cpp parser.h
clean:
	rm -f *.o diff
