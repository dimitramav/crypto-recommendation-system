OBJ1	= lsh.o
OBJ2	= cube.o
OBJ3	= lsh_funcs.o
OBJ4 	= cube_funcs.o
OBJ5	= datastructs.o
OUT1	= lsh
OUT2	= cube
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1) $(OUT2)
datastructs.o: datastructs.cc
	$(CC) $(FLAGS) datastructs.cc
cube_funcs.o: cube_funcs.cc
	$(CC) $(FLAGS) cube_funcs.cc
lsh_funcs.o: lsh_funcs.cc
	$(CC) $(FLAGS) lsh_funcs.cc
lsh.o: lsh.cc
	$(CC) $(FLAGS) lsh.cc
cube.o: cube.cc
	$(CC) $(FLAGS) cube.cc
lsh: $(OBJ1) $(OBJ3) $(OBJ5)
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ3) $(OBJ5)
cube: $(OBJ2) $(OBJ3) $(OBJ4)
	$(CC) -o $(OUT2) $(OBJ2) $(OBJ4) $(OBJ5)

# clean up
clean:
	rm -rf $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OUT1) $(OUT2)