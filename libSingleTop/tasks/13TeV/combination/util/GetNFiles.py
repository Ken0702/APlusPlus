import sys
import os
import subprocess
import re

""" Usage : First run ./getnevts.sh > nevts.txt 
then run this 
clean up the mc16a file: remove systs samples"""


def GetEvts(DSID):
    evts = 0
    File = open("sampleSyst.txt")
    for line in iter(File.readline, b''):
        #print DSID
        #print line
        line = line.split(";")
        #print line
        
        if(line[0].startswith(DSID)):
            evts = line[1]
            break
            
    #print evts
    return evts


def WriteToFile(out):
    File = open("NEVTPERFILE_sampleSyst.txt","a")
    File.write("{0}\t\t{1}\t\t{2}\t\t{3}\n".format(out[0],out[1], out[2], out[3]))
    return 0 
    
    
def ReadFiles():

   File = open("mc16a_v34.txt")
   #print(File.readline())
   for line in iter(File.readline, b''):
       if(  line.startswith('#')  ):
           continue
       line = line.strip('\n')
       #print line
       #cmd = 'rucio list-files ' + line + '| grep \"Total files\" | awk \'{print  $4}\' >> nFiles.txt'
       #print cmd
       cmd = 'rucio list-files ' + line
       #Command = subprocess.Popen(cmd , stdout=subprocess.PIPE, shell=True)
       #cmd = "ls"
       #print(cmd)
       Command = subprocess.Popen(cmd , stdout=subprocess.PIPE, shell=True)
       Full = Command.stdout.read()
       index = Full.find("Total files")
       nFiles_str = Full[index+14:index+18]
       #print(nFiles_str)
       if (nFiles_str.endswith("\n")):
           nFiles_str = nFiles_str[0:3]
       if (nFiles_str.endswith("T")):
           nFiles_str = nFiles_str[0:2]
       if (nFiles_str.endswith("To")):
           nFiles_str = nFiles_str[0:1]
       #print nFiles_str
       
       regex = re.compile('\d\d\d\d\d\d')
       out = regex.findall(line)
       if (not out):
           continue
       out.append(nFiles_str)
       
       evts = GetEvts(out[0])
       out.append(evts)
       
       nEvtsPerFile = float(out[2])/float(out[1])
       out.append(nEvtsPerFile)
       #print out
       #print nEvtsPerFile
       WriteToFile(out)
   return out



if __name__ == '__main__':
   out = ReadFiles()
   WriteToFile(out)
   #sys.exit(main())
