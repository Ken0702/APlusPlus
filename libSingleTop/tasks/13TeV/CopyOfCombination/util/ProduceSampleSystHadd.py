import sys
import re

def LookForSampleConfigName(DSID):
    SampleFile = open("/users/eeh/kkreul/GridSubmission/SampleSyst_v34_Submission/sample_config_v34_SampleSyst.json")
    prevline = ""
    for line in iter(SampleFile.readline, b''):
       regex = re.compile('\d\d\d\d\d\d')
       out = regex.findall(line)
       if(out):
           if(out[0] == DSID):
               prevline = prevline.strip()
               prevline = prevline.strip("\":{")
               prevline = prevline.strip()
               prevline = prevline.strip("\":")
               #print(prevline)
               break
           
           
       prevline = line
       
    Name = prevline
    Name = Name[:-2]  
    
    return Name

def ProduceHaddStatement(DSID, CorrectName, SampleConfigName):
    Statement = "hadd -f ../../run_check/ntup_schan_2j_lnu_nominal_"
    Statement += CorrectName
    Statement += ".root  user.kkreul.NewSampleSyst."
    Statement += SampleConfigName
    Statement += "_[1-10]_nominal_v34lj_MC16a.v1_mem_output.root/* \n"
    #print(Statement)
    
    #hadd ../../run_check/ntup_schan_2j_lnu_nominal_410654WtDS.root   user.kkreul.SampleSyst.tWPP8FS_[1-4]_nominal_v34lj_MC16a.v2_mem_output.root/*
    return Statement

def main():
    DSIDFile = open("/users/eeh/kkreul/APlusPlus/libSingleTop/tasks/13TeV/SampleList/SgTopDSIDs_mc16a_v34_lj_samplesyst.txt")
    OutFile = open("HaddSampleSysts.sh", "w")
    OutFile.write("""#!/bin/bash

CWD=$(pwd)
cd /rdsk/dats1/atlas/kkreul/combination/A++/v02_v34lj_sampleSysts_MC16a/rucio/download
""")

    for line in iter(DSIDFile.readline, b''):
       if(  line.startswith('#')  ):
           continue
       if not line.strip() :
           continue
       line = line.strip('\n')
       regex = re.compile('\d\d\d\d\d\d')
       out = regex.findall(line)
       CorrectName = line.split(",")[0]
       DSID = out[0]
       SampleConfigName = LookForSampleConfigName(DSID)
       Statement = ProduceHaddStatement(DSID, CorrectName, SampleConfigName)
       OutFile.write(Statement)
    OutFile.write("cd $CWD")
   
   


if __name__ == '__main__':
    sys.exit(main())

