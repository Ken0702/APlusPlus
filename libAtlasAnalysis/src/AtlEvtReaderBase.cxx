//____________________________________________________________________
//
// Abstract base class for reading evens with AtlSelector
// 
// 
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#include <AtlEvtReaderBase.h>

#include <memory>

#include <TDirectory.h>
#include <TH1.h>
#include <TROOT.h>
#include <TTree.h>

#ifndef __CINT__
ClassImp(AtlEvtReaderBase);
#endif

//____________________________________________________________________

AtlEvtReaderBase::AtlEvtReaderBase(AtlSelector *parent) :
    fParent(parent) {
    //
    // Default constructor
    //
    fEvent = 0;
}

//____________________________________________________________________

AtlEvtReaderBase::~AtlEvtReaderBase() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

AtlEvtReaderBase::InitialSumOfWeights_t AtlEvtReaderBase::GetInitialSumOfWeights(TFile *) const {
    Fatal(__FUNCTION__, "not implemented");
    return AtlEvtReaderBase::InitialSumOfWeights_t();
}

//____________________________________________________________________

Bool_t AtlEvtReaderBase::SumOverTree(TTree * tree, char const * expression, Double_t & result) {
    //
    // Sums expression over all entries in the tree, and stores the sum in result.
    // True return value indicates success.
    //
    if ( nullptr == tree )
        return kFALSE;
    TDirectory::TContext directoryContext(gROOT);
    auto h = std::make_unique<TH1D>("h_sumOverTree_temp", "", 1, -1., 1.);
    h->SetDirectory(gROOT);
    Long64_t nEntriesSelected = tree->Project("h_sumOverTree_temp", "0", expression);
    if ( !(nEntriesSelected >= 0) )
        return kFALSE;
    result = h->GetBinContent(1);
    return kTRUE;
}
