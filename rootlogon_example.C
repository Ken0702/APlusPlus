// root logon script; load libraries and set style
{
    // general
    gSystem->Load("libEG.so"); // PDG database

    // A++ libraries
    gSystem->AddIncludePath(" -I$LIBHEPEVENT/inc ");
    gInterpreter->AddIncludePath("$LIBHEPEVENT/inc");
    gSystem->Load("$LIBHEPEVENT/lib/$ARCH_TYPE/libHepEvent.so");
    
    gSystem->AddIncludePath(" -I$LIBHEPUTILS/inc ");
    gInterpreter->AddIncludePath("$LIBHEPUTILS/inc");
    gSystem->Load("$LIBHEPUTILS/lib/$ARCH_TYPE/libHepUtils.so");
    
    gSystem->AddIncludePath(" -I$LIBATLASEVENT/inc ");
    gInterpreter->AddIncludePath("$LIBATLASEVENT/inc");
    gSystem->Load("$LIBATLASEVENT/lib/$ARCH_TYPE/libAtlasEvent.so");
    
    gSystem->AddIncludePath(" -I$LIBATLASRUN/inc ");
    gInterpreter->AddIncludePath("$LIBATLASRUN/inc");
    gSystem->Load("$LIBATLASRUN/lib/$ARCH_TYPE/libAtlasRun.so");
    
    gSystem->AddIncludePath(" -I$LIBATLASANALYSIS/inc ");
    gInterpreter->AddIncludePath("$LIBATLASANALYSIS/inc");  
    gSystem->Load("$LIBATLASANALYSIS/lib/$ARCH_TYPE/libAtlasAnalysis.so");
    
    gSystem->AddIncludePath(" -I$LIBKINFITTER/inc ");
    gInterpreter->AddIncludePath("$LIBKINFITTER/inc");
    gSystem->Load("$LIBKINFITTER/lib/$ARCH_TYPE/libKinFitter.so");
    
    gSystem->AddIncludePath(" -I$LIBATLASFINDERS/inc ");
    gInterpreter->AddIncludePath("$LIBATLASFINDERS/inc");
    gSystem->Load("$LIBATLASFINDERS/lib/$ARCH_TYPE/libAtlasFinders.so");
    
    gSystem->AddIncludePath(" -I$LIBSINGLETOP/inc ");
    gInterpreter->AddIncludePath("$LIBSINGLETOP/inc");
    gSystem->Load("$LIBSINGLETOP/lib/$ARCH_TYPE/libSingleTop.so");
}
