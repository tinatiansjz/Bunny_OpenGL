CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
INC = ../Include
all: shader

shader: main.o $(INC)/Shader.h
	$(CC) -o ./shader $^ $(LIBGL)

main.o: main.cpp
	$(CC) -I $(INC) -c $< -o $@
clean:
	rm -f *.o shader
