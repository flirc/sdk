SCMVERSION := $(shell git describe --tags --dirty=+ --always 2>/dev/null || \
		echo "UNKNOWN")
SCMBRANCH := $(shell (git branch 2> /dev/null || echo '* UNKNOWN') | \
		awk '/^\* / {print $$2;}')
