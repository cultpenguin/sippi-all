clear all;close all;
O=mps_snesim_read_par('snesim_soft_as_hard.par');
SIM=ones(O.simulation_grid_size(2),O.simulation_grid_size(1)).*NaN;
TI=read_eas_matrix('ti.dat');%
cmap=cmap_linear([1 1 1; 1 0 0; 0 0 0]);

O.hard_data_filename='dummy';
O.soft_data_filename='dummy';

O.hard_data_filename='hard_as_hard.dat';
%O.soft_data_filename='soft_as_hard.dat';



%%
O.filename_parameter='mps_snesim.txt';
nmg=3;
O.n_multiple_grids=nmg;
O.debug=3;
O.n_real=1;
O.template_size=[10 10 1];
O.template_size=[7 7 1];
O.clean = 1;
O.exe_root = 'E:\Users\tmh\RESEARCH\PROGRAMMING\GITHUB\MPSLIB\msvc2017\x64\Release';

O.shuffle_simulation_grid=2;

[r,Oo]=mps_cpp(TI,SIM,O);

mps_cpp_plot(r,Oo,1);





return
%%
O.filename_parameter='mps_snesim_mul.txt';
O.debug=0;
O.n_real=50;
O.n_multiple_grids=3;
O.template_size=[10 10 1];
[r,Oo]=mps_cpp(TI,SIM,O);
mps_cpp_plot(r,Oo);
