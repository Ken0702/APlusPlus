#!/usr/bin/env python
#
# Task configuration for the SgTop 13TeV tZ FCNC analysis
#
# Known limitations:
# - In your sample list file, you must use the sample number (with an 'a' suffix for AtlFast samples)
#   for the sample name.
# - The script relies on AtlTopLevelAnalysis::BuildTree, which can be extremely slow
#   (15 minutes on as01 when systematics are on and Sherpa Z+jets is in the sample file list).
# - The implementation of the fake estimate is still a bit hacky.

import argparse
import array
import getpass
import itertools
import os
import re
import sys


# standard samples
samples = []
samples += [361106, 361107, 361108] # Z Powheg+Pythia8
samples += range(364100, 364142) # Z Sherpa
samples += [361600, 361601, 361603, 361604, 361607, 361610] # VV Powheg+Pythia8
samples += [361071, 361072, 361073, 363490, 363491, 361068] # VV Sherpa
samples += [410000, 410009] # ttbar Powheg+Pythia6
samples += [410015, 410016] # tW Powheg+Pythia6
samples += [410050] # tZj MadGraph+Pythia6
samples += range(410066, 410069) # ttW MadGraph+Pythia8
samples += range(410111, 410117) # ttZ MadGraph+Pythia8
samples += [343365, 343366, 343367] # ttH aMC@NLO+Pythia8
samples += [-410740, -410741] # tZ FCNC private production (AtlFast2)

# samples for generator variations
samples_for_gensyst = []
samples_for_gensyst += [-410000, -410004] # ttbar generator variations (AtlFast2)
samples_for_gensyst += [410001, 410002] # ttbar shower scale variations

# samples use for fake estimate
samples_for_fakes = [361106, 361107, 361108, 410000, 410009] + range(364100, 364142)

# user's preferred disk for job output
disk_by_user = {
      'mergelm': 'datm8',
      None: 'datm5', # default
   }


def load_ROOT(batch=None, rootlogon=None):
   """Import ROOT -- with some tweaks."""
   orig_args = sys.argv[1:]
   if batch is None:
      batch = bool(os.environ.get('DISPLAY'))
   if rootlogon is None:
      rootlogon = True
   root_args = list(itertools.compress(['-b', '-l', '-n'], [batch, True, not rootlogon]))
   try:
      sys.argv[1:] = root_args
      import ROOT
      ROOT.TObject
   finally:
      sys.argv[1:] = orig_args
   globals()['ROOT'] = ROOT


def dsid_from_task(task):
   """Extract signed dataset number from a task('s name).

   Returns the dataset number, using negative values for AtlFast2 samples.
   Uses 0 for real data, and None for unrecognized tasks.
   """
   name = task.GetName()
   m = re.match(r'(?:([1-9][0-9]*)([as]?)|(data))$', name)
   if not m:
      return None
   if m.group(1):
      sign = {'a': -1, 's': 1, '': 1}[m.group(2)]
      dsid = sign * int(m.group(1), 10)
   else:
      dsid = 0
   return dsid


def add_subselection(task, name, cuts):
   """Add a subselection to a task.
   
   task -- the task
   name -- the name of the subselection
   cuts -- dictionary with variables and values (bool, int, float)
   """
   subsel = ROOT.AtlSubselectionCuts(name)
   for var in sorted(cuts):
      val = cuts[var]
      if isinstance(val, bool):
         val = ('kFALSE', 'kTRUE')[val]
      elif isinstance(val, (int, float)):
         val = repr(val)
         if val[0].isalpha():
            val = {'inf': '-TMath::Log(0.)', '-inf': 'TMath::Log(0.)', 'nan': 'TMath::Log(-1.)'}[val]
      else:
         raise TypeError()
      cut = ROOT.AtlCut(var, val)
      ROOT.SetOwnership(cut, False)
      subsel.GetListOfCuts().Add(cut)
   ROOT.SetOwnership(subsel, False)
   task.GetListOfSubselectionCuts().Add(subsel)


def modify_tasks(task, opts, level=0):
   """Modify the task tree."""
   if isinstance(task, ROOT.AtlTask):
      if not accept_task(task, opts):
         task.SetActive(False)
   for subtask in task.GetListOfTasks():
      modify_tasks(subtask, opts, level=(level + 1))


