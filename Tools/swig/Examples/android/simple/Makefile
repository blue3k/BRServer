TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
TARGET     = example
INTERFACE  = example.i
INTERFACEDIR = jni/
PACKAGEDIR = $(SRCDIR)src/org/swig
PACKAGENAME= org.swig.simple
SWIGOPT    = -package $(PACKAGENAME) -outdir $(PACKAGEDIR)/simple
PROJECTNAME= SwigSimple
TARGETID  = 1
#INSTALLOPTIONS = -s # To install on SD Card

check: build

build:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' CXXSRCS='$(CXXSRCS)' SWIG='$(SWIG)' SWIGOPT='$(SWIGOPT)' \
	TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' INTERFACEDIR='$(INTERFACEDIR)' \
	PROJECTNAME='$(PROJECTNAME)' TARGETID='$(TARGETID)' android

install:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' INSTALLOPTIONS='$(INSTALLOPTIONS)' PROJECTNAME='$(PROJECTNAME)' \
	PACKAGEDIR='$(PACKAGEDIR)' PACKAGENAME='$(PACKAGENAME)' android_install

clean:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' TARGET='$(TARGET)' PROJECTNAME='$(PROJECTNAME)' \
	PACKAGEDIR='$(PACKAGEDIR)' INTERFACEDIR='$(INTERFACEDIR)' android_clean
