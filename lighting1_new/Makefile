CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
INC = ../Include
SRC = ../src
#pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: lighting1_new lighting1_new1

lighting1_new: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./lighting1_new $^ $(LIBGL)

lighting1_new1: main1.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./lighting1_new1 $^ $(LIBGL)

# main.o: main.cpp
# 	$(CC) -I $(INC) -c $<

clean:
	rm -f *.o lighting1_new lighting1_new1