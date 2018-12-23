CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: color_new

color_new: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./color_new $^ $(LIBGL)

# main.o: main.cpp
# 	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o color_new