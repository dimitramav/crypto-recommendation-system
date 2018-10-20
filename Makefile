OBJ2	= lsh_euclidean.o
OBJ1	= main.o
OUT1	= main
OUT2	= lsh_euclidean
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1)
lsh_euclidean.o: lsh_euclidean.cc
	$(CC) $(FLAGS) lsh_euclidean.cc
main.o: main.cc
	$(CC) $(FLAGS) main.cc
main: $(OBJ1) $(OBJ2)
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ2)

# clean up
clean:
	rm -rf $(OBJ1) $(OBJ2) $(OUT1)