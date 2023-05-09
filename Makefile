CC = gcc
C_FLAGS += -O2 -lpthread
OBJ = src/spammer.c src/log.c
LIB_OPT = -DLOG_USE_COLOR
CRI = podman
CONTAINER_REPO = quay.io/cdonato/spammer

all:
	$(CC) $(C_FLAGS) $(OBJ) $(LIB_OPT) -o spammer

build_container:
	$(CRI) build -t $(CONTAINER_REPO):latest .

push_container: build_container
	$(CRI) push $(CONTAINER_REPO)

clean:
	rm spammer
