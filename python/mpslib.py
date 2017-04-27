'''
mpslib.py: a Python interface to MPSlib

Example:
    As simple example:

        $ python3 mpslib_example.py

===========
** Variables
par: dictionary with simulation paramers


** Functions
mpslib.run()
mpslib.run()

mpslib.mps_genesim_par_write():

mpslib.parfile_def(): set the default parameters based on par['method']
mpslib.parfile_def_general(): Set default parameters for all methods
mpslib.parfile_def_snesim(): Set default parameters for mps_snesim
mpslib.parfile_def_genesim(): set default parameters for mps_genesim

mpslib.print_par():

mpslib.about(): # write out license+


'''
import os;
import sys;
import numpy as np;
import subprocess;
import eas; # read and write EAS formatted files
 
# Some defaults
verbose_level=1; # controls the amount of output from mpslib.py


# default empty parameter dict
par = {};


# Check if on windows
iswin=0;
if (os.name == 'nt'):
    iswin=1;

sim=[];    
    

'''
    mpslib.run;
    Runs mpslib simulation algorithm
'''
def run():
    global par;
    global sim;
    
    # check that parameter file is setup, and add deafult parameter values if not set
    parfile_def();
    
    # write the parameter file
    if ( par['method']=='mps_genesim' ):
        mps_genesim_par_write()
    else:
        mps_snesim_par_write()
    
    exefile = par['method'];
    if iswin:
        exefile = exefile + '.exe';
        
    # run mpslob    
    cmd = os.path.join(par['mpslib_exe_folder'],exefile)
    print ("mpslib: trying to run  " + cmd + " " + par['parameter_filename']);
    stdout = subprocess.run([cmd,par['parameter_filename']], stdout=subprocess.PIPE);
    
                             
    # read on simulated data
    sim=[];
    for i in range(0, par['n_real']):
        filename = '%s_sg_%d.gslib' % (par['ti_filename'],i);
        # print ('mpslib: Reading: %s' % (filename) );
        eas_dict = eas.read(filename);
        sim.append(eas_dict['Dmat']);
              
    
    return stdout;
    
    
    

#%% Write GENESIM type parameter file    
#def mps_genesim_par_write():
#    global par;
#    mps_genesim_par_write(par)
    
def par_write():
    global verbose_level;
    global par;
        
    # check parfile
    parfile_def();
    
    if ( par['method']=='mps_genesim' ):
        mps_genesim_par_write()
    else:
        mps_snesim_par_write()
    
    
def mps_genesim_par_write():
    global verbose_level;
    
    cwd = os.getcwd();
    full_path = os.path.join(os.getcwd(),par['parameter_filename'])
    
    
    if (verbose_level>0):
        print("mpslib: Writing GENESIM type parameter file: "+full_path);
    
    file = open(full_path,"w") 
 
    file.write("Number of realizations # %d\n" % par['n_real'])
    file.write("Random Seed (0 `random` seed) # %d \n" % par['rseed'])
    file.write("Maximum number of counts for condtitional pdf # %d\n" % par['n_max_cpdf_count'])
    file.write("Max number of conditional point # %d\n" % par['n_cond'])
    file.write("Max number of iterations # %d\n" % par['n_max_ite'])
    file.write("Distance measure [1:disc, 2:cont], minimum distance, power # %d %3.1f %3.1f\n" % (par['distance_measure'],par['distance_min'],par['distance_pow']))
    file.write("Max Search Radius for conditional data # %f \n" % par['max_search_radius'])
    file.write("Simulation grid size X # %d\n" % par['simulation_grid_size'][0])
    file.write("Simulation grid size Y # %d\n" % par['simulation_grid_size'][1])
    file.write("Simulation grid size Z # %d\n" % par['simulation_grid_size'][2])
    file.write("Simulation grid world/origin X # %g\n" % par['origin'][0])
    file.write("Simulation grid world/origin Y # %g\n" % par['origin'][1])
    file.write("Simulation grid world/origin Z # %g\n" % par['origin'][2])
    file.write("Simulation grid grid cell size X # %g\n" % par['grid_cell_size'][0])
    file.write("Simulation grid grid cell size Y # %g\n" % par['grid_cell_size'][1])
    file.write("Simulation grid grid cell size Z # %g\n" % par['grid_cell_size'][2])
    file.write("Training image file (spaces not allowed) # %s\n" % par['ti_filename'])
    file.write("Output folder (spaces in name not allowed) # %s\n" %  par['output_folder'])
    file.write("Shuffle Simulation Grid path (1 : random, 0 : sequential) # %d\n" % par['shuffle_simulation_grid'])
    file.write("Shuffle Training Image path (1 : random, 0 : sequential) # %d\n" % par['shuffle_ti_grid'])
    file.write("HardData filename  (same size as the simulation grid)# %s\n" % par['hard_data_filename'])
    file.write("HardData seach radius (world units) # %g\n" % par['hard_data_search_radius'])
    file.write("Softdata categories (separated by ;) # %s\n" % par['soft_data_categories'])
    file.write("Soft datafilenames (separated by ; only need (number_categories - 1) grids) # %s\n" % par['soft_data_filename'])
    file.write("Number of threads (not currently used) # %d\n" % par['n_threads'])
    file.write("Debug mode(2: write to file, 1: show preview, 0: show counters, -1: no ) # %d\n" % par['debug_level'])
    
    file.close();     
        
    return 1;


    
