## 2D Examples
This folder contains a number of 2D examples for running

	mps_genesim
	mps_snesim_tree
	mps_snesim_list

ALl parameter files have been generated using the Matlab interface to MPSlib 
(thorugh mps_examples.m), but this is not needed to run the examples. 

For all example the simulation grid is 80x40 pixels.
4 realizations are genearted from each run. 


# uncondtional simulation
SNESIM type simulation:

	../mps_snesim_tree mps_snesim_unconditional.txt
	../mps_snesim_list mps_snesim_unconditional.txt

GENESIM in ENESIM mode, with full scan for coniditonal pdf:

	../mps_genesim mps_genesim_enesim_unconditional.txt

GENESIM in scanning for only up to 10 events to establish the conditional pdf:

	../mps_genesim mps_genesim_unconditional.txt

GENESIM in Direct Sampling mode, with a maximum of 1 event to establish the conditional pdf:

	../mps_genesim mps_genesim_dsam_unconditional.txt


# Condtional simulation - Hard data
Reference 'image' from which hard and soft data is created
![Reference image](https://raw.githubusercontent.com/ergosimulation/mpslib/master/examples/2D/mps_2d_examples_reference_ti01.png)

Hard data:
![Hard data](https://raw.githubusercontent.com/ergosimulation/mpslib/master/examples/2D/mps_2d_examples_hard_ti01.png)

Examples of runnnig simulation conditional to the hard data

	../mps_snesim_tree mps_snesim_hard.txt
	../mps_snesim_list mps_snesim_hard.txt
	../mps_genesim mps_genesim_enesim_hard.txt
	../mps_genesim mps_genesim_hard.txt
	../mps_genesim mps_genesim_dsam_hard.txt


# Condtional simulation - Soft data
Soft data:
![Soft data](https://raw.githubusercontent.com/ergosimulation/mpslib/master/examples/2D/mps_2d_examples_hard_soft_ti01.png)

Examples of running simulation conditional to the hard data

	../mps_snesim_tree mps_snesim_hard_soft.txt
	../mps_snesim_list mps_snesim_hard_soft.txt
	../mps_genesim mps_genesim_enesim_hard_soft.txt
	../mps_genesim mps_genesim_hard_soft.txt



