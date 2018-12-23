CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: camera camera1 camera2 camera3 camera4 camera5

camera: main.o
	$(CC) -o ./camera $^ -I $(INC) $(LIBGL)

camera1: main1.o
	$(CC) -o ./camera1 $^ -I $(INC) $(LIBGL)

camera2: main2.o
	$(CC) -o ./camera2 $^ -I $(INC) $(LIBGL)

camera3: main3.o
	$(CC) -o ./camera3 $^ -I $(INC) $(LIBGL)

camera4: main4.o
	$(CC) -o ./camera4 $^ -I $(INC) $(LIBGL)

camera5: main5.o
	$(CC) -o ./camera5 $^ -I $(INC) $(LIBGL)

clean:
	rm -f *.o camera camera1 camera2 camera3 camera4 camera5
