# mpslib_example.py
# example of using mpslib.py
import mpslib as mps

#%% 
mps.par['method'] = 'mps_snesim_tree'; # default
#mps.par['method'] = 'mps_snesim_list'; # 
#mps.par['method'] = 'mps_genesim'; # 
#mps.par['n_max_ite']=1000;

# optionally set some options
mps.par['parameter_filename']='snesim.par';
mps.par['simulation_grid_size']=mps.np.array([40,40,1]);
mps.par['n_real']=5;
out = mps.run();



#%% RUN GENESIM
mps.par['method'] = 'mps_genesim'; # default
mps.par['parameter_filename']='genesim.par';
out = mps.run();


