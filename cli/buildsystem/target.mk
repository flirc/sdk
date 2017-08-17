include buildsystem/func.mk

ifeq ($(TARGETMK),)
$(error TARGETMK not defined)
endif

TARGET ?= $(basename $(notdir $(TARGETMK)))

ifeq ($(CONFIG),)
.DEFAULT_GOAL := all
else
.DEFAULT_GOAL := $(CONFIG)
endif

include $(TARGETMK)

GOAL :=
ifeq ($(INSTALL),1)
GOAL := install
endif

# Make sure there is at least one config
ifeq ($(words $(CONFIGS)),0)
$(error Must specify at least one config in target makefile)
endif

.PHONY : all
all : $(CONFIGS)

.PHONY : $(CONFIGS)
$(CONFIGS) :
	$(call OUTPUTINFO,BUILDING,$(TARGET).$@)
	$(Q)$(MAKE) -f buildsystem/main.mk \
		TARGETMK="$(TARGETMK)" \
		TARGET=$(TARGET) \
		CONFIG=$@ \
		$(GOAL)
	$(call OUTPUTINFO,DONE,$(TARGET).$@)
	@echo
