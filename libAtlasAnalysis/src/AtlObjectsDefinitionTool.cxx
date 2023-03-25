//___________________________________________________________
//
//  A++ Objects Definition Tool
//
//  This tool is used to select objects, like jets or leptons
//  for the user analysis and the tools.
// 
//
//  Author: Soeren Stamm <mailto:stamm@physik.hu-berlin.de> 
//  Update: $Id$
//  Copyright: 2013 (C) Soeren Stamm
//
//
//
#ifndef ATLAS_AtlObjectsDefinitionTool
#include <AtlObjectsDefinitionTool.h>
#endif
#include <AtlEvent.h>
#include <iostream>
#include <TMath.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlObjectsDefinitionTool);
#endif

//___________________________________________________________

AtlObjectsDefinitionTool::AtlObjectsDefinitionTool(const char* name, const char* title) : 
AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    
    SetCutDefaults();

}

//___________________________________________________________

AtlObjectsDefinitionTool::~AtlObjectsDefinitionTool(){
    //
    // Default destructor
    //
}

//___________________________________________________________

void AtlObjectsDefinitionTool::SetBranchStatus() {
    //
    // Switch on needed branches
    //

    fTree->SetBranchStatus("fEventHeader*",               kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4TopoEMJets*",       kTRUE);
    fTree->SetBranchStatus("fAntiKt4TopoEMJets*",         kTRUE);
    fTree->SetBranchStatus("fN_Electrons*",               kTRUE);
    fTree->SetBranchStatus("fElectrons*",                 kTRUE);
    fTree->SetBranchStatus("fN_Muons*",                   kTRUE);
    fTree->SetBranchStatus("fMuons*",                     kTRUE);
    fTree->SetBranchStatus("fEnergySum*",                 kTRUE);
    fTree->SetBranchStatus("fVertices*",                  kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",                kTRUE);
    fTree->SetBranchStatus("fIDTracks*",                  kTRUE);
    fTree->SetBranchStatus("fN_IDTracks",                 kTRUE);
}

//___________________________________________________________

void AtlObjectsDefinitionTool::SetCutDefaults() {
  //
  // Selection cut defaults
  //

  // Define when the tool runs and how
  fProcessMode       = AtlAnalysisTool::kObjectsDefinition;
}

//___________________________________________________________

void AtlObjectsDefinitionTool::Clear(){
    //
    //  Clears the dynamically allocated objects
    //    
    
}

//___________________________________________________________

void AtlObjectsDefinitionTool::BookHistograms(){
    //
    // Book histograms (after kinematic fit)
    //
   
}

//___________________________________________________________

Bool_t AtlObjectsDefinitionTool::AnalyzeEvent() {
    //
    // Event analysis: required object selection
    //
    return kTRUE;
}

//___________________________________________________________

void AtlObjectsDefinitionTool::FillHistograms() {
    //
    // Method for filling histograms, only events that survive 
    // AnalyzeEvent() are taken into account
    //

}

//___________________________________________________________

void AtlObjectsDefinitionTool::Print() const {
    //
    // Print cuts for object selection of the tool
    //

}

//___________________________________________________________


void AtlObjectsDefinitionTool::Terminate(){
    //
    // Executed at the end of the tool
    //

}

