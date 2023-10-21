include buildsystem/func.mk

# Set default flags
CPPFLAGS := -Iinclude -Isrc -I../libs/
CFLAGS := -Wall -Werror 

ifeq ($(HOSTOS),win)
CC		:= i686-w64-mingw32.static-gcc
CCP		:= i686-w64-mingw32.static-g++
WINRES		:= i686-w64-mingw32.static-windres
MAKENSIS	:= i686-w64-mingw32.static-makensis
CXXFLAGS 	+= -D__HOST_WIN__
CFLAGS		+= -D__HOST_WIN__
LIBS_STATIC	+= hidapi usb-1.0
LIBS_STATIC	+= hid setupapi
LSEARCH		:= -L../libs/Win/

# Fixes packing issue
CFLAGS		+= -mno-ms-bitfields
endif

CXXFLAGS = $(CFLAGS)
ASFLAGS :=
LDFLAGS :=
OPTIONS :=
ANALYZE_FLAGS :=	-analyzer-viz-egraph-graphviz \
			-analyzer-output=html \
			-analyzer-opt-analyze-headers \
			-analyzer-checker=core \
			-analyzer-checker=unix \
			-analyzer-checker=alpha.unix \
			-analyzer-checker=security \
			-analyzer-checker=alpha.security \

ifeq ($(TARGET),)
$(error no TARGET defined)
endif
ifeq ($(CONFIG),)
$(error no CONFIG defined)
endif
ifeq ($(TARGETMK),)
$(error no TARGETMK defined)
endif
include $(TARGETMK)

ifeq ($(HOSTOS),LIBREELEC)
MACHINE := Linux_aarch64
endif

# Add all the libraries defined in config.mk to LDLIBS
LDLIBS := $(addprefix -l,$(LIBRARIES))
ifeq ($(HOSTOS), win)
LSEARCH := -L../libs/Win/
LSEARCH += -L../../build/mxe/usr/i686-w64-mingw32.static/lib/
endif

# Add all the frameworks defined in config.mk to LDFLAGS (This is only for OSX)
ifeq ($(HOSTOS),DARWIN)
LSEARCH := -L../libs/$(MACHINE)/
LDFLAGS += $(addprefix -framework ,$(FRAMEWORKS))
endif

# Raspberry Pi Build
ifeq ($(MACHINE),Linux_aarch64)
ifeq (64, $(shell getconf LONG_BIT))
LSEARCH := -L../libs/Pi_64
else
LSEARCH := -L../libs/Pi_32
endif
endif

ifneq ($(HOSTOS),LIBREELEC)
LDFLAGS += -L/usr/local/lib
endif

# Build Directory
BUILDDIR := $(BUILDDIR)/$(TARGET)/$(CONFIG)

# Add in the target name and host OS
OPTIONS += __TARGET__='"$(TARGET)"' __CONFIG__='"$(CONFIG)"'
OPTIONS += __TARGET_$(call TOUPPER,$(TARGET))__
OPTIONS += __CONFIG_$(call TOUPPER,$(CONFIG))__
OPTIONS += __HOST_$(HOSTOS)__

# Add in the options
CPPFLAGS += $(addprefix -D,$(OPTIONS))

# Include the dependencies
sinclude $(addprefix $(BUILDDIR)/,$(call CONVERTEXT, d, $(SOURCES)))

### Main Rule ###
TARGET_TYPE := $(strip $(TARGET_TYPE))
.DEFAULT_GOAL := $(BUILDDIR)/$(TARGET)$(TARGET_TYPE)
$(BUILDDIR)/$(TARGET)$(TARGET_TYPE) : \
		$(addprefix $(BUILDDIR)/,$(call CONVERTEXT, o, $(SOURCES)))

# Standard type targets
$(BUILDDIR)/$(TARGET) :
	$(call OUTPUTINFO,LINK,$@)
	@mkdir -p $(@D)
	$(Q)$(CC) $(LDFLAGS) $^ $(LSEARCH) $(LDLIBS) -o $@
ifeq ($(ANALYZE),1)
	$(call OUTPUTINFO,ANALYZE,generating report)
	$(Q)buildsystem/gen_analyzer_report $(BUILDDIR)
endif

### Utility Rules ###
ifneq ($(INSTALL_SCRIPT),)
.PHONY : install
install : $(BUILDDIR)/$(TARGET)$(TARGET_TYPE)
	$(call OUTPUTINFO,INSTALL,$<)
	$(Q)$(INSTALL_SCRIPT) $<
endif

include buildsystem/autodep.mk
include buildsystem/rules.mk
