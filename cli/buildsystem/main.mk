include buildsystem/func.mk
include buildsystem/cross.mk

# Set default flags
CPPFLAGS := -Iinclude -Isrc -I../libs/include
CFLAGS := -Wall -Werror 

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

# Add all the libraries defined in config.mk to LDLIBS
ifneq ($(HOSTOS), DARWIN)
LDLIBS := -Wl,--start-group $(addprefix -l,$(LIBRARIES)) -Wl,--end-group
else
LDLIBS := $(addprefix -l,$(LIBRARIES))
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
