CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include

#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: texture texture1 texture2 texture3

texture: main.o $(INC)/Shader.h
	$(CC) -o ./texture $^ $(LIBGL)

texture1: main1.o $(INC)/Shader.h
	$(CC) -o ./texture1 $^ $(LIBGL)

texture2: main2.o $(INC)/Shader.h
	$(CC) -o ./texture2 $^ $(LIBGL)

texture3: main3.o $(INC)/Shader.h
	$(CC) -o ./texture3 $^ $(LIBGL)

clean:
	rm -f *.o texture texture1
