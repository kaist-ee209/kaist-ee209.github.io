##
## Makefile for mmr_db project
## 
## Usage:
##   make           (build all targets)
##   make submit    (package submission files)
##   make clean     (remove build artifacts)
##   make release   (package release files)
##

# Compiler and flags
CC ?= gcc209
CFLAGS += -D_GNU_SOURCE

# Directories
SRCDIR := src
TESTDIR := test

# Test files
TEST_FILES := $(TESTDIR)/main.c $(TESTDIR)/test_suite.c

# Default target
TARGET := test_mmr_db

# Implementations
HT_SRC := $(SRCDIR)/mmr_db_ht.c
BST_SRC := $(SRCDIR)/mmr_db_bst.c
INTEGRATED_SRC := $(SRCDIR)/mmr_db_integrated.c

# Default rule: build everything (default to hash table)
all: ht

# Build rules
ht:
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(TARGET) $(HT_SRC) $(TEST_FILES)

bst:
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(TARGET) $(BST_SRC) $(TEST_FILES)

integrated:
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(TARGET) $(INTEGRATED_SRC) $(TEST_FILES)

##
## Submission
##
STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := $(STUDENT_ID)_assign3

SUBMIT_SRC_FILES := \
  $(SRCDIR)/mmr_db.h \
  $(SRCDIR)/mmr_db_ht.c \
  $(SRCDIR)/mmr_db_bst.c \
  $(SRCDIR)/mmr_db_integrated.c \

SUBMIT_FILES := \
  readme \
  EthicsOath.pdf

SUBMIT := $(SUBMIT_DIR).tar.gz

submit:
	mkdir -p $(SUBMIT_DIR)
	mkdir -p $(SUBMIT_DIR)/src
	cp $(SUBMIT_SRC_FILES) $(SUBMIT_DIR)/src
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	tar zcf $(SUBMIT) $(SUBMIT_DIR)
	rm -rf $(SUBMIT_DIR)
	@echo "Created $(SUBMIT) for submission."

##
## Cleanup
##
clean:
	rm -f $(TARGET) *.o
	@echo "Cleaned build artifacts."

##
## Release
##
SKELETON_SRC_FILES := \
  $(SRCDIR)/mmr_db.h \
  $(SRCDIR)/mmr_db_ht.c \
  $(SRCDIR)/mmr_db_bst.c \
  $(SRCDIR)/mmr_db_integrated.c

SKELETON_TEST_FILES := \
  $(TESTDIR)/main.c \
  $(TESTDIR)/test_suite.c \
  $(TESTDIR)/test_suite.h \

SKELETON_FILES := \
  readme \
  STUDENT_ID \
  README.md \
  Makefile

release:
	mkdir -p assignment3
	mkdir -p assignment3/src
	mkdir -p assignment3/test
	cp $(SKELETON_SRC_FILES) assignment3/src
	cp $(SKELETON_TEST_FILES) assignment3/test
	cp $(SKELETON_FILES) assignment3
	tar zcf assignment3.tar.gz assignment3
	rm -rf assignment3
	@echo "Created assignment3.tar.gz for distribution."


.PHONY: all ht bst integrated clean submit