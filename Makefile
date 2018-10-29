OBJ4 	= cube.o
OBJ3	= helper.o
OBJ2	= datastructs.o
OBJ1	= lsh.o
OUT1	= lsh
OUT2	= cube
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1) $(OUT2)
datastructs.o: datastructs.cc
	$(CC) $(FLAGS) datastructs.cc
helper.o: helper.cc
	$(CC) $(FLAGS) helper.cc
lsh.o: lsh.cc
	$(CC) $(FLAGS) lsh.cc
cube.o: cube.cc
	$(CC) $(FLAGS) cube.cc
lsh: $(OBJ1) $(OBJ2) $(OBJ3)
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ2) $(OBJ3)
cube: $(OBJ2) $(OBJ3) $(OBJ4)
	$(CC) -o $(OUT2) $(OBJ2) $(OBJ3) $(OBJ4)

# clean up
clean:
	rm -rf $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OUT1) $(OUT2)