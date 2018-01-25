# mpslib_example_tmh
import sys
sys.path.append('mpslib')
import matplotlib.pyplot as plt
import mpslib as mps


#%% MPS_SNESIM_TREE
O1=mps.mpslib(method='mps_snesim_tree')
O1.par['debug_level']=-1
# when debug_level>-1 and the text output from mpslib increase, the mpslib exe file is detached from 
# python before it has finished running!!!
TI=mps.trainingimages.lines()
O1.par['ti_fnam']=TI['filename']
O1.par['n_cond']=81
O1.par['rseed']=0
O1.par['n_real']=9
O1.parameter_filename='test.par'
O1.run()


plt.set_cmap('hot')
fig1 = plt.figure(1)
for i in range(0, O1.par['n_real']):
    plt.subplot(3,3,i+1)
    plt.imshow(O1.sim[i], interpolation='none')

fig1.suptitle(O1.method, fontsize=16)
plt.savefig(O1.method+'.png', dpi=600)
plt.show()
