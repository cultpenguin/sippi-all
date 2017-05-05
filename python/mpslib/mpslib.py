import numpy as np
import os
import subprocess
#import mpslib.eas as eas
from . import eas as eas
import time

def is_exe(fpath):
    return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

def which(program):
    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        # test for exe in current working directory
        if is_exe(program):
            return program
        # test for exe in path statement
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None

class mpslib:

    def __init__(self, parameter_filename = 'mps.txt', method = 'mps_genesim', debug_level = 1, n_real = 1, rseed = 1,
                out_folder = '.', ti_fnam = 'ti.dat', simulation_grid_size = np.array([80, 40, 1]),
                origin = np.zeros(3), grid_cell_size = np.array([1, 1, 1]),hard_data_fnam = 'd_hard.dat',
                shuffle_simulation_grid =1, entropyfactor_simulation_grid=4, shuffle_ti_grid=1, hard_data_search_radius = 1,
                soft_data_categories= np.arange(2), soft_data_filename = 'soft.dat', n_threads = 1, verbose_level = 0, 
                template_size = np.array([8, 7, 1]), n_multiple_grids=3, n_cond=36, n_min_node_count=0,
                n_max_ite=1000000, n_max_cpdf_count=1, distance_measure=1, distance_min=0, distance_pow=1,
                max_search_radius=10000000):
        '''Initialize variables in Class'''
        self.mpslib_exe_folder = os.path.join(os.path.dirname('mpslib.py'),'..')
        self.blank_grid = None
        self.blank_val = np.NaN
        self.parameter_filename = parameter_filename.lower() # change string to lower case
        self.method = method.lower() # change string to lower case
        self.verbose_level = verbose_level
        self.sim = None
        
        self.par = {}
       
        self.par['n_real'] = n_real
        self.par['rseed'] = rseed
        self.par['n_max_cpdf_count'] = n_max_cpdf_count
        self.par['out_folder'] = out_folder
        self.par['ti_fnam'] = ti_fnam.lower() # change string to lower case
        self.par['simulation_grid_size'] = simulation_grid_size
        self.par['origin'] = origin
        self.par['grid_cell_size'] = grid_cell_size
        self.par['hard_data_fnam'] = hard_data_fnam.lower() # change string to lower case
        self.par['shuffle_simulation_grid'] = shuffle_simulation_grid
        self.par['entropyfactor_simulation_grid'] = entropyfactor_simulation_grid
        self.par['shuffle_ti_grid'] = shuffle_ti_grid
        self.par['hard_data_search_radius'] = hard_data_search_radius
        self.par['soft_data_categories'] = soft_data_categories
        self.par['soft_data_filename'] = soft_data_filename.lower() # change string to lower case
        self.par['n_threads'] = n_threads
        self.par['debug_level'] = debug_level
        
        # if the method is GENSIM, add package specific parameters
        if self.method == 'mps_genesim':
            self.par['n_cond'] = n_cond
            self.par['n_max_ite'] = n_max_ite
            self.par['n_max_cpdf_count'] = n_max_cpdf_count
            self.par['distance_measure'] = distance_measure
            self.par['distance_min'] = distance_min
            self.par['distance_pow'] = distance_pow
            self.par['max_search_radius'] = max_search_radius
            #self.par['exe'] = 'mps_genesim'

        if self.method[0:10] == 'mps_snesim':
            self.par['template_size'] = template_size
            self.par['n_multiple_grids'] = n_multiple_grids
            self.par['n_min_node_count'] = n_min_node_count
            self.par['n_cond'] = n_cond
            #self.exe = 'mps_mps_snesim_tree'

        # Check if on windows
        self.iswin=0
        if (os.name == 'nt'):
            self.iswin=1


    #% Check parameter file setting using  GENESIM
    def parfile_check_genesim(self):
        
        self.par.setdefault('n_cond', 25)
        self.par.setdefault('n_max_ite', 10000)
        self.par.setdefault('n_max_cpdf_count', 10)
        self.par.setdefault('distance_measure', 1)
        self.par.setdefault('distance_min', 0)
        self.par.setdefault('distance_pow', 0)
        self.par.setdefault('max_search_radius', 1000000)
        
        
        
    #% Check parameter file setting using  SNESIM
    def parfile_check_snesim(self):
           
        self.par.setdefault('template_size', np.array([5, 5, 1]))
        self.par.setdefault('n_multiple_grids', 3)
        self.par.setdefault('n_min_node_count',0)
        self.par.setdefault('n_cond', -1)
    
    # Change simulation method        
    def change_method(self,method='mps_genesim'):  
        print(self.method)
        if method == 'mps_genesim':
            self.parfile_check_genesim()
            self.method=method
        if method[0:10] == 'mps_snesim':
            self.parfile_check_snesim()
            self.method=method


    def par_write(self):
        if self.method == 'mps_genesim':
            self.mps_genesim_par_write()
        elif self.method[0:10] == 'mps_snesim':
            self.mps_snesim_par_write()
        else:
            s = 'unknown method type: {}'.format(self.method)
            raise Exception(s)

    def mps_genesim_par_write(self):
        full_path = os.path.join(self.parameter_filename)

        if (self.verbose_level > 0):
            print('mpslib: Writing GENESIM type parameter file: ' + full_path);

        file = open(full_path, 'w')

        file.write('Number of realizations # %d\n' % self.par['n_real'])
        file.write('Random Seed (0 `random` seed) # %d \n' % self.par['rseed'])
        file.write('Maximum number of counts for condtitional pdf # %d\n' % self.par['n_max_cpdf_count'])
        file.write('Max number of conditional point # %d\n' % self.par['n_cond'])
        file.write('Max number of iterations # %d\n' % self.par['n_max_ite'])
        file.write("Distance measure [1:disc, 2:cont], minimum distance, power # %d %3.1f %3.1f\n" % (self.par['distance_measure'],self.par['distance_min'],self.par['distance_pow']))
        file.write("Max Search Radius for conditional data # %f \n" % self.par['max_search_radius'])
        file.write('Simulation grid size X # %d\n' % self.par['simulation_grid_size'][0])
        file.write('Simulation grid size Y # %d\n' % self.par['simulation_grid_size'][1])
        file.write('Simulation grid size Z # %d\n' % self.par['simulation_grid_size'][2])
        file.write('Simulation grid world/origin X # %g\n' % self.par['origin'][0])
        file.write('Simulation grid world/origin Y # %g\n' % self.par['origin'][1])
        file.write('Simulation grid world/origin Z # %g\n' % self.par['origin'][2])
        file.write('Simulation grid grid cell size X # %g\n' % self.par['grid_cell_size'][0])
        file.write('Simulation grid grid cell size Y # %g\n' % self.par['grid_cell_size'][1])
        file.write('Simulation grid grid cell size Z # %g\n' % self.par['grid_cell_size'][2])
        file.write('Training image file (spaces not allowed) # %s\n' % self.par['ti_fnam'])
        file.write('Output folder (spaces in name not allowed) # %s\n' % self.par['out_folder'])
        file.write('Shuffle Simulation Grid path (1 : random, 0 : sequential) # %d\n' % self.par['shuffle_simulation_grid'])
        file.write('Shuffle Training Image path (1 : random, 0 : sequential) # %d\n' % self.par['shuffle_ti_grid'])
        file.write('HardData filename  (same size as the simulation grid)# %s\n' % self.par['hard_data_fnam'])
        file.write('HardData seach radius (world units) # %g\n' % self.par['hard_data_search_radius'])
        file.write('Softdata categories (separated by ;) # ')
        for c, ii in enumerate(self.par['soft_data_categories']):
            if c == 0:
                file.write('%d' % ii)
            else:
                file.write('; %d' % ii)
        file.write('\n')
        file.write('Soft datafilenames (separated by ; only need (number_categories - 1) grids) # %s\n' %
                   self.par['soft_data_filename'])
        file.write('Number of threads (not currently used) # %d\n' % self.par['n_threads'])
        file.write('Debug mode(2: write to file, 1: show preview, 0: show counters, -1: no ) # %d\n' % self.par['debug_level'])
        file.close()


    def mps_snesim_par_write(self):
        cwd = os.getcwd();
        full_path = os.path.join(self.parameter_filename)

        if (self.verbose_level > 0):
            print('mpslib: writing SNESIM type parameter file: ' + full_path);

        file = open(full_path, 'w')

        file.write('Number of realizations # %d\n' % self.par['n_real'])
        file.write('Random Seed (0 `random` seed) # %d \n' % self.par['rseed'])
        file.write('Number of mulitple grids (start from 0) # %d\n' % self.par['n_multiple_grids'])
        file.write('Min Node count (0 if not set any limit)# %d\n' % self.par['n_min_node_count'])
        file.write('Maximum number condtitional data (0: all) # %d\n' % self.par['n_cond'])
        file.write('Search template size X # %d\n' % self.par['template_size'][0])
        file.write('Search template size Y # %d\n' % self.par['template_size'][1])
        file.write('Search template size Z # %d\n' % self.par['template_size'][2])        
        file.write('Simulation grid size X # %d\n' % self.par['simulation_grid_size'][0])
        file.write('Simulation grid size Y # %d\n' % self.par['simulation_grid_size'][1])
        file.write('Simulation grid size Z # %d\n' % self.par['simulation_grid_size'][2])
        file.write('Simulation grid world/origin X # %g\n' % self.par['origin'][0])
        file.write('Simulation grid world/origin Y # %g\n' % self.par['origin'][1])
        file.write('Simulation grid world/origin Z # %g\n' % self.par['origin'][2])
        file.write('Simulation grid grid cell size X # %g\n' % self.par['grid_cell_size'][0])
        file.write('Simulation grid grid cell size Y # %g\n' % self.par['grid_cell_size'][1])
        file.write('Simulation grid grid cell size Z # %g\n' % self.par['grid_cell_size'][2])
        file.write('Training image file (spaces not allowed) # %s\n' % self.par['ti_fnam'])
        file.write('Output folder (spaces in name not allowed) # %s\n' % self.par['out_folder'])
        file.write('Shuffle Simulation Grid path (1 : random, 0 : sequential) # %d\n' % self.par['shuffle_simulation_grid'])
        file.write('Shuffle Training Image path (1 : random, 0 : sequential) # %d\n' % self.par['shuffle_ti_grid'])
        file.write('HardData filename  (same size as the simulation grid)# %s\n' % self.par['hard_data_fnam'])
        file.write('HardData seach radius (world units) # %g\n' % self.par['hard_data_search_radius'])
        file.write('Softdata categories (separated by ;) # ')
        for c, ii in enumerate(self.par['soft_data_categories']):
            if c == 0:
                file.write('%d' % ii)
            else:
                file.write('; %d' % ii)
        file.write('\n')
        file.write('Soft datafilenames (separated by ; only need (number_categories - 1) grids) # %s\n' %
                   self.par['soft_data_filename'])
        file.write('Number of threads (not currently used) # %d\n' % self.par['n_threads'])
        file.write('Debug mode(2: write to file, 1: show preview, 0: show counters, -1: no ) # %d\n' % self.par['debug_level'])
        file.close()
        
       

    def run(self, normal_msg='Elapsed time (sec)', silent=False):
        """
        """
        

        # Write parameter file to disc
        self.par_write()

        exefile = self.method        
        if self.iswin:
            exefile = exefile + '.exe'
        
                
        # run mpslob    
        cmd = os.path.join(self.mpslib_exe_folder,exefile)
        if (self.verbose_level > 0):
            print ("mpslib: trying to run  " + cmd + " " + self.parameter_filename)
   
        # Make sure no terminal windows i shown n Windows     
        #si = subprocess.STARTUPINFO()
        #si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        CREATE_NO_WINDOW = 0x08000000
        
        #si.wShowWindow = subprocess.SW_HIDE # default
        t_start = time.time()
        #stdout = subprocess.run([cmd,self.parameter_filename], stdout=subprocess.PIPE);
        #stdout = subprocess.run([cmd,self.parameter_filename], stdout=subprocess.PIPE, startupinfo=si);
        stdout = subprocess.run([cmd,self.parameter_filename], stdout=subprocess.PIPE, creationflags=CREATE_NO_WINDOW)
        self.execution_time = time.time() - t_start
        if (self.verbose_level > 0):
            print ("mpslib: finished in  %3.1f seconds (%3.1g minutes) " % (self.execution_time,self.execution_time/60))
                     
        # read on simulated data
        self.sim=[]
        for i in range(0, self.par['n_real']):
            filename = '%s_sg_%d.gslib' % (self.par['ti_fnam'],i)
            OUT = eas.read(filename)                              
            if (self.verbose_level > 0):
                print ('mpslib: Reading: %s' % (filename) )
            self.sim.append(OUT['Dmat'])
    
        return stdout

    def run_model(self, normal_msg='Elapsed time (sec)', silent=False):
            """
            *Description:*\n
            This function runs the mpslib executable from python.

            :param normal_msg: last line write to screen upon  successful completion
            :param silent: boolean, determines if modle outputs should be written to the screen

            :return: returns boolean success if the model run has been completed
            """
            success = False

            self.par_write()

            exefile = self.method
            if self.iswin:
                exefile = exefile + '.exe'

            exe = which(exefile)
            exe_name = exefile

            if exe is None:
                s = 'The program {} does not exist or is not executable.'.format(exe_name)
                raise Exception(s)
            else:
                if not silent:
                    s = 'Using the following executable to run the model: {}'.format(exe)
                    print(s)

            if not os.path.isfile(os.path.join(self.parameter_filename)):
                s = 'The the mpslib input file does not exists: {}'.format(self.parameter_filename)
                raise Exception(s)

            proc = subprocess.Popen(exe + ' ' + self.parameter_filename, stdout=subprocess.PIPE)

            while success == False:
                line = proc.stdout.readline()
                c = line.decode('utf-8', errors='ignore')
                if c != '':
                    if normal_msg.lower() in c.lower():
                        success = True
                    c = c.rstrip('\r\n')
                    if not silent:
                        print('{}'.format(c))
                else:
                    break

            # read on simulated data
            self.sim = []
            for i in range(0, self.par['n_real']):
                filename = '%s_sg_%d.gslib' % (self.par['ti_fnam'], i)
                OUT = eas.read(filename)
                if (self.verbose_level > 0):
                    print('mpslib: Reading: %s' % (filename))
                self.sim.append(OUT['Dmat'])

            return success

    def blank_sim(self):
        if hasattr(self, 'sim') is False:
            s = 'Simulation results not imported'
            raise Exception(s)

        if self.blank_grid is None:
            s = 'No blanking grid defined'
            raise Exception(s)

        nsim = len(self.sim)
        sim_grid_size = np.shape(self.sim[0])
        blank_grid_size = np.shape(self.blank_grid)

        if sim_grid_size != blank_grid_size:
            s = "Blank grid with size {} incompatiable with sim. grid size {} ".format(sim_grid_size, blank_grid_size)
            raise Exception(s)

        blk = self.blank_grid == 0

        self.simblk = np.copy(self.sim)
        for ii in np.arange(nsim):
            self.simblk[ii][blk == True] = self.blank_val

    # Loads existing simulation results into class without running mps algorithm
    def load_sim(self):
        # Check if simulation results are already imported
        if self.sim is not None:
            s = 'Simulation reaults already imported'
            raise  Exception(s)

        self.sim = []
        for i in range(0, self.par['n_real']):
            filename = '%s_sg_%d.gslib' % (self.par['ti_fnam'], i)

            if os.path.isfile(filename) is False:
                s = '{} file not found'.format(filename)

            OUT = eas.read(filename)

            if (self.verbose_level > 0):
                print('mpslib: Reading: %s' % (filename))
            self.sim.append(OUT['Dmat'])





'''
    def to_file(self):
        nreal = self.nreal
        outname = self.ti_fnam + '_all.gslib'
        f = open(outname, 'w')
        f.write('%d %d %d\n' % (self.simulation_grid_size[0], self.simulation_grid_size[1], self.simulation_grid_size[2]))
        if nreal == 1:
            f.write('%d\n' % nreal)
            f.write('%s\n' % 'real1')
            for ii in np.arange(self.simulation_grid_size[0] * self.simulation_grid_size[1] * self.simulation_grid_size[2]):
                f.write('%d\n' % self.simulations[ii])
            f.close()
        else:
            f.write('%d\n' % nreal)
            for ii in np.arange(nreal):
                f.write('%s%d\n' % ('real', ii))

            for ii in np.arange(self.simulation_grid_size[0] * self.simulation_grid_size[1] * self.simulation_grid_size[2]):
                for jj in np.arange(nreal):
                    f.write('%d ' % self.simulations[ii, jj])
                f.write('\n')
        f.close()

'''
