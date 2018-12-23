CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
# pattern rule
.cpp.o:
	echo rebuilding $*.o from $*.cpp
	$(CC) -I $(INC) -c $<

all: directionalLight pointLight spotLight spotLight_softEdges

directionalLight: main.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./directionalLight $^ $(LIBGL)

pointLight: main1.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./pointLight $^ $(LIBGL)

spotLight: main2.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./spotLight $^ $(LIBGL)

spotLight_softEdges: main3.o $(SRC)/glad.o
	$(CC) -I $(INC) -o ./spotLight_softEdges $^ $(LIBGL)

clean:
	rm -f *.o directionalLight pointLight spotLight spotLight_softEdges
