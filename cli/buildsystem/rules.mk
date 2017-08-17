### Implicit Source Rules ###
$(BUILDDIR)/%.o : %.c
	$(call OUTPUTINFO,CC,$<)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
ifeq ($(ANALYZE),1)
	$(call OUTPUTINFO,ANALYZE,$<)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) \
		$(addprefix -Xanalyzer ,$(ANALYZE_FLAGS)) --analyze \
		-o $@.analyze $<
endif

$(BUILDDIR)/%.o : %.m
	$(call OUTPUTINFO,OBJCC,$<)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
ifeq ($(ANALYZE),1)
	$(call OUTPUTINFO,ANALYZE,$<)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) \
		$(addprefix -Xanalyzer ,$(ANALYZE_FLAGS)) --analyze \
		-o $@.analyze $<
endif

$(BUILDDIR)/%.o : %.cpp
	$(call OUTPUTINFO,C++,$<)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
ifeq ($(ANALYZE),1)
	$(call OUTPUTINFO,ANALYZE,$<)
	$(Q)$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		$(addprefix -Xanalyzer ,$(ANALYZE_FLAGS)) --analyze \
		-o $@.analyze $<
endif

$(BUILDDIR)/%.o : %.S
	$(call OUTPUTINFO,AS,$<)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(CC) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o : %.s
	$(call OUTPUTINFO,AS,$<)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(AS) $(ASFLAGS) -c -o $@ $<

%.a :
	$(call OUTPUTINFO,AR,$@)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(AR) crs $@ $^

%.dylib :
	$(call OUTPUTINFO,DYLIB,$@)
	$(Q)[ -d "$(@D)" ] || mkdir -p "$(@D)"
	$(Q)$(CC) -dynamiclib $(LDFLAGS) $^ $(LDLIBS) -o $@
