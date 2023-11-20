CC = gcc209
CCFLAGS = -g -O0 -no-pie
LDFLAGS = -no-pie

STUDENT_ID = $(shell cat STUDENT_ID)
SUBMIT_DIR = $(STUDENT_ID)_assign4
SUBMIT_FILES = mydc.s README.md EthicsOath.pdf
SUBMIT = $(STUDENT_ID)_assign4.tar.gz

all: mydc

mydc: mydc.o
	$(CC) $(LDFLAGS) -o $@ $<

mydc.o: mydc.s
	$(CC) -c $(CCFLAGS) -o $@ $<

submit:
	mkdir -p $(SUBMIT_DIR)
	cp -r $(SUBMIT_FILES) $(SUBMIT_DIR)
	tar zcf ./$(SUBMIT) $(SUBMIT_DIR)
	rm -rf $(SUBMIT_DIR)

.PHONY: all clean submit

clean:
	rm -rf mydc mydc.o