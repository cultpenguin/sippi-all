# mpslib_example.py
# example of using mpslib.py

import sys
sys.path.append('mpslib')

#import mpslib.trainingimages as ti 
#print(dir(ti))


#%%
import mpslib as mps;

print(dir(mps))

# import mpslib_tmh as mps_tmh;
#import matplotlib.pyplot as plt;
#plt.ion();

#%% MPS_SNESIM_TREE

O1 = mps.mpslib(method='mps_snesim_tree',n_real = 2, verbose_level=-1)
O1.par['debug_level']=-1;
O1.parameter_filename = 'mps_snesim.txt'
O1.run()

