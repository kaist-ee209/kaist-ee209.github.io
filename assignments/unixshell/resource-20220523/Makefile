STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := .submit
SUBMIT_FILES:= *.c *.h Makefile readme EthicsOath.pdf
SUBMIT := $(STUDENT_ID)_assign5.tar.gz

TARGET = ish

# FIX THIS FILE

all: $(TARGET)

submit:
	mkdir -p $(SUBMIT_DIR)
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean submit
