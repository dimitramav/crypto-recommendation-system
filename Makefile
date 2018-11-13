OBJ1	= cluster.o
OBJ2	= datastructs.o
OUT1	= cluster
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1)
datastructs.o: datastructs.cc
	$(CC) $(FLAGS) datastructs.cc
cluster.o: cluster.cc
	$(CC) $(FLAGS) cluster.cc
cluster: $(OBJ1) $(OBJ2) 
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ2) 

# clean up
clean:
	rm -rf $(OBJ1) $(OUT1) $(OBJ2) 