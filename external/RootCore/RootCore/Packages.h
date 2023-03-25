#ifndef ROOTCORE_PACKAGES_H
#define ROOTCORE_PACKAGES_H

// This file contains one define statement for each package detected by
// RootCore.  It is meant to allow to write package A in a form that
// it uses the services of package B when available, but otherwise
// skips it without failing.  For this to work properly you need to list
// package B in the PACKAGE_TRYDEP of package A.

#define ROOTCORE_PACKAGE_RootCore
#define ROOTCORE_PACKAGE_CalibrationDataInterface
#define ROOTCORE_PACKAGE_PdfReweightingTool
#define ROOTCORE_PACKAGE_TopFakes

#endif