def accept_task(task, opts):
   """Decide whether a task should run."""
   name = task.GetName()
   if (name == 'plotting') != (opts.what == 'plot'):
      return False
   dsid = dsid_from_task(task)
   assert dsid is not None
   if opts.what == 'data' and not (dsid == 0):
      return False
   if opts.what == 'gensyst':
      return (dsid in samples_for_gensyst)
   if opts.what != 'all' and not (dsid == 0 or dsid in samples):
      return False
   return True


def list_systematics():
   """Yield the list of systematic names."""
   yield "JET_21NP_JET_BJES_Response__1down"
   yield "JET_21NP_JET_BJES_Response__1up"
   yield "JET_21NP_JET_EffectiveNP_1__1down"
   yield "JET_21NP_JET_EffectiveNP_1__1up"
   yield "JET_21NP_JET_EffectiveNP_2__1down"
   yield "JET_21NP_JET_EffectiveNP_2__1up"
   yield "JET_21NP_JET_EffectiveNP_3__1down"
   yield "JET_21NP_JET_EffectiveNP_3__1up"
   yield "JET_21NP_JET_EffectiveNP_4__1down"
   yield "JET_21NP_JET_EffectiveNP_4__1up"
   yield "JET_21NP_JET_EffectiveNP_5__1down"
   yield "JET_21NP_JET_EffectiveNP_5__1up"
   yield "JET_21NP_JET_EffectiveNP_6__1down"
   yield "JET_21NP_JET_EffectiveNP_6__1up"
   yield "JET_21NP_JET_EffectiveNP_7__1down"
   yield "JET_21NP_JET_EffectiveNP_7__1up"
   yield "JET_21NP_JET_EffectiveNP_8restTerm__1down"
   yield "JET_21NP_JET_EffectiveNP_8restTerm__1up"
   yield "JET_21NP_JET_EtaIntercalibration_Modelling__1down"
   yield "JET_21NP_JET_EtaIntercalibration_Modelling__1up"
   yield "JET_21NP_JET_EtaIntercalibration_NonClosure__1down"
   yield "JET_21NP_JET_EtaIntercalibration_NonClosure__1up"
   yield "JET_21NP_JET_EtaIntercalibration_TotalStat__1down"
   yield "JET_21NP_JET_EtaIntercalibration_TotalStat__1up"
   yield "JET_21NP_JET_Flavor_Composition__1down"
   yield "JET_21NP_JET_Flavor_Composition__1up"
   yield "JET_21NP_JET_Flavor_Response__1down"
   yield "JET_21NP_JET_Flavor_Response__1up"
   yield "JET_21NP_JET_Pileup_OffsetMu__1down"
   yield "JET_21NP_JET_Pileup_OffsetMu__1up"
   yield "JET_21NP_JET_Pileup_OffsetNPV__1down"
   yield "JET_21NP_JET_Pileup_OffsetNPV__1up"
   yield "JET_21NP_JET_Pileup_PtTerm__1down"
   yield "JET_21NP_JET_Pileup_PtTerm__1up"
   yield "JET_21NP_JET_Pileup_RhoTopology__1down"
   yield "JET_21NP_JET_Pileup_RhoTopology__1up"
   yield "JET_21NP_JET_PunchThrough_MC15__1down"
   yield "JET_21NP_JET_PunchThrough_MC15__1up"
   yield "JET_21NP_JET_SingleParticle_HighPt__1down"
   yield "JET_21NP_JET_SingleParticle_HighPt__1up"
   yield "JET_JER_SINGLE_NP__1up"
   yield "EG_RESOLUTION_ALL__1down"
   yield "EG_RESOLUTION_ALL__1up"
   yield "EG_SCALE_ALL__1down"
   yield "EG_SCALE_ALL__1up"
   yield "EL_SF_ID__1down"
   yield "EL_SF_ID__1up"
   yield "EL_SF_Isol__1down"
   yield "EL_SF_Isol__1up"
   yield "EL_SF_Reco__1down"
   yield "EL_SF_Reco__1up"
   yield "EL_SF_Trigger__1down"
   yield "EL_SF_Trigger__1up"
   yield "MET_SoftTrk_ResoPara"
   yield "MET_SoftTrk_ResoPerp"
   yield "MET_SoftTrk_ScaleDown"
   yield "MET_SoftTrk_ScaleUp"
   yield "MUON_ID__1down"
   yield "MUON_ID__1up"
   yield "MUON_MS__1down"
   yield "MUON_MS__1up"
   yield "MUON_SAGITTA_RESBIAS__1down"
   yield "MUON_SAGITTA_RESBIAS__1up"
   yield "MUON_SAGITTA_RHO__1down"
   yield "MUON_SAGITTA_RHO__1up"
   yield "MUON_SCALE__1down"
   yield "MUON_SCALE__1up"
   yield "MU_SF_ID_STAT_LOWPT__1down"
   yield "MU_SF_ID_STAT_LOWPT__1up"
   yield "MU_SF_ID_STAT__1down"
   yield "MU_SF_ID_STAT__1up"
   yield "MU_SF_ID_SYST_LOWPT__1down"
   yield "MU_SF_ID_SYST_LOWPT__1up"
   yield "MU_SF_ID_SYST__1down"
   yield "MU_SF_ID_SYST__1up"
   yield "MU_SF_Isol_STAT__1down"
   yield "MU_SF_Isol_STAT__1up"
   yield "MU_SF_Isol_SYST__1down"
   yield "MU_SF_Isol_SYST__1up"
   yield "MU_SF_TTVA_STAT__1down"
   yield "MU_SF_TTVA_STAT__1up"
   yield "MU_SF_TTVA_SYST__1down"
   yield "MU_SF_TTVA_SYST__1up"
   yield "MU_SF_Trigger_STAT__1down"
   yield "MU_SF_Trigger_STAT__1up"
   yield "MU_SF_Trigger_SYST__1down"
   yield "MU_SF_Trigger_SYST__1up"
   yield "Pileup_SF__1down"
   yield "Pileup_SF__1up"
   yield "JVT_SF__1down"
   yield "JVT_SF__1up"
   btagVariations = [('B', 6), ('C', 4), ('Light', 17)]
   for flavour, componentCount in btagVariations:
      for componentIndex in xrange(0, componentCount):
         yield "bTagSF_%s_%d__1down" % (flavour, componentIndex)
         yield "bTagSF_%s_%d__1up" % (flavour, componentIndex)
   yield "Njet_BC__1up"
   yield "Njet_1__1up"
   yield "Njet_2__1up"
   yield "Njet_3__1up"
   yield "Njet_4orMore__1up"


