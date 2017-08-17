### Custom Functions ###

# convert source file extensions to another extension
CONVERTEXT = $(addsuffix .$(strip $1), $(basename $2))

TOUPPER = $(shell echo $1 | \
	  sed 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/')

RMTRAILINGSLASH = $(patsubst %/,%,$1)

USCORESUB = $(shell echo $1 | sed 'y; -./;____;')

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

PRINTLIST = @echo $1 | awk '{for(ii=1;ii<=NF;ii++){print "$2" $$ii;}}'

EXTRACT_TARGET = $(basename $(1))
EXTRACT_CONFIG = $(patsubst .%,%,$(suffix $(1)))
