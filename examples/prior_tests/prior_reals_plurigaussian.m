% prior_reals_plurigaussian Sampling a PLURIGAUSSIAN type a priori model
clear all,close all;

%% Define a 2D plurigausiian a priori model
im=1;
prior{im}.type='plurigaussian';
dx=1;
prior{im}.x=[0:dx:100]; % X array
prior{im}.y=[0:dx:180]; % Y array

% define plurigaussian setup
prior{im}.pg_prior{1}.Cm='1 Gau(30,90,.6)';
prior{im}.pg_prior{2}.Cm='1 Gau(10,45,.3)';

% pg_map=[0 1];
pg_map=[0 1 2 1 2 1 ];
pg_map=[0 1 2 1 2 1 ; 1 1 1 2 2 2 ];
%pg_map=[0 0 0 1 1 1 2 2 2 1 1 1];
%pg_map=[0 0 0 1 1 1; 1 0 1 2 2 2; 2 1 0 1 1 1];
%pg_map=[0 0 0 ; 1 0 1; 2 1 0];
prior{im}.pg_map=pg_map;
cax=[min(pg_map(:)) max(pg_map(:))];;
prior{im}.cax=cax;

%%  Plot sample of the prior model
figure(10);clf
for i=1:5;
    [m,prior]=sippi_prior(prior);
    subplot(1,5,i);
    imagesc(prior{1}.x,prior{1}.y,m{1});
    axis image
end
colormap(sippi_colormap(1));
colorbar_shift;
%print_mul('prior_reals_plurigaussian')
%suptitle(sprintf('plurigaussian'))

%%
%return
%% Show movie of a random walk in the prior (using sequential Gibbs sampling)
save_movie=0;
randn('seed',1);
figure(11+j);clf
if save_movie==1;set(gcf,'units','normalized','outerposition',[0 0 1 1]);end
prior{1}.seq_gibbs.step=0.02;

if (save_movie==1)
    if isunix==1
        fname=[mfilename,'_target','.avi'];
        vidObj = VideoWriter(fname,'Uncompressed AVI');
    else
        fname=[mfilename,'_target','.mp4'];
        vidObj = VideoWriter(fname,'MPEG-4');
    end
end

if save_movie==1;open(vidObj);end
for i=1:200;
    [m,prior]=sippi_prior(prior,m);
    imagesc(prior{1}.x,prior{1}.y,m{1});
    colormap(sippi_colormap(1));
    caxis(cax)
    axis image
    axis tight
    set(gca,'nextplot','replacechildren');
    drawnow;
    if save_movie==1
        % Write each frame to the file.
        currFrame = getframe;
        writeVideo(vidObj,currFrame);
    end
end
if save_movie==1;close(vidObj);end
