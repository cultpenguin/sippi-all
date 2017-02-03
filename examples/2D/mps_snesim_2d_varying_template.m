% mps_snesim_2d_varying_template
%
% needs mGstat/SIPPI
%
O=mps_snesim_read_par('mps_snesim_2d_unconditional.txt');
O.parameter_filename='mps_snesim_2d_varying_template.txt';


TI=read_eas_matrix('ti.dat');
TI=channels;
O.simulation_grid_size=[80 100 1];

SIM=zeros(100,80).*NaN; %  simulation grid
O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree')

maxt=9;
k=0;
nr=3;
nt=5;
for j=1:nt
    for i=1:nr;
        k=k+1;
        O.template_size=[maxt maxt-(j-1) ; maxt maxt-(j-1) ; 1 1 ];
        O.rseed=i;
        O.n_real=1;             %  optional number of realization
        [reals,O]=mps_cpp(TI,SIM,O);
        
        subplot(nt,nr,k);
        imagesc(reals);axis image;
        title(sprintf('t=%3.2fs T(%d,%d-%d,%d)',O.time,O.template_size(1,1),O.template_size(2,1),O.template_size(1,2),O.template_size(2,2)),'FontSize',8)
        axis off
        
        drawnow;
    end
end
