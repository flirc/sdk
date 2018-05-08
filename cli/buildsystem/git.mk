SCMVERSION := $(shell git describe --tags --dirty=+ --always 2>/dev/null || \
		echo "UNKNOWN")
SCMBRANCH := $(shell (git branch 2> /dev/null || echo '* UNKNOWN') | \
		awk '/^\* / {print $$2;}')

SCMVERSION := $(shell (git describe --tags --dirty=+ --always | sed -e 's/v\(.*\)/\1/g'))
SCMVER     := $(shell (git describe --tags --always --abbrev=0 | sed -e 's/v\(.*\)/\1/g'))
