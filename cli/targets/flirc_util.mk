CONFIGS := debug release

# Sources
SOURCES :=	src/main.c \
		src/prepost.c \
		lib/logging.c \
		lib/getline.c \
		lib/dict.c \
		lib/cmds.c \
		lib/cmds_script.c \
		src/cmds/flirc_cmds.c \
		src/cmds/ir_transmit.c \

# Libraries
LIBRARIES  += flirc

ifeq ($(ISHELL), 1)
LIBRARIES += readline
SOURCES   += lib/cmds_shell.c
endif

# Version
include buildsystem/git.mk
OPTIONS += SCMVERSION='"$(SCMVERSION)"'
OPTIONS += SCMBRANCH='"$(SCMBRANCH)"'
OPTIONS += SCMVER='"$(SCMVER)"'
OPTIONS += SCMVERSION='"$(SCMVERSION)"'

CPPFLAGS += -DBUILD_DATE='"$(shell date)"'

# Release Config
ifeq ($(CONFIG),release)
# Options
OPTIONS += MAX_LOGLEVEL=3 DEFAULT_LOGLEVEL=2
# Flags
CFLAGS += -O2
endif

# Debug Config
ifeq ($(CONFIG),debug)
# Options
OPTIONS += MAX_LOGLEVEL=5 DEFAULT_LOGLEVEL=4
# Flags
CFLAGS += -O0 -g
endif

INSTALL_SCRIPT = targets/install
