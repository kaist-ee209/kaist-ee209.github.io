CC = gcc209
CFLAGS += -g

STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := .submit
SUBMIT_FILES:= customer_manager1.c customer_manager2.c readme EthicsOath.pdf
SUBMIT := $(STUDENT_ID)_assign3.tar.gz

TARGET := client1 client2

# ONLY FOR TA
ifdef GRADING
	TARGET = client_ta1 client_ta2 \
						stress1 stress2 \
						performance_checker
endif

all: $(TARGET)

client1: client.c customer_manager1.c
	$(CC) $(CFLAGS) -o $@ $^

client2: client.c customer_manager2.c
	$(CC) $(CFLAGS) -o $@ $^

client_ta1: client_ta.c customer_manager1.c
	$(CC) $(CFLAGS) -o $@ $^

client_ta2: client_ta.c customer_manager2.c
	$(CC) $(CFLAGS) -o $@ $^

stress1: stress_test.c customer_manager1.c
	$(CC) $(CFLAGS) -o $@ $^

stress2: stress_test.c customer_manager2.c
	$(CC) -DHASH $(CFLAGS) -o $@ $^

performance_checker: performance_checker.c customer_manager2.c
	$(CC) $(CFLAGS) -o $@ $^

submit:
	mkdir -p $(SUBMIT_DIR)
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean submit
