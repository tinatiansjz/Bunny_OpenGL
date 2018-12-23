CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: materials

materials: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./materials $^ $(LIBGL)

# main.o: main.cpp
# 	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o materials