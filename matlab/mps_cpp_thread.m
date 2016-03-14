% mps_cpp_threads: run MPS algorirthms using multiple threads
%
% Call:
%   [reals,O]=mps_cpp_threads(TI,SIM,O);
%
% example:
%   TI=channels;           %  training image
%   SIM=zeros(80,60).*NaN; %  simulation grid
%   O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree') 
%   %O.method='mps_snesim_list'; % MPS algorithm to run (def='mps_snesim_tree') 
%   %O.method='mps_enesim_general'; % MPS algorithm to run (def='mps_snesim_tree') 
%   O.nreal=8;             %  optional number of realization
%   [reals,O]=mps_cpp_thread(TI,SIM,O);
%
% See also: mps_cpp, mps_snesim_read_par, mps_snesim_write_par, 
%           mps_enesim_read_par, mps_enesim_write_par
%
%
function [reals,Othread]=mps_cpp_thread(TI,SIM,O,verbose);
reals=[];

if ~isfield(O,'nreal');
    O.nreal=1;
end

%% try to hety 
try
    poolobj = gcp('nocreate');
    n_threads=poolobj.NumWorkers;
catch
    disp(sprintf('%s: No parallel toolbox - using 1 thread/worker',mfilename))
    n_threads=1;
end

n_reals_per_thread=ceil(O.nreal/n_threads);
actual_threads=ceil(O.nreal/n_reals_per_thread);


if actual_threads==1;
    disp(sprintf('%s: No parallelization, using 1 thread/worker',mfilename))
    [reals,Othread]=mps_cpp(TI,SIM,O);
    return    
else
    disp(sprintf('%s: Using %d threads/workers',mfilename,actual_threads))
end


%%
for i=1:actual_threads;
    outdir{i}=sprintf('mps_%02d',i);   
    if ~exist(outdir{i},'dir')
        mkdir(outdir{i});
    end
    Othread{i}=O;
    %Othread{i}.output_folder=[outdir{i},filesep,'.'];
    if i==actual_threads
        reals_left=O.nreal-(i-1)*n_reals_per_thread;
        Othread{i}.nreal=reals_left;
    else
        Othread{i}.nreal=n_reals_per_thread;
    end
    Othread{i}.rseed=O.rseed+i;
end

%%

cwd=pwd;
parfor i=1:actual_threads;
    cd(cwd);
    cd(outdir{i});
    disp(pwd);
    [r{i}]=mps_cpp(TI,SIM,Othread{i});
end
%%
cd(cwd);

if (Othread{1}.simulation_grid_size(3)==1)&&(Othread{1}.simulation_grid_size(2)==1)
    ndim=1;
elseif (Othread{1}.simulation_grid_size(3)==1)
    ndim=2;
else
    ndim=3;
end

for i=1:actual_threads;
    if i==1;
        reals=r{i};        
    else
        reals=cat(ndim+1,reals,r{i});
    end
end







