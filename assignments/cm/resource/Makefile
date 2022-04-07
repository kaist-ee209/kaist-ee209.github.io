CC ?= gcc209
CFLAGS += -g

STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := .submit
SUBMIT_FILES:= customer_manager1.c customer_manager2.c customer_manager1_b.c readme EthicsOath.pdf
SUBMIT := $(STUDENT_ID)_assign3.tar.gz

TARGET := client1 client2 client1_b example

all: $(TARGET)

client1: client.c customer_manager1.c
	$(CC) $(CFLAGS) -o $@ $^

client1_b: client.c customer_manager1_b.c countable_bloomfilter.c murmurhash.c
	$(CC) $(CFLAGS) -o $@ $^

client2: client.c customer_manager2.c
	$(CC) $(CFLAGS) -o $@ $^

example: example.c countable_bloomfilter.c  murmurhash.c 
	$(CC) -o $@ -g -Wall -Werror -Wno-unused-variable -pedantic -std=c99 $^

submit:
	mkdir -p $(SUBMIT_DIR)
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean submit
