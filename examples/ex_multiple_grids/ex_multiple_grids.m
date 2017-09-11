% mpslib_multiple_grid
clear all;close all
x=1:1:90;nx=length(x);
y=1:1:95;ny=length(y);


f_ti{1}='ti_cb_6x6_120_120_1.dat';
f_ti{2}='ti_strebelle_125_125_1.dat';
f_ti{3}='ti_lines_arrows_100_100_1.dat';
i_ti=3;


% load TIaxis
TI=read_eas_matrix(['..',filesep,'..',filesep,'ti',filesep,f_ti{i_ti}]);

% setup simulation grid
SIM=zeros(ny,nx).*NaN;;

O.debug=1;
O.n_cond=36;
O.template_size=[19 19 1];
%% UNCONDITIONAL
% MPS_SNESIM_TREE
O.parameter_filename='mps_snesim.txt';
O.method='mps_snesim_tree'; 
O.n_real=1;
O.rseed=1;
j=0;


O.shuffle_simulation_grid=0; % sequential/unilateral
O.shuffle_simulation_grid=1; % random

nmg=[0,1,2,3];ni=length(nmg);
tsize=[3 6 9 15 25];;nj=length(tsize);

for i=1:length(nmg);
for j=1:length(tsize);
    O.template_size=[tsize(j) tsize(j) 1];
    O.n_multiple_grids=nmg(i);
   
    [reals,O]=mps_cpp(TI',SIM,O);
    
    subplot(ni,nj,j+(i-1)*nj);
    imagesc(x,y,reals(:,:,1)');
    set(gca,'FontSize',5)
    axis image
    title(sprintf('NMG=%d, TS=%d, t=%3.1fs',O.n_multiple_grids,O.template_size(1),O.time),'FontSize',6)
    drawnow;
end
end
print('-dpng',sprintf('ex_multiple_grids_shuffle%d',O.shuffle_simulation_grid))