def mps_snesim_par_write():
    global verbose_level;
    
    cwd = os.getcwd();
    full_path = os.path.join(os.getcwd(),par['parameter_filename'])
    
    
    if (verbose_level>0):
        print("mpslib: writing SNESIM type parameter file: "+full_path);
    
    file = open(full_path,"w") 
 
    file.write("Number of realizations # %d\n" % par['n_real'])
    file.write("Random Seed (0 `random` seed) # %d \n" % par['rseed'])
    file.write("Number of mulitple grids (start from 0) # %d\n" % par['n_multiple_grids']);
    file.write("Min Node count (0 if not set any limit)# 0\n")
    file.write("Maximum number condtitional data (0: all) # -1\n")
    file.write("Search template size X # 9\n")    
    file.write("Search template size Y # 9\n")
    file.write("Search template size Z # 1\n")
    file.write("Simulation grid size X # %d\n" % par['simulation_grid_size'][0])
    file.write("Simulation grid size Y # %d\n" % par['simulation_grid_size'][1])
    file.write("Simulation grid size Z # %d\n" % par['simulation_grid_size'][2])
    file.write("Simulation grid world/origin X # %g\n" % par['origin'][0])
    file.write("Simulation grid world/origin Y # %g\n" % par['origin'][1])
    file.write("Simulation grid world/origin Z # %g\n" % par['origin'][2])
    file.write("Simulation grid grid cell size X # %g\n" % par['grid_cell_size'][0])
    file.write("Simulation grid grid cell size Y # %g\n" % par['grid_cell_size'][1])
    file.write("Simulation grid grid cell size Z # %g\n" % par['grid_cell_size'][2])
    file.write("Training image file (spaces not allowed) # %s\n" % par['ti_filename'])
    file.write("Output folder (spaces in name not allowed) # %s\n" %  par['output_folder'])
    file.write("Shuffle Simulation Grid path (1 : random, 0 : sequential) # %d\n" % par['shuffle_simulation_grid'])
    file.write("Shuffle Training Image path (1 : random, 0 : sequential) # %d\n" % par['shuffle_ti_grid'])
    file.write("HardData filename  (same size as the simulation grid)# %s\n" % par['hard_data_filename'])
    file.write("HardData seach radius (world units) # %g\n" % par['hard_data_search_radius'])
    file.write("Softdata categories (separated by ;) # %s\n" % par['soft_data_categories'])
    file.write("Soft datafilenames (separated by ; only need (number_categories - 1) grids) # %s\n" % par['soft_data_filename'])
    file.write("Number of threads (not currently used) # %d\n" % par['n_threads'])
    file.write("Debug mode(2: write to file, 1: show preview, 0: show counters, -1: no ) # %d\n" % par['debug_level'])
    
    file.close();     
        
    return 1;
    

    
    
