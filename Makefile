CC = gcc
C_FLAGS += -O2 -lpthread 
OBJ = src/spammer.c src/log.c
LIB_OPT = -DLOG_USE_COLOR

all:
	$(CC) $(C_FLAGS) $(OBJ) $(LIB_OPT) -o spammer

clean:
	rm spammer