def make_top_task(opts):
   """Create and initialize the top-level analysis task."""

   top = ROOT.AtlTopLevelAnalysis("AtlSgTopFCNC_tZ", "FCNC tZ 3L")
   ROOT.SetOwnership(top, False)

   # AtlTopLevelAnalysis options
   top.SetForceRetry(opts.force_retry)
   top.SetBuildHforTree(False)
   top.SetBuildAnalysisTree(opts.what != 'plot')
   top.SetBuildMemTkAnalysisTree(False)
   top.SetBuildMergingTree(False)
   top.SetBuildPlottingTree(opts.what == 'plot')
   top.SetUseHforSamples(False)
   top.SetTtbarIsSplitted(False)

   # Grid config (outdated)
   top.SetGridRootVersion("5.34/21")
   top.SetGridCmtVersion("x86_64-slc6-gcc47-opt")
   top.SetGridUserName("kind")
   top.SetGridSuffix("v1")
   top.SetGridJob(False)

   # Setup Samples
   top.SetTreeName("") # Use "" for tight and "_Loose" for loose lepton events
   top.SetInputFormat(ROOT.AtlTopLevelAnalysis.kTreePerSyst)
   top.SetCampaign(15)
   top.SetSgTopVersion("v16")
   top.SetCrossSectionFilePath("/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisTop/2.4.33/TopDataPreparation/data/XSection-MC15-13TeV.data")
   top.SetDSIDList("../SampleList/SgTopDSIDs_mc15_v16.txt")
   top.ReadSampleList("../SampleList")

   # Setup Directories
   user = getpass.getuser()
   disk = disk_by_user.get(user, disk_by_user[None])
   suffix = ('_%s' % opts.name if opts.name else '')
   assert suffix
   if opts.what.startswith('fakes'):
      suffix = '_%s%s' % (opts.what, suffix)
   top.SetHistDir('/rdsk/%s/atlas/%s/sgtop_fcnc_tZ%s' % (disk, user, suffix))
   top.SetJobHomeDir('/rdsk/%s/atlas/%s/sgtop_fcnc_tZ%s' % (disk, user, suffix))
   top.SetTempDir('/datc/atlas/%s/sgtop_fcnc_tZ%s.tmp' % (user, suffix))
   top.SetInputDir('%s/%s' % (top.GetHistDir(), top.GetName()))
   top.SetPlotDir('%s/%s' % (top.GetHistDir(), top.GetName()))

   # Set output tree and format
   top.SetOutputTree("", "")

   # Set output file prefix
   top.SetOutputFilePrefix("ntup_tZ")

   # Systematics
   top.AddSystematicGroup(ROOT.AtlTopLevelAnalysis.kSystGrpNominal)
   if opts.systematics:
      map(top.AddSystematic2, list_systematics())

   # ==============
   # Setup Analysis
   # ==============
   
   # User environment
   top.AddUserEnv("LIBSINGLETOP")

   # Set lepton channel (e or u)
   top.AddLeptonChannel(ROOT.AtlSelector.kLepton)
   
   # Set jet multiplicity
   top.AddJetMultiplicity(ROOT.AtlSelector.kAllJets)

   # Analysis selector + cuts
   top.SetSelector("AtlSgTopFCNC_tZ_Multi")
   add_regions(top)

   # Object Selection Tool
   tool = ROOT.AtlObjectsToolD3PDSgTop("D3PDObjects", "Analysis objects")
   ROOT.SetOwnership(tool, False)
   top.AddTool(tool)
   top.SetToolParameter("D3PDObjects", "fJetsAuthor", "AtlJet::kAntiKt4TopoEM")
   top.SetToolParameter("D3PDObjects", "fJetPt_min", "30.")
   top.SetToolParameter("D3PDObjects", "fJetEta_max", "4.5")
   top.SetToolParameter("D3PDObjects", "fBJetEta_max", "2.5")
   top.SetToolParameter("D3PDObjects", "fElectronPt_min", "15.")
   top.SetToolParameter("D3PDObjects", "fMuonPt_min", "15.")
   top.SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_77")
   top.SetToolParameter("D3PDObjects", "fBTagWeight_min", "0")

   # Object Reco SF Tool
   tool = ROOT.AtlObjRecoScaleFactorTool("SFTool", "SF tool")
   ROOT.SetOwnership(tool, False)
   top.AddTool(tool)
   top.SetToolParameter("SFTool", "fBTagWP", "AtlObjRecoScaleFactorTool::kMV2c10_77")
   top.SetToolParameter("SFTool", "fOperationMode", "AtlObjRecoScaleFactorTool::kAllSF") # Check with D3PDObjects config!
   top.SetToolParameter("SFTool", "fUseCDI", "kFALSE") # Do not use CDI interface

   # Fake-lepton tool
   if opts.what.startswith('fakes'):
      tool = ROOT.AtlBDecayGenTool("BDecayGenTool", "B-meson decay generation")
      ROOT.SetOwnership(tool, False)
      top.AddTool(tool)
      top.SetToolParameter("BDecayGenTool", "fValidDSIDs", '"%s"' % ','.join(map(str, map(abs, samples_for_fakes))))
      top.SetToolParameter("BDecayGenTool", "fBJetMode", {
            'fakes': "AtlBDecayGenTool::kHalfRandom",
            'fakes_low': "AtlBDecayGenTool::kLowPt",
            'fakes_random': "AtlBDecayGenTool::kRandom",
         }[opts.what])
      top.SetToolParameter("BDecayGenTool", "fRequireOverlapRemoval", 'kTRUE')

   # ==============
   # Setup Plotting
   # ==============
   top.SetPlotterScheme("tZ")
   top.SetDataLuminosity(36074.56) # pb^{-1}
   top.SetAtlasLabelStatus("Work in Progress")
   top.SetCenterOfMassEnergyLabel("13")
   top.SetLuminosityLabel("36.1 fb^{-1}")
   top.SetUseOverflowUnderflow(True)
   top.SetDrawDataZeroEntryErrors(False)
   top.SetDrawMCError(True)
   top.SetExportPDF(False)
   top.SetDrawSignalOverlay(True)
   top.SetScaleOverlay(1.)

   return top