#%% DFEAULT general MPS parameters 
def parfile_def():
    global par;
    parfile_def_general()
    if ( par['method']=='mps_genesim' ):
        parfile_def_genesim()
    else:
        parfile_def_snesim()
     

def parfile_def_general():
    global par;
    
    par.setdefault('parameter_filename', "mps.txt")
    
    par.setdefault('method','mps_genesim');
    
    par.setdefault('rseed',1);
    par.setdefault('debug_level',1);
    par.setdefault('n_threads', 1);
    par.setdefault('n_real',1);
    par.setdefault('rseed',1);
    par.setdefault('output_folder','.')
    par.setdefault('ti_filename','ti.dat')
    par.setdefault('simulation_grid_size', np.array([60,80,1]))
    par.setdefault('origin', np.array([0, 0, 0]))
    par.setdefault('grid_cell_size', np.array([1, 1, 1]))
    par.setdefault('hard_data_filename',"d_hard.dat")
    par.setdefault('shuffle_simulation_grid', 1)
    par.setdefault('entropyfactor_simulation_grid', 4)
    par.setdefault('shuffle_ti_grid', 1)
    par.setdefault('hard_data_search_radius', 1)
    par.setdefault('soft_data_categories', "0;1")
    par.setdefault('soft_data_filename', "soft.dat");
    par.setdefault('n_threads', 1);
         
    # default mpslib folder with executables
    # the oflder below the location of the Pyhton module    
    par.setdefault('mpslib_exe_folder',os.path.join(os.path.dirname('mpslib.py'),'..'))
    
    return 1
    

#% DFEAULT parameter file setting using  GENESIM
def parfile_def_genesim():
    global par;
    
    par.setdefault('n_cond', 25)
    par.setdefault('n_max_ite', 10000)
    par.setdefault('n_max_cpdf_count', 10)
    par.setdefault('distance_measure', 1)
    par.setdefault('distance_min', 0)
    par.setdefault('distance_pow', 0)
    par.setdefault('max_search_radius', 1000000)
    
    return 1
    
#% DFEAULT parameter file setting using  SNESIM
def parfile_def_snesim():
    global par;
    
    par.setdefault('template_size', np.array([5, 5, 1]))
    par.setdefault('n_multiple_grids', 3)
    par.setdefault('n_min_node_count',0)
    par.setdefault('n_cond', -1)
    
    return 1

#%% Print the parameter file to screen
def print_par():
    global par;
    print("mpslib: The parameter file:")
    for key, value in par.items() :
        print (key,'=',value);
           
    
#%% Write out the license
def about(): # write out license+
    license = """
     (c) 2015-2017 I-GIS (www.i-gis.dk) and Solid Earth Geophysics, Niels Bohr Institute (http://imgp.nbi.ku.dk)
    
        This file is part of MPSlib.
    
    
        MPSlib is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.
    
        MPSlib is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.
    
        You should have received a copy of the GNU Lesser General Public License
        along with MPSlib (COPYING.LESSER).  If not, see <http://www.gnu.org/licenses/>.
    """
    print(license)
    return license
    
        
'''
Initial stuff
'''        

# set default parfile
parfile_def();

# get/set the simulation algoritm
if (len(sys.argv)>1):
    print(len(sys.argv))
    par['method'] = sys.argv[1];
else: 
    par['method'] = 'mps_snesim_tree';

        
# get/set the parameter filename
if (len(sys.argv)>2):
    print(len(sys.argv))
    par['parameter_filename'] = sys.argv[2];
else: 
    par['parameter_filename'] = 'mps.par';
        
        
# Everything load and setup.. write information
if (verbose_level>0):
    print ("mpslib.py loaded")
    print ("mpslib: executable folder: ",par['mpslib_exe_folder'])
    print ("mpslib: method (algorithm) = "+ par['method'])
    print ("mpslib: parameter file = "+par['parameter_filename'])
    if (verbose_level>1):
        print_par();

