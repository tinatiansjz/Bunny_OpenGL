CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
all: texture

texture: main.o $(INC)/Shader.h
	$(CC) -o ./texture $^ $(LIBGL)

main.o: main.cpp
	$(CC) -I $(INC) -c $< -o $@
clean:
	rm -f *.o texture
