% mps_cpp_demo_examples_unconditional: Examples of runinng MPS from Matlab
%
% See also: mps_cpp_demo
%
clear all;close all


%% ENESIM
clear all;
O.method='mps_enesim_general'; 

par_1='n_cond';    arr_1=[2,3,4,5].^2; 
par_2='n_max_ite'; arr_2=[10, 100, 1000, 10000];

figure(1);
mps_cpp_demo;

%% SNESIM, Multiple Grids and size of conditional template
clear all;
O.method='mps_snesim_tree'; 
    
par_1='template_size';    arr_1=[2,3,4,5,6,7].^2;
par_2='n_multiple_grids'; arr_2=[0,1,2,3];

figure(2);
mps_cpp_demo;

%% SNESIM TREE N_COND
clear all
O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree')
O.template_size=[7 7 1];

par_1='n_cond';arr_1=[0:1:25,48,49,-1];

figure(3);
mps_cpp_demo;


%% SNESIM LIST N_COND
clear all
O.method='mps_snesim_list'; % MPS algorithm to run (def='mps_snesim_tree')
O.template_size=[7 7 1];

par_1='n_cond';arr_1=[0:1:25,48,49,-1];

figure(4);
mps_cpp_demo;



