TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
SWIGOPT    =
SRCS       = example.c
TARGET     = example
INTERFACE  = example.i
MLFILE     = example.ml
PROGFILE   = example_prog.ml
OBJS       = example.o

check: build
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' TARGET='$(TARGET)' ocaml_run

build: static static_top

static:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' SRCS='$(SRCS)' SWIG='$(SWIG)' \
	SWIGOPT='$(SWIGOPT)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' \
	MLFILE='$(MLFILE)' PROGFILE='$(PROGFILE)' OBJS='$(OBJS)' \
	ocaml_static_cpp

static_top:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' SRCS='$(SRCS)' SWIG='$(SWIG)' \
	SWIGOPT='$(SWIGOPT)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)' \
	MLFILE='$(MLFILE)' PROGFILE='$(PROGFILE)' OBJS='$(OBJS)' \
	ocaml_static_cpp_toplevel

dynamic:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' SRCS='$(SRCS)' SWIG='$(SWIG)' \
	SWIGOPT='$(SWIGOPT)' TARGET='$(TARGET)' INTERFACE='$(INTERFACE)'
	MLFILE='$(MLFILE)' PROGFILE='$(PROGFILE)' OBJS='$(OBJS)' \
	ocaml_dynamic_cpp

clean:
	$(MAKE) -f $(TOP)/Makefile SRCDIR='$(SRCDIR)' MLFILE='$(MLFILE)' ocaml_clean
