TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
CXXSRCS    =
TARGET     = example
INTERFACE  = example.i
SWIGOPT    =

check: build
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' CXXSRCS='$(CXXSRCS)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' go_run

build:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' CXXSRCS='$(CXXSRCS)' SWIG='$(SWIG)' \
	SWIGOPT='$(SWIGOPT)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' go

clean:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' INTERFACE='$(INTERFACE)' go_clean
