#
# Makefile for the FourthGen shared library
#
# Author: Oliver Maria Kind, Feb 2008
# Last update:
#   $Author$
#   $Date$
#   $Revision$
#
PROJECTNAME = SingleTop

# Defaults
DEBUG = off
MAP = off
PROF = off
OBJTABLE = off  
EVTPRINT = off

CDI = $(LIBCDI)

# Set architecture (in case it was not yet done)
ifeq ($(ARCH_TYPE),)
	ARCH_TYPE = $(shell uname -i`-`uname -s`-gcc`gcc -dumpversion) 
endif

# Debug mode ?
ifeq ($(DEBUG),on)
	CXXFLAGS += -DCF__DEBUG -g
endif

# Generate linker map file ?
ifeq ($(MAP),on)
	LDFLAGS += -Xlinker -Map -Xlinker linker-map
endif

# Generate profile information (used by gprof) ?
ifeq ($(PROF),on)
	CXXFLAGS += -pg
	LDFLAGS += -pg
endif

# Print object table ?
ifeq ($(OBJTABLE),on)
        CXXFLAGS += -DCF__OBJTABLE
endif

# Print every event
ifeq ($(EVTPRINT),on)
        CXXFLAGS += -DCF__EVTPRINT
endif

#
HdrSuf	      = h
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTGLIBS    += -lTreePlayer
CXX           = g++
CXXFLAGS     += -O -Wall -fPIC
LD            = g++
LDFLAGS      += $(shell root-config --ldflags) -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS         = $(ROOTGLIBS) $(SYSLIBS) \
	-L$(LIBHEPUTILS)/lib/$(ARCH_TYPE) -lHepUtils \
	-L$(LIBHEPEVENT)/lib/$(ARCH_TYPE) -lHepEvent \
	-L$(LIBATLASRUN)/lib/$(ARCH_TYPE) -lAtlasRun \
	-L$(LIBATLASEVENT)/lib/$(ARCH_TYPE) -lAtlasEvent \
	-L$(LIBATLASANALYSIS)/lib/$(ARCH_TYPE) -lAtlasAnalysis \
	-L$(LIBKINFITTER)/lib/$(ARCH_TYPE) -lKinFitter \
	-L$(LIBATLASFINDERS)/lib/$(ARCH_TYPE) -lAtlasFinders \
	-L$(PDFTOOL)/StandAlone -lPdfReweightingTool \
	-L$(LHAPDF6)/lib -lLHAPDF6 \
	-L$(TMVAHOME)/lib -lTMVA \
	-L$(CDI)/StandAlone -lCalibrationDataInterface
OBJDIR	 = ../obj/$(ARCH_TYPE)
DICTDIR	 = ../dict/$(ARCH_TYPE)
USRINCS += ../inc/
EXTINCS = $(PDFTOOL)/ $(PDFTOOL)/PdfReweightingTool/
EXTINCS += $(CDI)/ $(CDI)/CalibrationDataInterface/
USRSRC  += ../src/
LIBDIR     = ../lib/$(ARCH_TYPE)
ALLINCS  = $(USRINCS) $(EXTINCS) $(LIBHEPUTILS)/inc $(LIBHEPEVENT)/inc $(LIBATLASEVENT)/inc $(LIBATLASRUN)/inc $(LIBATLASANALYSIS)/inc $(LIBATLASANALYSIS)/inc/external $(LIBKINFITTER) $(LIBKINFITTER)/KinFitter $(LIBATLASFINDERS)/inc

CXXFLAGS += $(addprefix -I,$(ALLINCS))
LDFLAGS  +=  $(addprefix -I,$(ALLINCS))

PROJECTLIB = lib$(PROJECTNAME).$(DllSuf)

SRCFILES  = $(wildcard $(USRSRC)/*.$(SrcSuf))
HDRFILES  = $(wildcard $(USRINCS)/*.h))
DICTFILES = $(addprefix $(DICTDIR)/, $(addsuffix Dict.$(SrcSuf), \
	    	$(notdir $(basename $(HDRFILES)))))
OBJFILES  = $(addprefix $(OBJDIR)/, $(addsuffix .$(ObjSuf), \
		$(notdir $(basename $(SRCFILES))))) \
	   	$(addprefix $(OBJDIR)/, $(addsuffix .$(ObjSuf), \
		$(notdir $(basename $(DICTFILES)))))

MAKE_HTMLDOC = make_htmldoc.C

#------------------------------------------------------------------

all:	dict lib html

lib:	$(OBJFILES)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(LIBS) \
	$^ $(OutPutOpt) $(LIBDIR)/$(PROJECTLIB)
	@echo "$@ done"	

$(OBJDIR)/%.$(ObjSuf):	$(USRSRC)/%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) -c $< -o $@	
$(OBJDIR)/%.$(ObjSuf):	$(DICTDIR)/%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(OBJDIR)/%.$(ObjSuf):  $(USRINCS)/%.h

dict:	$(DICTFILES)

$(DICTDIR)/%Dict.$(SrcSuf): $(USRINCS)/%.h
	@echo -n "Generating dictionary $@...  "
	@rootcint -f $@	-c $(addprefix -I,$(ALLINCS)) $^
	@echo "done"

clean:
	@rm -fv $(OBJDIR)/*.$(ObjSuf)

cleanlib:
	@rm -fv $(LIBDIR)/*.$(DllSuf)

cleanall:	clean
	@rm -fv $(DICTDIR)/*.*
	@rm -fv $(LIBDIR)/*.$(DllSuf)
	@rm -frv ../htmldoc/*
purge:
	@rm -fv $(USRSRC)/*~
	@rm -fv $(USRINCS)/*~
	@rm -fv ../*~


checkdirs: $(LIBDIR) $(OBJDIR) $(DICTDIR)

$(LIBDIR): ../lib
	mkdir -p $@

$(OBJDIR): ../obj
	mkdir -p $@

$(DICTDIR): ../dict
	mkdir -p $@

../lib:
	mkdir -p $@

../obj:
	mkdir -p $@

../exe:
	mkdir -p $@

../dict:
	mkdir -p $@



html:
	@echo "Generating HTML documentation ..."
	@root -q -l $(MAKE_HTMLDOC)
	@echo "Done."

help:
	@echo ""
	@echo "Make the $(PROJECTNAME) shared library."
	@echo "Just type make."
	@echo ""
	@echo "Options available:"
	@echo "all       -> make shared library + html documentation"
	@echo "lib       -> make shared library"
	@echo "clean     -> remove object files"
	@echo "cleanlib  -> remove shared library"
	@echo "cleanall  -> remove object files, dictionaries, the shared"
	@echo "             library, and the HTML documentation"
	@echo "purge     -> remove backup files"
	@echo "dep       -> remake dependencies"
	@echo "html      -> make HTML documentation"
	@echo "checkdirs -> create necessary directories"
	@echo ""
	@echo "To create a debug version set the DEBUG variable to on. A linker map is created with the MAP variable. Information for gprof (profiling tool for time consumption per subroutine etc.) is produced by setting the PROF variable. In order to print an object table for every event set OBJTABLE=on (do not forget to set the Root.ObjectStat variable in your .rootrc file). To print every event set EVTPRINT=on."
	@echo "Variables can be combined, e.g. 'make lib DEBUG=on MAP=on'"
	@echo ""

# Automatic dependencies:
dep: .depend
.depend: $(SRCFILES)
	$(CXX) $(CXXFLAGS) -M $^ > .depend 2> /dev/null

# Include the dependency list:
include .depend
