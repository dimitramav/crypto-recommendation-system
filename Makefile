OBJ1	= cluster.o
OBJ2	= datastructs.o
OBJ3	= algorithms.o
OUT1	= cluster
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1)
algorithms.o: algorithms.cc
	$(CC) $(FLAGS) algorithms.cc
datastructs.o: datastructs.cc
	$(CC) $(FLAGS) datastructs.cc
cluster.o: cluster.cc
	$(CC) $(FLAGS) cluster.cc
cluster: $(OBJ1) $(OBJ2) $(OBJ3)
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ2) $(OBJ3)

# clean up
clean:
	rm -rf $(OBJ1) $(OUT1) $(OBJ2) $(OBJ3)