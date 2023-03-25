A++ 
====

ATLAS Analysis Framework from Humboldt-University of Berlin based on C++

Install:
--------

- clone the repository
```
git clone ssh://git@gitlab.cern.ch:7999/hub-atlas/aplusplus.git APlusPlus
```
- set up environment (example at HU group)
  - take `.zshenv_example` code and add it to your `.zshenv` file (or `.bashrc`)
  - change `source ~/APlusPlus/setup.sh` to the path you chose for cloning
  - define a ROOT and Python setup in `~/bin/setup-root.sh` or link the example file with
```
ln -s $HOME/APlusPlus/setup-root.sh $HOME/bin/setup-root.sh
```
  - load libraries in `.rootlogon.C` like in `rootlogon_example.C`:
```
gSystem->AddIncludePath(" -I$LIBHEPEVENT/inc ");
gInterpreter->AddIncludePath("$LIBHEPEVENT/inc");
gSystem->Load("$LIBHEPEVENT/lib/$ARCH_TYPE/libHepEvent.so");
...
```
- build external libraries with RootCore
  - `./RootCore-build.sh -c` until it builds (needs CERN svn)
  - against the error in CalibrationDataInterface:
```
sed -i 's/TAxis\*/TAxis const \*/g' external/CalibrationDataInterface/Root/CalibrationDataContainer.cxx
```
  - against the error in TopFakes:
```
sed -i 's/PACKAGE_DEP/PACKAGE_DEP      =\n#PACKAGE_DEP/g' $APP_HOME/external/TopFakes/cmt/Makefile.RootCore
sed -i 's/#ifndef STANDALONE/#define STANDALONE\n#ifndef STANDALONE/g' $APP_HOME/external/TopFakes/Root/LinkDef.h
rm $APP_HOME/external/TopFakes/Root/TopFakes*.cxx $APP_HOME/external/TopFakes/TopFakes/TopFakes*.h
```
Alternativ: copy external directory of other APlusPlus directory then run ./build.sh


- build A++ with cmake (-g for debug flags):
```
./build.sh -g
```
