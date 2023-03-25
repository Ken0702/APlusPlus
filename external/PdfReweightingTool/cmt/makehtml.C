{
  gSystem->Load( "$ROOTCOREDIR/lib/libPdfReweightingTool.so" );
  gSystem->AddIncludePath(" -I$ROOTCOREDIR/include/PdfReweightingTool ");
  gInterpreter->AddIncludePath("$ROOTCOREDIR/include/PdfReweightingTool"); 

  gSystem->AddIncludePath(" -I$ROOTSYS/include ");
  
  THtml html;
  //html.SetInputDir("$ROOTSYS/include/:../PdfReweightingTool/");
  html.SetInputDir("$ROOTCOREDIR/include/PdfReweightingTool/");
  html.SetProductName("PdfReweightingTool");
  html.SetDocPath("../doc");
  html.SetOutputDir("../html");
  html.MakeAll(kFALSE, "Pdf*");
  
}
