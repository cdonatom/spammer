CC = gcc
C_FLAGS += -O2
OBJ = memory_spammer.c

all:
	$(CC) $(C_FLAGS) $(OBJ) -o memory_spammer

clean:
	rm memory_spammer
