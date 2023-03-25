# configure for the RootCore packages
# using the ROOTCOREBIN environment variable

set(RootCore_INCLUDE_DIRS "$ENV{ROOTCOREBIN}/include")
set(RootCore_LIBRARY "$ENV{ROOTCOREBIN}/lib/$ENV{ARCH_TYPE}")
include_directories(${RootCore_INCLUDE_DIRS})
