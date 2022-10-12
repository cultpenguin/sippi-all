%% VISIM EXAMPLES

% UYSING ONLY SOFT DATA, MATLAB CANNOT READ IN FROM VISIM.OUT

%% The simplest Examples
%%clear all;
%V=visim_init;
%visim;
%V=visim(V);

%%
delete('d_vol*eas');delete('visim.par');delete('randpath_visim.out');
fclose all;
clear all
close all;drawnow;
delete('*.eas')
delete('*.out')
x=0:1:5;
y=0:1:8;
z=0;

m0=0;
nugget=0.000001;
sill=1;
range=6;
rotation=70;
anisotropy=0.25;

sill=1;
range=.1;
rotation=70;
anisotropy=1;

Va=sprintf('%g Sph(%g,%g,%g)',sill,range,rotation,anisotropy);

V=visim_init(x,y,z);

useTarget=0;
if useTarget==1;
    d_target=[randn(200,1)*1.5+2;randn(200,1)*.5+6];
    sill=var(d_target);
    m0=mean(d_target);
    
    V.refhist.fname='d_target.eas';
    write_eas(V.refhist.fname,d_target(:));
    V.tail.zmin=min(d_target);
    V.tail.zmax=max(d_target);
    V.ccdf=1; % use target dist !
    %V.refhist.do_discrete=0; % continous values in target hist
    V.refhist.do_discrete=1; % discrete values in target hist
end

V.cond_sim=0; % only point data

useHardPoint=1;
if useHardPoint==1
    V.fconddata.fname='d_obs.eas';
    d_obs=[0,0,0,.50
        1,0,0,.51];    
    %V.cols=[1:1:4]';
    write_eas(V.fconddata.fname,d_obs);
    V.cond_sim=2; % only point data

end


useSoftPoint=1;
if useSoftPoint==1
    
    d_obs=[0,1,0,1,0.1
        0,2,0,1,0.0001    
        0,3,0,1,0.0002];    
   
    V.fvolgeom.fname='d_volgeom.eas';
    V.fvolsum.fname='d_volsum.eas';
    
    clear d_volgeom d_volsum
    for i=1:size(d_obs,1)
        d_volgeom(i,:)=[d_obs(i,1:3) i 1];
        d_volsum(i,:)= [i 1 d_obs(i,4:5)];
    end
    write_eas(V.fvolgeom.fname,d_volgeom);
    write_eas(V.fvolsum.fname,d_volsum);
    if useHardPoint==1
        V.cond_sim=1; % hard and soft data
    else
        V.cond_sim=3; % soft data
    end
end

%
V.rseed=1;
V.nsim=82;
V=visim_set_variogram(V,Va);
V.Va.nugget=nugget;
V.gmean=m0;

V.debuglevel=-1;
V.densitypr=0;
V.volnh.method=2;



% run visim
V.volnh.method=0; 
V.densitypr=0;
V.debuglevel=2;
V.nsim=22;

V.read_randpath=0;
V.read_volnh=-1;
V.read_covtable=-1;
tic
V=visim(V);
toc

figure(1);clf;
visim_plot_etype(V,1);
subplot(1,2,1);caxis([-1.5 1.5])
subplot(1,2,2);caxis([0 2])
drawnow;

figure(2);clf;
visim_plot_sim(V);
drawnow;
%%
figure(3);
r=f77strip('randpath_visim.out');
subplot(1,2,1);
plot(std(r));
subplot(1,2,2);
plot(r(:,1:8),'.')
%axis image
%figure(2);clf;
%visim_plot_sim(V);

