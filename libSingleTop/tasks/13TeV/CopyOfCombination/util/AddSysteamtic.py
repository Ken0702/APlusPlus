import sys

def AddSyst():
    File = open("systematics_v34_updated.txt")
    for line in iter(File.readline, b''):
        if(  line.startswith('#')  ):
            continue
        line = line.strip('\n')	
        syst = line.split("   ")
        print("main->AddSystematic2(\"" + syst[0] + "\");")

    return



if __name__ == '__main__':
   sys.exit(AddSyst())

