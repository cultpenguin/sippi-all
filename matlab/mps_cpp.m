% mps_cpp: run MPS algorirthms from Matlab
%
% Call:
%   [reals,O]=mps_cpp(TI,SIM,O);
%
%
% example:
%   TI=channels;           %  training image
%   SIM=zeros(80,60).*NaN; %  simulation grid
%   O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree') 
%   %O.method='mps_snesim_list'; % 
%   %O.method='mps_genesim'; % 
%   O.n_real=1;             %  optional number of realization
%   [reals,O]=mps_cpp(TI,SIM,O);
%
% See also: mps_snesim_read_par, mps_snesim_write_par, mps_enesim_read_par,
% mps_enesim_write_par
%

function [reals,O]=mps_cpp(TI,SIM,O,verbose);

if nargin==0;
   TI=channels;           %  training image
   SIM=zeros(80,60).*NaN; %  simulation grid
   O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree') 
  %O.method='mps_snesim_list'; % MPS algorithm to run (def='mps_snesim_tree') 
   %O.method='mps_enesim_general'; % MPS algorithm to run (def='mps_snesim_tree') 
   O.n_real=1;             %  optional number of realization
   [reals,O]=mps_cpp(TI,SIM,O);
   imagesc(reals);drawnow;
end

D=[];
reals=[];

if nargin<1; TI=channels;end
if nargin<2; SIM=zeros(100,100);end
if nargin<3, O.null='';end
if nargin<4, 
    if isfield(O,'verbose')
        verbose=O.verbose;
    else
        verbose=0;
    end
end
O.verbose=verbose;
    
%% write ti
doWriteTI=1;
if doWriteTI==1;
  O.ti_filename='ti.dat';
  %[ny,nx,nz]=size(TI);
  [nx,ny,nz]=size(TI);
  t_title=sprintf('%d %d %d',nx,ny,nz);
  t_row='rowHeader';
  write_eas(O.ti_filename,TI(:),t_row,t_title);
  % tTI=TI';write_eas(O.ti_filename,tTI(:),t_row,t_title);
end

%% write simulation grid
doSetSIM=1;
if doSetSIM==1;
  if ~isfield(O,'simulation_grid_size');
    [ny,nx,nz]=size(SIM);
    O.simulation_grid_size=[nx ny nz];
  end
  if ~isfield(O,'origin');
    O.origin=[0 0 0];
  end
  if ~isfield(O,'grid_cell_size');
    O.grid_cell_size=[1 1 1];
  end
end

%% 
% WRITE HARD DATA IF NECESSARY
if isfield(O,'d_cond');
    if ~isfield(O,'d_cond');
        O.hard_data_filename='d_cond.eas';
    end    
    write_eas(O.hard_data_filename,O.d_cond);
end


% WRITE SOFT DATA IF NECESSARY

%% 
if ~isfield(O,'method');
  O.method='mps_snesim_tree';
  %O.method='mps_snesim_list';
  %O.method='mps_genesim';
end

%% WRITE PARAMETER FILE
if strfind(O.method,'snesim');
    if strcmp(O.method,'mps_snesim');
        O.method='mps_snesim_tree';
    end
    if ~isfield(O,'parameter_filename');O.parameter_filename='snesim.txt';end
    O=mps_snesim_write_par(O);
elseif (strcmp(O.method,'mps_genesim'))||(strcmp(O.method,'mps_enesim_general'))
    O.method='mps_genesim';
    if ~isfield(O,'parameter_filename');O.parameter_filename='genesim.txt';end
    O=mps_enesim_write_par(O);
elseif strcmp(O.method,'mps_dsam');
    O.method='mps_genesim';
    O.n_max_cpdf_count=1;
    if ~isfield(O,'parameter_filename');O.parameter_filename='dsam.txt';end
    O=mps_enesim_write_par(O);
