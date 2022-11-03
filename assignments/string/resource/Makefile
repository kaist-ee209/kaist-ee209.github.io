CC = gcc209

STUDENT_ID := $(shell cat STUDENT_ID 2>/dev/null)
SUBMIT_DIR := .submit
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
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean submit
