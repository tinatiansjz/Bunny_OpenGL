CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
# .cpp.o:
# 	echo rebuilding $*.o from $*.cpp
# 	$(CC) -I $(INC) -c $<

all: coordinate_depth_new

coordinate_depth_new: main.o $(SRC)/glad.o $(INC)/stb_image.h
	$(CC) -I $(INC) -o ./coordinate_depth_new $^ $(LIBGL)

main.o: main.cpp
	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o coordinate_depth_new