import os
import glob

import mpslib as mps
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

import numpy as np

TI1, TI_filename1 = mps.trainingimages.strebelle(2, coarse3d=1)


O1=mps.mpslib(method='mps_snesim_tree')
#O1.ti=TI1
O1.par['n_cond']=9
O1.par['n_real']=25
O1.par['debug_level']=-1
O1.par['simulation_grid_size'][0]=35
O1.par['simulation_grid_size'][1]=30
O1.par['hard_data_fnam']='hard.dat'

d_hard = np.array([[ 25.,  10.,   0.,   1.],
       [  5.,  10.,   0.,   1.],
       [  7.,  10.,   0.,   1.],
       [ 15.,  25.,   0.,   0.]])
O1.d_hard = d_hard
O1.par['hard_data_fnam']='h.dat'

O1.run()
O1.delete_gslib()

O1.plot_reals()
O1.plot_etype()

'''

#%%
nr=25
nr  = np.min((O1.par['n_real'], nr))
nsp = int(np.ceil(np.sqrt(nr)))

fig = plt.figure(1)
sp = gridspec.GridSpec(nsp, nsp , wspace=0.1, hspace=0.1)
plt.set_cmap('hot')
for i in range(0, nr):
    ax1 = plt.Subplot(fig, sp[i])
    fig.add_subplot(ax1)
    plt.imshow(O1.sim[i], interpolation='none')
    plt.title("Real %d" % (i + 1))

fig.suptitle(O1.method+' - '+O1.parameter_filename, fontsize=16)
plt.show(block=False)


file_list = glob.glob('%s/%s*.gslib' % (O1.par['out_folder'],O1.par['ti_fnam']))
for file in file_list:
    os.remove(os.path.join(O1.par['out_folder'], file))
    print('Removing {0}'.format(os.path.join(O1.par['out_folder'], file)))
'''

