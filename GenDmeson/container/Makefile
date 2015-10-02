# ======================================================================
# NOTE: make all
# -----
#
# ======================================================================


ifdef ROOTSYS
  ROOTCINT      = $(ROOTSYS)/bin/rootcint
  ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
  ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
  ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
else 
  ROOTCINT      = rootcint
  ROOTCFLAGS    = $(shell root-config --cflags)
  ROOTLIBS      = $(shell root-config --libs)
  ROOTGLIBS     = $(shell root-config --glibs)
endif

ifdef SCRAM_ARCH
  CXX         := $(shell scramv1 tool info gcc-cxxcompiler | grep CXX= | sed s/CXX=//)
else 
  ifdef CXXCOMPILER
    CXX         := $(CXXCOMPILER)
  else 
    CXX         := c++
  endif
endif

CXXFLAGS      = -g -Os -Wall -fPIC -pipe
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))

#########
# FILES #
#########

#used by all targets
ROOTIO_HEADERS = daughterparticle.h middleparticle.h motherquark.h dmeson.h 

# Ana00 compilation
ANA00 = daughterparticle.o middleparticle.o motherquark.o dmeson.o
ANA00_DICT = ${ANA00:.o=Dict.o}

#################
# PATTERN RULES #
#################

# Default rule compiling c++ files
obj/%.o : %.cc $(addprefix ,$(ROOTIO_HEADERS))
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

obj/%.o : tnp/%.cc $(addprefix tnp/,$(TNP_HEADERS))
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

# Default rules creating dictionaries
%Dict.cc : %.h %LinkDef.h
	$(ROOTCINT) -f $@ -c $^

%Dict.cc : %.h
	$(ROOTCINT) -f $@ -c $<

tnp/%Dict.cc : tnp/%.h tnp/%LinkDef.h
	$(ROOTCINT) -f $@ -c $^

tnp/%Dict.cc : tnp/%.h
	$(ROOTCINT) -f $@ -c $<


###########
# TARGETS #
###########

all: Gendmeson links
	@true

#short
Gendmeson: lib/libGendmeson.so
	@true

lib/libGendmeson.so: $(addprefix obj/, $(ANA00) $(ANA00_DICT))
	$(CXX) $(SOFLAGS) $(GLIBS) $(addprefix obj/,$(ANA00) $(ANA00_DICT)) -o lib/libGendmeson.so


# ================================================================================
links:
	mkdir -p ../../../../lib/$(SCRAM_ARCH)
	cd ../../../../lib/$(SCRAM_ARCH)/ && rm -f libGendmeson.so && ln -s ../../src/UserCode/GenDmeson/container/lib/libGendmeson.so && cd -

copy:
	cd ../../../../lib/$(SCRAM_ARCH)/ && rm -f libGendmeson.so && cp ../../src/UserCode/GenDmeson/container/lib/libGendmeson.so . && cd -

# ================================================================================
clean:
	rm -f obj/*.o *Dict.* test/*Dict.* tnp/*Dict.* lib/libGendmeson.so 

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f lib/lib*.so
	rm -f ../../../../lib/$(SCRAM_ARCH)/libGendmeson.so
