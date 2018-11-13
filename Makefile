OBJ1	= cluster.o
OUT1	= cluster
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1)
cluster.o: cluster.cc
	$(CC) $(FLAGS) cluster.cc
cluster: $(OBJ1)
	$(CC) -o $(OUT1) $(OBJ1)

# clean up
clean:
	rm -rf $(OBJ1) $(OUT1)