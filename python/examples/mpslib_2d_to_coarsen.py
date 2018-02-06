#

import numpy as np
import matplotlib.pyplot as plt
import mpslib as mps

plt.ion()



di=3 # Use every di'th data
# NO coarsening --> 1 2D TI
#TI1, TI_filename1 = mps.trainingimages.lines(di, coarse3d=0)
TI1, TI_filename1 = mps.trainingimages.strebelle(di, coarse3d=0)
#TI1, TI_filename1 = mps.trainingimages.bangladesh(di, coarse3d=0)
# Coarsening --> multiple 2D TI
#TI2, TI_filename2 = mps.trainingimages.lines(di, coarse3d=1)
TI2, TI_filename2 = mps.trainingimages.strebelle(di, coarse3d=1)
#TI2, TI_filename2 = mps.trainingimages.bangladesh(di, coarse3d=1)
mps.eas.write_mat(TI1,'ti1.dat')
mps.eas.write_mat(TI2,'ti2.dat')


#%%
fig, axs = plt.subplots(1,2, figsize=(10, 6), facecolor='w', edgecolor='k')
plt.subplot(121)
plt.imshow(TI1)
plt.subplot(122)
plt.imshow(TI2[:,:,0])
plt.show()


alg='mps_snesim_tree'
#alg='mps_genesim'
nc=5*5
rseed=3;
#nc=8*8
#Coarsen channel TI, using only coarsened TI
O1=mps.mpslib(method=alg)
O1.par['debug_level']=-1
O1.ti = TI1
O1.par['ti_fnam']='ti1.dat'
O1.par['simulation_grid_size'][0]=85
O1.par['simulation_grid_size'][1]=45
O1.par['simulation_grid_size'][2]=1
O1.par['shuffle_simulation_grid']=2
O1.par['n_cond']=nc
O1.par['rseed']=rseed
O1.par['n_real']=1
O1.parameter_filename='sim1.par'
O1.run()

#Coarsen channel TI, using all di*di coarsened TI
O2=mps.mpslib(method=alg)
O2.par['debug_level']=-1
O2.ti = TI2
O2.par['ti_fnam']='ti2.dat'
O2.par['simulation_grid_size'][0]=85
O2.par['simulation_grid_size'][1]=45
O2.par['simulation_grid_size'][2]=1
O2.par['shuffle_simulation_grid']=2
O2.par['n_cond']=nc
O2.par['rseed']=rseed
O2.par['n_real']=1
O2.parameter_filename='sim2.par'
O2.run()


#%%
fig, axs = plt.subplots(2,1, figsize=(8, 10), facecolor='w', edgecolor='k')
plt.subplot(211)
plt.imshow(O1.sim[0])
plt.title('Real using Single coarse TI')
plt.subplot(212)
plt.imshow(O2.sim[0])
plt.title('Real using Multiple coarse TI')
plt.suptitle("%s - di=%d" % (alg,di))

