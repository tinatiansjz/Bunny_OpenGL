CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: lighting_new

lighting_new: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./lighting_new $^ $(LIBGL)

# main.o: main.cpp
# 	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o lighting_new