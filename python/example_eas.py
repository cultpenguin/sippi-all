# mpslib_example.py
# example of using mpslib.py
import mpslib as mps;
import numpy as np;
import eas;
import matplotlib.pyplot as plt;

#%%
mps.par['method'] = 'mps_genesim'; # 

# optionally set some options
#mps.par['parameter_filename']='snesim.par';
mps.par['simulation_grid_size']=mps.np.array([39,39,1]);
mps.par['n_real']=5;
out = mps.run();


#%%
file_eas='ti.dat_sg_0.gslib';
eas = eas.read(file_eas)

plt.imshow(eas['Dmat'])