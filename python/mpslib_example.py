# mpslib_example.py
# example of using mpslib.py
import mpslib as mps;
# import mpslib_tmh as mps_tmh;
import matplotlib.pyplot as plt;
import importlib
importlib.reload(mps)
plt.ion();

#%%

mpsobj = mps.mpslib(method='mps_snesim_tree',hard_data_fnam = 'mps_2d_hard_data.dat',
                    n_real = 2, verbose_level=1)

mpsobj.par['n_max_cpdf_count'] = 1
mpsobj.par['n_max_ite'] = 1000000000
mpsobj.parameter_filename = 'mps_snesim.txt'

#mpsobj.par_write()
mpsobj.run_model()

plt.set_cmap('hot')
fig1 = plt.figure(1)
for i in range(0, mpsobj.par['n_real']):
    plt.subplot(3,3,i+1)
    plt.imshow(mpsobj.sim[i], interpolation='none')

fig1.suptitle(mpsobj.method, fontsize=16)
plt.show();




#%% RUN GENESIM
Og = mps.mpslib(method='mps_genesim')
#Og.par['method'] = 'mps_genesim'; # When changing method, 'par' should be re-initialized..
Og.par['n_real']=4;
Og.par['n_cond']=81;
Og.parameter_filename = 'mps_genesim.txt'
#Og.par_write()
Og.run_model();


#%%
font = {'family' : 'Ubuntu',
        'weight' : 'bold',
        'size'   : 8}
plt.rc('font', **font)
#plt.rcParams.update({'font.size': 6})

fig = plt.figure(2)
plt.set_cmap('gray')
fig.suptitle(Og.method, fontsize=16)
    

for i in range(0, Og.par['n_real']):
    ax = plt.subplot(3,3,i+1)
    ax.set_title('Realization #%d' % i)
    ax.imshow(Og.sim[i],interpolation='none')
        
plt.savefig('test.png', dpi=600)
plt.show();
plt.pause(10);