CC = g++
LIBGL = -g -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
# .cpp.o:
# 	echo rebuilding $*.o from $*.cpp
# 	$(CC)  -c $< -I $(INC)

all: coordinate_mass

coordinate_mass: main.o
	$(CC) -o ./coordinate_mass $^  $(LIBGL) -I $(INC)

main.o: main.cpp
	$(CC) -c $< -I $(INC)
clean:
	rm -f *.o coordinate_mass
