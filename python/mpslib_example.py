# mpslib_example.py
# example of using mpslib.py
import mpslib as mps;
#import matplotlib.pylab as plt;
import matplotlib.pyplot as plt;
import eas;

#plt.ion();
#%% 
mps.par['method'] = 'mps_snesim_tree'; # default
#mps.par['method'] = 'mps_snesim_list'; # 
#mps.par['method'] = 'mps_genesim'; # 
#mps.par['n_max_ite']=1000;

# optionally set some options
mps.par['parameter_filename']='snesim.par';
mps.par['simulation_grid_size']=mps.np.array([70,44,1]);
mps.par['n_real']=9;
out = mps.run();

#%%
for i in range(0, mps.par['n_real']):
    plt.subplot(3,3,i+1)
    plt.imshow(mps.sim[i])
    
plt.show(block=False);

#%%
#file_eas='ti.dat_sg_1.gslib';
#D = eas.read(file_eas)


#%% RUN GENESIM
mps.par['method'] = 'mps_genesim'; # default
mps.par['parameter_filename']='genesim.par';
mps.par['n_cond']=25;
out = mps.run();


#%%
font = {'family' : 'Ubuntu',
        'weight' : 'bold',
        'size'   : 8}
plt.rc('font', **font)
#plt.rcParams.update({'font.size': 6})

fig = plt.figure(2)
plt.set_cmap('gray')
fig.suptitle("Title for whole figure", fontsize=16)
    

for i in range(0, mps.par['n_real']):
    ax = plt.subplot(3,3,i+1)
    ax.set_title('Realization #%d' % i)
    ax.imshow(mps.sim[i],interpolation='none')
        
plt.savefig('test.png', dpi=600)
plt.show();
