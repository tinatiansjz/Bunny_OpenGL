CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: multiple_lights

multiple_lights: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./multiple_lights $^ $(LIBGL)

# main.o: main.cpp
# 	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o multiple_lights