elseif strcmp(O.method,'mps_enesim');
    O.method='mps_genesim';
    O.n_max_cpdf_count=1e+20;
    if ~isfield(O,'parameter_filename');O.parameter_filename='enesim.txt';end
    O=mps_enesim_write_par(O);
else
    disp(sprintf('%s: no method for ''%s''',mfilename,O.method));
    return
end

%% RUN MPS CODE
% make output folder if it does not exist
if ~exist(O.output_folder,'dir')
    mkdir(O.output_folder);
end

%w_root='C:\Users\tmeha\RESEARCH\PROGRAMMING\MPS';

w_root=[fileparts(which('mps_cpp')),filesep,'..'];
O.exe_filename=[w_root,filesep,O.method];
if ~isunix
  O.exe_filename=[O.exe_filename,'.exe'];
end

% check that file exist
if ~exist(O.exe_filename,'file')
    disp(sprintf('%s: %s does not exist',mfilename,O.exe_filename));
    return;
end

cmd=sprintf('%s %s',O.exe_filename,O.parameter_filename);
if O.verbose>0
    disp(cmd)
end
% run the algorithm
tic;
[status,cmdout]=system(cmd);
if O.verbose>0
    disp(status);
    disp(cmdout);
end
O.time=toc;
%%

%% READ DATA
[p,f,e]=fileparts(O.ti_filename);
clear D;
for i=1:O.n_real
  fname=sprintf('%s%s%s%s_sg_%d.gslib',O.output_folder,filesep,f,e,i-1);
  try
      D=read_eas(fname);
  catch
      disp(sprintf('%s: problems reading output file (%s) - waiting a bit an retrying',mfilename,fname));
      pause(5);
      D=read_eas(fname);
  end
  
  if (O.simulation_grid_size(2)==1)&(O.simulation_grid_size(3)==1)
    % 1D
    reals=D;
  elseif (O.simulation_grid_size(3)==1)
    % 2D
    reals(:,:,i)=reshape(D,O.simulation_grid_size(1),O.simulation_grid_size(2))';
  else
    % 3D
  end
end

%% READ TEMPORARY GRID VALUES
if (O.debug>1)
    for i=1:O.n_real
        fname_tg1=sprintf('%s%s%s%s_temp1_%d.gslib',O.output_folder,filesep,f,e,i-1);
        if exist(fname_tg1,'file')
            O.TG1=read_eas_matrix(fname_tg1);
        end
        fname_tg2=sprintf('%s%s%s%s_temp2_%d.gslib',O.output_folder,filesep,f,e,i-1);
        if exist(fname_tg2,'file')
            O.TG2=read_eas_matrix(fname_tg2);
        end
        
    end
    
end

%%
if (O.debug>1)
for i=1:O.n_real
    fname_path=sprintf('%s%s%s%s_path_%d.gslib',O.output_folder,filesep,f,e,i-1);

    

    try
        PP=read_eas(fname_path);
    catch
        disp(sprintf('%s: problems reading output file (%s) - waiting a bit an retrying',mfilename,fname));
        pause(5);
        PP=read_eas(fname_path);
    end
    O.path(:,i)=PP(:);
 
    if (O.simulation_grid_size(2)==1)&(O.simulation_grid_size(3)==1)
        % 1D
    elseif (O.simulation_grid_size(3)==1)
        % 2D
        try
            O.P=read_eas_matrix(fname_path);
        end
        [ix,iy]=ind2sub([O.simulation_grid_size(2),O.simulation_grid_size(1)],PP(:)+1);
        for j=1:length(ix);
            O.path_index(iy(j),ix(j),i)=j;
        end
    else
        % 3D
    end
    
    
end
end


O.x=[0:1:(O.simulation_grid_size(1)-1)].*O.grid_cell_size(1)+O.origin(1);
O.y=[0:1:(O.simulation_grid_size(2)-1)].*O.grid_cell_size(2)+O.origin(2);
O.z=[0:1:(O.simulation_grid_size(3)-1)].*O.grid_cell_size(3)+O.origin(3);

%%
mps_cpp_clean(O);




