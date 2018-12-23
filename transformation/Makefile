CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include

#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: transformation transformation1 transformation2

transformation: main.o $(INC)/Shader.h
	$(CC) -o ./transformation $^ $(LIBGL)

transformation1: main1.o $(INC)/Shader.h
	$(CC) -o ./transformation1 $^ $(LIBGL)

transformation2: main2.o $(INC)/Shader.h
	$(CC) -o ./transformation2 $^ $(LIBGL)

# transformation3: main3.o $(INC)/Shader.h
# 	$(CC) -o ./transformation3 $^ $(LIBGL)

clean:
	rm -f *.o transformation transformation1 transformation2
