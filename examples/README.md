## Examples
This folder contains a number of examples for runnin

	mps_genesim
	mps_snesim_tree
	mps_snesim_list


# uncondtional simualation
SNESIM type simulation:

	../mps_snesim_tree mps_snesim_unconditional.txt
	../mps_snesim_list mps_snesim_unconditional.txt

GENESIM in ENESIM mode, with full scan for coniditonal pdf:

	../mps_genesim mps_genesim_enesim_unconditional.txt

GENESIM in scanning for only up to 10 events to establish the conditional pdf:

	../mps_genesim mps_genesim_unconditional.txt

GENESIM in Direct Sampling mode, with a maximum of 1 event to establish the conditional pdf:

	../mps_genesim mps_genesim_dsam_unconditional.txt
