include buildsystem/func.mk

# Host OS
export HOSTOS ?= $(call TOUPPER,$(call USCORESUB,$(shell uname -s)))

# Verbose Option
ifeq ($(VERBOSE),1)
export Q :=
export VERBOSE := 1
else
export Q := @
export VERBOSE := 0
endif

# Machine Name and Tool Versions
export MACHINE := $(call USCORESUB,$(shell uname -sm))
export CCNAME := $(call USCORESUB,$(notdir $(realpath $(shell which $(CC)))))
export MARCH   := $(shell uname -m)

# Build Directory
export BUILDDIR_ROOT := buildresults
export BUILDDIR := $(BUILDDIR_ROOT)/$(MACHINE)/$(CCNAME)/$(HOSTOS)

# Target Directory
TARGETDIR := targets

# Build Target List
TARGETS := $(basename $(notdir $(wildcard $(TARGETDIR)/*.mk)))

ifeq ($(TARGETMK),)
.DEFAULT_GOAL := help
else
.DEFAULT_GOAL := buildtarget
endif

.PHONY : help
help :
	@echo "usage: make <target[.config]>"
	@echo "       make TARGETMK=<target makefile> [CONFIG=<config>]"
	@echo "       make all"
	@echo "       make clean"
	@echo "other options:"
	@echo "       ISHELL     setting this to 1 enables interactive shell"
	@echo "                  support"
	@echo "       VERBOSE    setting this to 1 enables verbose output"
	@echo "       INSTALL    setting this to 1 runs the install script for"
	@echo "                  each goal specified"
	@echo "       ANALYZE    run static analysis (only works with clang)"
	@echo "       HOSTOS     override compile target supported targets:"
	@echo "                  WIN, LINUX, DARWIN, LIBREELEC"
	@echo "targets:"
	$(call PRINTLIST,$(TARGETS), * )

all : $(TARGETS)

# <target> rule (all configs)
.PHONY : $(TARGETS)
$(TARGETS) :
	$(Q)$(MAKE) -f buildsystem/target.mk \
		TARGETMK="$(TARGETDIR)/$@.mk"

# <target>.<config> rule
$(addsuffix .%,$(TARGETS)) :
	$(Q)$(MAKE) -f buildsystem/target.mk \
		CONFIG=$(call EXTRACT_CONFIG,$@) \
		TARGETMK="$(TARGETDIR)/$(call EXTRACT_TARGET,$@).mk"

# Rule for situations where environment is passed in
.PHONY : buildtarget
buildtarget :
	$(Q)$(MAKE) -f buildsystem/target.mk

### Utility Rules ###
.PHONY : clean
clean :
	$(Q)-rm -rf $(BUILDDIR_ROOT)
	$(call OUTPUTINFO,CLEAN,$(BUILDDIR_ROOT))
