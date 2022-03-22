STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := .submit
SUBMIT_FILES:= mydc.s readme EthicsOath.pdf tests
SUBMIT := $(STUDENT_ID)_assign4.tar.gz

TARGET = dc

all: $(TARGET)

dc: mydc.s
	as --32 -g mydc.s -o dc.o
	gcc209 -g -m32 dc.o -o dc

submit:
	mkdir -p $(SUBMIT_DIR)
	cp -r $(SUBMIT_FILES) $(SUBMIT_DIR)
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean submit
