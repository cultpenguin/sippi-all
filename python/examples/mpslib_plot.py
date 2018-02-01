# -*- coding: utf-8 -*-
"""
Created on Thu Feb  1 20:44:17 2018

@author: thoma
"""

#%load_ext autoreload
#%autoreload 2
#%%

import sys
#sys.path.append('../mpslib')
#import matplotlib.pyplot as plt
import mpslib as mps
import numpy as np

O1=mps.mpslib(method='mps_snesim_tree')
O1.par['debug_level']=-1
# when debug_level>-1 and the text output from mpslib increase, the mpslib exe file is detached from 
# python before it has finished running!!!
TI, TI_filename=mps.trainingimages.checkerboard2()
O1.ti = TI
O1.par['ti_fnam']=TI_filename
O1.par['simulation_grid_size'][0]=35
O1.par['simulation_grid_size'][1]=35
O1.par['simulation_grid_size'][2]=1
O1.par['soft_data_filename']='soft_case2.dat'
O1.par['shuffle_simulation_grid']=2
O1.par['n_cond']=9
O1.par['rseed']=0
O1.par['n_real']=41
O1.parameter_filename='plot.par'
O1.run()



O1.plot_reals(O1.par['n_real'])

#%% plot realizations
%load_ext autoreload
%autoreload 2

import matplotlib.pyplot as plt
import mpslib as mps
import numpy as np


def plot_reals(O1,nshow=9):
    import matplotlib.pyplot as plt
    import seaborn as sns
    import numpy as np
    sns.set()
    #plt.ion()
    plt.figure(1)
    
    
    plt.subplot(3, 3, 1)
    plt.imshow(TI, interpolation='none')
    plt.title(O1.par['ti_fnam'])
    
    plt.set_cmap('hot')
    fig1 = plt.figure(1)
    for i in range(1, np.min((O1.par['n_real'],6))):
        plt.subplot(3,3,i+1)
        plt.imshow(O1.sim[i], interpolation='none')
        #sns.heatmap(O1.sim[i])
        plt.title("Real %d" % i)
    fig1.suptitle(O1.method, fontsize=16)
    plt.show()
    
O1.plot_reals()