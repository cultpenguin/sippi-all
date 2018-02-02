import numpy as np
import os
import subprocess
from . import eas as eas
#from . import trainingimages
import time

def is_exe(filename):
    return os.path.isfile(filename) and os.access(filename, os.X_OK)


#%% FUNCTIONS FOR PLOTTING OF EAS FILES


#%% THE CLASS
class mpslib:

    def __init__(self, parameter_filename = 'mps.txt', method = 'mps_genesim', debug_level = 1, n_real = 1, rseed = 1,
                out_folder = '.', ti_fnam = 'ti.dat', simulation_grid_size = np.array([80, 40, 1]),
                origin = np.zeros(3), grid_cell_size = np.array([1, 1, 1]),hard_data_fnam = 'd_hard.dat',
                shuffle_simulation_grid =1, entropyfactor_simulation_grid=4, shuffle_ti_grid=1, hard_data_search_radius = 1,
                soft_data_categories= np.arange(2), soft_data_filename = 'soft.dat', n_threads = 1, verbose_level = 0, 
                template_size = np.array([8, 7, 1]), n_multiple_grids=3, n_cond=36, n_min_node_count=0,
                n_max_ite=1000000, n_max_cpdf_count=1, distance_measure=1, distance_min=0, distance_pow=1,
                max_search_radius=10000000, ti=np.empty(0)):
        '''Initialize variables in Class'''
        
        mpslib_py_path,fn = os.path.split(__file__)
        self.mpslib_exe_folder = os.path.abspath(os.path.join(mpslib_py_path,'..','..'))
        #self.mpslib_exe_folder = os.path.join(os.path.dirname('mpslib.py'),'..')
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





    def which(self,program):
        '''
        self.which: Locate executable in the following order:
            1) current directotu
            2) MPSlib installation directory
            3) In system path
        '''
        # check if executable is found in the current folder
        if is_exe(program):
            return program
        else:
            # Check of executable is located in MPLSIB folder
            program_path_mpslib = os.path.abspath(os.path.join(self.mpslib_exe_folder,program))
            if is_exe(program_path_mpslib):
                return program_path_mpslib            
            else:
                # Check of executable is located in system path
                for path in os.environ["PATH"].split(os.pathsep):                    
                    path = path.strip('"')
                    exe_file = os.path.join(path, program)
                    if is_exe(exe_file):
                        return exe_file
                        
                print("mpslib: " + program + " not found")
                return None
                


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
            *Description:*\n
            This function runs the mpslib executable from python.

            :param normal_msg: last line write to screen upon  successful completion
            :param silent: boolean, determines if modle outputs should be written to the screen

            :return: returns boolean success if the model run has been completed
            """
            success = False

            # write training image is set
            if self.ti.shape[0]>0:
                #self.par['ti_fnam']='TrainingImage.dat'
                eas.write_mat(self.ti, self.par['ti_fnam'])



            # write parameter file
            self.par_write()


            exe_file = self.method
            if self.iswin:
                exe_file = exe_file + '.exe'

            exe_path = self.which(exe_file)
            
            if exe_path is None:
                s = 'mpslib: The program {} does not exist or is not executable.'.format(exe_file)
                raise Exception(s)
            else:
                if not silent:
                    s = 'mpslib: Using the following executable to run the model: {}'.format(exe_path)
                    print(s)

            if not os.path.isfile(os.path.join(self.parameter_filename)):
                s = 'The the mpslib input file does not exists: {}'.format(self.parameter_filename)
                raise Exception(s)

            
            if (self.verbose_level > 0):
                print ("mpslib: trying to run  " + exe_path + " " + self.parameter_filename)
            
            if self.iswin: 
                CREATE_NO_WINDOW = 0x08000000        
                # stdout = subprocess.run([cmd,self.parameter_filename], stdout=subprocess.PIPE, creationflags=CREATE_NO_WINDOW)
                proc = subprocess.Popen([exe_path,self.parameter_filename], stdout=subprocess.PIPE, creationflags=CREATE_NO_WINDOW)
            else:
                proc = subprocess.Popen([exe_path,self.parameter_filename], stdout=subprocess.PIPE)


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


    # plot realizations (only in 2D so far)
    def plot_reals(self, nr=9):
        import matplotlib.pyplot as plt
        import numpy as np
        nsp=np.ceil(np.sqrt(nr))
        #plt.ion()
        fig1=plt.figure(1)
        fig1.clf()
        plt.set_cmap('hot')
        for i in range(0, np.min((self.par['n_real'],nr))):
            plt.subplot(nsp,nsp,i+1)
            plt.imshow(self.sim[i], interpolation='none')
            plt.title("Real %d" % (i+1))
        
        fig1.suptitle(self.method, fontsize=16)
        plt.show()

    # plot etypes (only in 2D so far)
    def plot_etype(self):
        
        import matplotlib.pyplot as plt
        import numpy as np
        from scipy import stats
        
        # read soft data ('check if it exist')
        d=eas.read(self.par['soft_data_filename'])
        #d=mps.eas.read(O1.par['soft_data_filename'])
        
        # compute Etype
        emean = np.mean(self.sim, axis=0)
        estd = np.std(self.sim, axis=0)
        emode = stats.mode(self.sim, axis=0)
        emode = emode[0][0]

        # plot the Etypes
        fig2=plt.figure(2)
        fig2.clf()                
        plt.subplot(1,3,1)
        plt.imshow(emean)
        plt.colorbar()
        plt.plot(d['D'][:,0], d['D'][:,1], 'k*',MarkerSize=32)
        plt.scatter(x=d['D'][:,0], y=d['D'][:,1], c=d['D'][:,4],s=15)
        plt.title('Etype Mean')

        plt.subplot(1,3,2)
        plt.imshow(estd)
        plt.colorbar()
        #plt.plot(d['D'][:,0], d['D'][:,1], 'k*',MarkerSize=32)
        plt.title('Etype Std')
        
        plt.subplot(1,3,3)
        plt.imshow(emode)
        plt.colorbar()
        plt.title('Etype Mode')
        
        #plt.savefig("soft_ti_example_%s_%s.png" % (O1.method,O1.par['ti_fnam']), dpi=600)
        plt.show()

       




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
