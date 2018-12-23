CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
# .cpp.o:
# 	echo rebuilding $*.o from $*.cpp
# 	$(CC) -I $(INC) -c $<

all: texture_new

texture_new: main.o $(SRC)/glad.o
	$(CC) -o ./texture_new $^ $(LIBGL)

main.o: main.cpp
	$(CC) -I $(INC) -c $<

clean:
	rm -f *_new.o texture_new
