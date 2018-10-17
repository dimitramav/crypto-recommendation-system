OBJ2	= lsh_euclidean.o
OBJ1	= main.o
OUT1	= main
OUT2	= lsh_euclidean
CC		= g++
FLAGS	= -c -Wall

all: $(OUT2) $(OUT1)
lsh_euclidean.o: lsh_euclidean.cc
	$(CC) $(FLAGS) lsh_euclidean.cc
main.o: main.cc
	$(CC) $(FLAGS) main.cc
main: $(OBJ1)
	$(CC) $(OBJ1) -o $(OUT1)
lsh_euclidean: $(OBJ2)
	$(CC) $(OBJ2) -o $(OUT2)
# clean up
clean:
	rm -rf $(OBJ1) $(OBJ2) $(OUT1) $(OUT2)