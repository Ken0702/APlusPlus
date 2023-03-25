syst_short_list_file = '/mnt/eeh/kaphle/atlas/APlusPlus_git/libSingleTop/tasks/13TeV/s_channel/systematics_v25.txt'
d_syst_toshort = {}
d_syst_tolong = {}
l_syst_long = []
with open(syst_short_list_file) as f:
    for line in f:
        if line.startswith('#') or line.isspace():
            continue
        s_long, s_short = line.split()
        d_syst_toshort[s_long] = s_short
        d_syst_tolong[s_short] = s_long
        l_syst_long.append(s_long)


for syst in l_syst_long:
    print('        main->AddSystematic2("{0}");'.format(syst))
