CC = gcc
C_FLAGS += -O2 -lpthread 
OBJ = spammer.c

all:
	$(CC) $(C_FLAGS) $(OBJ) -o spammer

clean:
	rm spammer
