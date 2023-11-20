CC = gcc209

STUDENT_ID := $(shell cat STUDENT_ID 2>/dev/null)
SUBMIT_DIR := $(STUDENT_ID)_assign2
SUBMIT_FILES:= str.c sgrep.c readme EthicsOath.pdf
SUBMIT := $(STUDENT_ID)_assign2.tar.gz

TARGET := sgrep client

all: $(TARGET)

sgrep: str.o sgrep.o
	$(CC) $(CFLAGS) -o $@ $^

client: str.o client.o
	$(CC) $(CFLAGS) -o $@ $^

submit:
	mkdir -p $(SUBMIT_DIR)
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	tar zcf ./$(SUBMIT) $(SUBMIT_DIR)
	rm -rf $(SUBMIT_DIR)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean submit
