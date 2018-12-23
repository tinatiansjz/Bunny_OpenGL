CC = g++
LIBGL = -lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lSOIL
INC = ../Include
SRC = ../src
#pattern rule
# .cpp.o:
# 	echo rebuilding $*.o from $*.cpp
# 	$(CC) -I $(INC) -c $<

all: window_new

window_new: window_new.o $(SRC)/glad.o
	$(CC) -o ./window_new $^ $(LIBGL)

window_new.o: window_new.cpp
	$(CC) -I $(INC) -c $<
$(SRC)/glad.o: $(SRC)/glad.c
	$(CC) -I $(INC) -c $<
clean:
	rm -f *.o window_new