def add_regions(task):
   """Add subselections for all our regions to a task."""

   inf = float('inf')

   def add_region(name, spec, **extra_cuts):

      # parse spec string
      m = re.match('''\s*
                     ([23])     l\s*
                     (\d+)(\+)? j\s*
                  (?:(\d+)(\+)? b\s*)?
                  (?:([01])     z\s*)?
            $''', spec, re.X)
      gs = m.groups()
      cast = lambda t, v: None if v is None else t(v)
      NLeptons = cast(int, gs[0])
      NJets = cast(int, gs[1])
      InclJets = cast(bool, gs[2])
      NBTags = cast(int, gs[3])
      InclBTags = cast(bool, gs[4])
      NZs = cast(int, gs[5])

      # set cuts according to spec
      cuts = {}
      cuts['fNLeptons_min'] = NLeptons
      cuts['fNLeptons_max'] = NLeptons
      cuts['fReconstructTop'] = (NLeptons >= 3)
      cuts['fNJets_min'] = NJets
      cuts['fNJets_max'] = (999 if InclJets else NJets)
      cuts['fNBTags_min'] = (0 if NBTags is None else NBTags)
      cuts['fNBTags_max'] = (999 if NBTags is None or InclBTags else NBTags)
      cuts['fRequireZ'] = (NZs == 1)
      cuts['fVetoZbosonLate'] = (NZs == 0)

      # default values for other cuts
      defaults = {}
      defaults['fPt_Lep1_min'] = (28. if NLeptons >= 1 else 0.)
      defaults['fPt_Lep2_min'] = (25. if NLeptons >= 2 else 0.)
      defaults['fPt_Lep3_min'] = (15. if NLeptons >= 3 else 0.)
      defaults['fMET_min'] = 0.
      defaults['fMET_max'] = inf
      defaults['fMtW_min'] = 0.
      defaults['fMtW_max'] = inf

      # add subselection using a merged set of cuts
      for var, val in extra_cuts.iteritems():
         if var in cuts:
            raise KeyError("cut value for %s already defined" % repr(var))
         cuts[var] = val
      for var, val in defaults.iteritems():
         if var in cuts:
            continue
         cuts[var] = val
      add_subselection(task, name, cuts)

   # 3l regions
   add_region('signal', '3l 1j 1b 1z')
   add_region('signal_MtWL', '3l 1j 1b 1z', fMtW_max=50.)
   add_region('signal_MtWH', '3l 1j 1b 1z', fMtW_min=50.)
   add_region('signal_2j', '3l 2j 1b 1z')
   add_region('signal_2j_MtWL', '3l 2j 1b 1z', fMtW_max=50.)
   add_region('signal_2j_MtWH', '3l 2j 1b 1z', fMtW_min=50.)
   add_region('signal_0z', '3l 1j 1b 0z')
   add_region('ttbar3', '3l 2+j 1+b 0z')
   add_region('ttbar3_2j', '3l 2j 1+b 0z')
   add_region('ttbar3_2j1b', '3l 2j 1b 0z')
   add_region('ttv3', '3l 3+j 1+b')
   add_region('wz', '3l 1j 0b 1z')
   add_region('wz_MtWL', '3l 1j 0b 1z', fMtW_max=50.)
   add_region('wz_MtWH', '3l 1j 0b 1z', fMtW_min=50.)
   add_region('wz_0z', '3l 1j 0b 0z')

   # partial unblinding
   add_region('signal_MtWL_PtZL', '3l 1j 1b 1z', fMtW_max=50., fPtZ_max=160.)
   add_region('signal_2j_PtZL', '3l 2j 1b 1z', fPtZ_max=80.)

   # 2l regions
   add_region('zjets2', '2l 1+j 1b 1z')
   add_region('z', '2l 1+j 1z')
   add_region('z_b', '2l 1+j 1+b 1z')
   add_region('z_1j', '2l 1j 1z')
   add_region('z_1j0b', '2l 1j 0b 1z')
   add_region('z_1j1b', '2l 1j 1b 1z')
   add_region('z_2j', '2l 2j 1z')
   add_region('z_2j0b', '2l 2j 0b 1z')
   add_region('z_2j1b', '2l 2j 1b 1z')
   add_region('z_2j2b', '2l 2j 2b 1z')
   add_region('ttbar2', '2l 2j 2b 0z')
   add_region('dil', '2l 1+j')
   add_region('dil_b', '2l 1+j 1+b')
   add_region('dil_1j', '2l 1j')
   add_region('dil_2j', '2l 2j')


def main(argv):
   parser = argparse.ArgumentParser()
   parser.add_argument('-n', '--name', action='store')
   parser.add_argument('-r', '--force-retry', action='store_true')
   parser.add_argument('-s', '--systematics', dest='systematics', action='store_true')
   parser.add_argument('-w', '--what', action='store', default='standard')
   parser.set_defaults(force_retry=False, systematics=False)
   opts = parser.parse_args(argv[1:])
   print 'Building top-level task ...'
   top = make_top_task(opts)
   print 'Building tasks ...'
   if opts.systematics:
      print '... this might take a while ...'
   top.BuildTree(False)
   print 'Modifying tasks ...'
   modify_tasks(top, opts)
   print 'Executing tasks ...'
   top.ExecuteTask()
   print 'Done.'


if __name__ == '__main__':
   load_ROOT()
   main(sys.argv)
