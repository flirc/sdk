include cross.mk

SRC := main.c

TARGET = ir

LIBSRC := main.c 

CFLAGS  += -Wall -g -std=c99 -I. -I../libs/include -Ideps/include 
LDFLAGS += -lusb-1.0 -lflirc -lir

TARGET := $(TARGET)$(SUFFIX)

# Verbose Option
ifeq ($(VERBOSE),1)
export Q :=
export VERBOSE := 1
else
export Q := @
export VERBOSE := 0
endif

# Verbose Option
ifeq ($(DEBUG),1)
CFLAGS += -DCONFIG_DEBUG -DDEBUG_BUILD -DMAX_LOGLEVEL=5 -DDEFAULT_LOGLEVEL=5
endif

OBJDIR = ./objs

OBJ := $(LIBSRC:%.c=$(OBJDIR)/%.o) $(CXXLIBSRC:%.cpp=$(OBJDIR)/%.o)

.DEFAULT_GOAL := $(TARGET)

$(OBJDIR)/%.o : %.c 
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(call OUTPUTINFO,CC,$<)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET) : Makefile $(LIBSRC) $(SRC)
	$(call OUTPUTINFO,LINK,$@)
	$(Q)$(CC) $(SRC) -o $@ -L. $(LDFLAGS) $(LSEARCH) $(CFLAGS)

.PHONY : clean
clean :
	$(call OUTPUTINFO,CLEAN,$(OBJDIR))
	$(call OUTPUTINFO,CLEAN,$(TARGET))
	$(Q)rm -fR $(OBJDIR) lib$(TARGET)* $(TARGET)*

help :
	@echo "usage: make"
	@echo "       make clean"
	@echo "       make $(TARGET)$(SUFFIX)"
	@echo "other options:"
	@echo "       VERBOSE    setting this to 1 enables verbose output"
	@echo "       DEBUG      builds debug version of our executable and lib"
	@echo "       HOST       set this to windows to cross compile"

OUTPUTINFO = @echo "[$1],$2" | \
                awk 'BEGIN { \
                        FS = "[ \t]*,[ \t]*"; \
                        L = 60; \
                } \
                { \
                        s = $$2; \
                        if(length(s) > L) { \
                                s = "... " substr(s,length(s)-L+1,length(s)); \
                        } \
                        printf("%-15s %s\n",$$1,s); \
                }'
