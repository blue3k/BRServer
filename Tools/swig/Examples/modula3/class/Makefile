TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
SRCS       =
TARGET     = example
PLATFORM   = LINUXLIBC6
INTERFACE  = example.i
SWIGOPT    = -c++
MODULA3SRCS = *.[im]3

check: build
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' modula3_run

build:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' SRCS='$(SRCS)' SWIG='$(SWIG)' \
	SWIGOPT='$(SWIGOPT)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' modula3
	m3ppinplace $(MODULA3SRCS)
# compilation of example_wrap.cxx is started by cm3
#	$(CXX) -c $(TARGET)_wrap.cxx
	mv example_wrap.cxx m3makefile $(MODULA3SRCS) src/
	ln -sf ../example.h src/example.h
	cm3

clean:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' modula3_clean
