OBJS    := msgqueue.o
CURDIR  := $(ROOT_DIR)/source
OBJS_SQL := $(CURDIR)/park_mysql
MV      := mv

all: $(OBJS) $(OBJS_SQL)
$(OBJS_SQL) : %:%.c
#@echo "make source sql $(OBJS_SQL)"
#@echo "$(CC) $^ $(X_INCDIR) -o $@ $(CURDIR)/$(OBJS) -lmysqlclient"
	$(CC) $^ $(X_INCDIR) -o $@ $(OBJS) -lmysqlclient
	@$(MV) $(OBJS_SQL) $(ROOT_DIR)

$(OBJS) : %.o : %.c
#@echo "make source other"
#@echo "$(CC) -c $^ $(X_INCDIR) -o $@"
	$(CC) -c $^ $(X_INCDIR) -o $@
