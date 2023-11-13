USCORESUB = $(shell echo $1 | sed 'y; -./;____;')
export MACHINE := $(call USCORESUB,$(shell uname -sm))

# Handle Windows, make sure mingw is in your path
ifeq ($(HOSTOS),WIN)
    CC          := i686-w64-mingw32.static-gcc
    CCP         := i686-w64-mingw32.static-g++
    WINRES      := i686-w64-mingw32.static-windres
    MAKENSIS    := i686-w64-mingw32.static-makensis
    CXXFLAGS    += -D__HOST_WIN__
    CFLAGS      += -D__HOST_WIN__
    LIBRARIES   += hidapi ncurses hid setupapi 
    LSEARCH     := -L../libs/Win/
    # Fixes packing issue
    CFLAGS      += -mno-ms-bitfields
    SUFFIX      := .exe
# Mac OSX (x86)
else ifeq ($(HOSTOS),DARWIN)
    LIBRARIES   += hidapi 
    LSEARCH     := -L../libs/$(MACHINE)/
    LDFLAGS     += $(addprefix -framework ,$(FRAMEWORKS))
# Handle Linux
else ifeq ($(HOSTOS),LINUX)
    LIBRARIES += hidapi-hidraw usb-1.0

    # Check if we are compiling for ARM/Pi
    ifeq ($(findstring aarch64,$(MACHINE)),aarch64)
        ifeq (64, $(shell getconf LONG_BIT))
            LSEARCH := -L../libs/Linux_aarch64
        else
            LSEARCH := -L../libs/Linux_arm
        endif
    else
        ifeq (64, $(shell getconf LONG_BIT))
            LSEARCH := -L../libs/Linux_x86_64
        else
            LSEARCH := -L../libs/Linux_i686
        endif
    endif
# LIBREELEC
else ifeq ($(HOSTOS),LIBREELEC)
    LIBRARIES += hidapi-hidraw
    LDFLAGS := $(subst -L/usr/local/lib,,$(LDFLAGS))
    # Check if we are compiling for ARM/Pi
    ifeq ($(findstring aarch64,$(MACHINE)),aarch64)
        ifeq (64, $(shell getconf LONG_BIT))
            LSEARCH := -L../libs/Linux_aarch64
        else
            LSEARCH := -L../libs/Linux_arm
        endif
    else
        ifeq (64, $(shell getconf LONG_BIT))
            LSEARCH := -L../libs/Linux_x86_64
        else
            LSEARCH := -L../libs/Linux_i686
        endif
    endif
else
$(info "Only Supported Options are: WIN, LINUX, DARWIN, LIBREELEC")
$(error HOSTOS=$(HOSTOS) is not supported)
endif

# Add all the libraries defined in config.mk to LDLIBS
ifneq ($(HOSTOS), DARWIN)
LDFLAGS := -Wl,--start-group $(addprefix -l,$(LIBRARIES)) -Wl,--end-group
else
LDFLAGS := $(addprefix -l,$(LIBRARIES))
endif
