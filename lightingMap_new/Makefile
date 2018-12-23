CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
# pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: diffuseMap specularMap emissionMap

diffuseMap: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./diffuseMap $^ $(LIBGL)

specularMap: main1.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./specularMap $^ $(LIBGL)

emissionMap: main2.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./emissionMap $^ $(LIBGL)

clean:
	rm -f *.o diffuseMap specularMap emissionMap
