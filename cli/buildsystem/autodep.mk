### Auto Dependency Rules ###
$(BUILDDIR)/%.d : %.c
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	@rm -f $@ && \
	$(CC) -MM $(CPPFLAGS) $< | \
	sed 's,\($(*F)\).o[ ]*:[ ]*,$(@D)/\1.o $@ : ,g' > $@

$(BUILDDIR)/%.d : %.cpp
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	@rm -f $@ && \
	$(CC) -MM $(CPPFLAGS) $< | \
	sed 's,\($(*F)\).o[ ]*:[ ]*,$(@D)/\1.o $@ : ,g' > $@

$(BUILDDIR)/%.d : %.m
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	@rm -f $@ && \
	$(CC) -MM $(CPPFLAGS) $< | \
	sed 's,\($(*F)\).o[ ]*:[ ]*,$(@D)/\1.o $@ : ,g' > $@

$(BUILDDIR)/%.d : %.S
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	@rm -f $@ && \
	$(CC) -MM $(CPPFLAGS) $< | \
	sed 's,\($(*F)\).o[ ]*:[ ]*,$(@D)/\1.o $@ : ,g' > $@
