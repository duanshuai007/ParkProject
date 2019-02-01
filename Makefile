CC=gcc
RM=rm

CPATH   := .

OBJS    := source/msgqueue.o
SQL_HEAD_FILE := include/park_mysql.h

ROOT_DIR    :=  $(shell pwd)
TARGET      := debug

NO_MAKE_DIR := include back picture lib

INCDIRS     := include

X_INCDIR    := $(patsubst %, -I $(ROOT_DIR)/%, $(INCDIRS))
NO_MAKE     := $(patsubst %, grep -v % |, $(NO_MAKE_DIR))

SUBDIRS     = $(shell ls -l | grep ^d | awk '{print $$9}' | $(NO_MAKE) tr "\n" " ")

SQL_HEAD_FILE   := $(INCDIRS)/park_mysql.h

export CC X_INCDIR ROOT_DIR SQL_HEAD_FILE

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean

all: $(SUBDIRS)
	@echo "make all"
	$(CC) -o $(TARGET) $(X_INCDIR) main.c $(OBJS) lib/libwty.a -lpthread

subdirs: $(SUBDIRS)
$(SUBDIRS):
	@echo "make dir $@"
	@make -s -C $@

clean:
	@$(RM) -rf $(TARGET) $(OBJS) park_mysql
