OBJ1	= recommendation.o
OBJ2	= datastructs.o
OBJ3	= algorithms.o
OBJ4	= general_functions.o
OUT1	= recommendation
CC		= g++
FLAGS	= -c -std=c++11 -Wall

all: $(OUT1)
algorithms.o: algorithms.cc
	$(CC) $(FLAGS) algorithms.cc
datastructs.o: datastructs.cc
	$(CC) $(FLAGS) datastructs.cc
general_functions.o:general_functions.cc
	$(CC) $(FLAGS) general_functions.cc
recommendation.o: recommendation.cc
	$(CC) $(FLAGS) recommendation.cc
recommendation: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)
	$(CC) -o $(OUT1) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)

# clean up
clean:
	rm -rf $(OBJ1) $(OUT1) $(OBJ2) $(OBJ3) $(OBJ4)