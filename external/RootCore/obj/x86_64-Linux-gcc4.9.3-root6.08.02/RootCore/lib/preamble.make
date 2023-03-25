RC_CXX       = g++
RC_LD        = g++
RC_CXXFLAGS  = -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/Root -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore -O2 -Wall -fPIC -pthread -std=c++1y -m64 -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/include -g -Wno-tautological-undefined-compare -DROOTCORE -pthread -std=c++1y -m64 -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -pipe -W -Wall -Wno-deprecated -pedantic -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-long-long -Wdeprecated-declarations -DROOTCORE_PACKAGE=\"RootCore\" 
RC_DICTFLAGS = -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/Root -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore -O2 -Wall -fPIC -pthread -std=c++1y -m64 -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/include -g -Wno-tautological-undefined-compare -DROOTCORE -pthread -std=c++1y -m64 -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -pipe -W -Wall -Wno-deprecated -pedantic -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-long-long -Wdeprecated-declarations -DROOTCORE_PACKAGE=\"RootCore\" 
RC_INCFLAGS  = -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/Root -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -I/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/include -DROOTCORE -I/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/include -DROOTCORE_PACKAGE=\"RootCore\"
RC_LIBFLAGS  = -shared -m64 -L/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic 
RC_BINFLAGS  = -L/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/obj/x86_64-Linux-gcc4.9.3-root6.08.02/RootCore/lib -L/mnt/eeh/kaphle/atlas/APlusPlus_git/external/RootCore/lib/x86_64-Linux-gcc4.9.3-root6.08.02 -m64 -L/cvmfs/sft.cern.ch/lcg/releases/LCG_87/ROOT/6.08.02/x86_64-slc6-gcc49-opt/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic


all_RootCore : dep_RootCore package_RootCore

package_RootCore :  postcompile_RootCore

postcompile_RootCore : 
	$(SILENT)rc --internal postcompile_pkg RootCore


dep_RootCore :
