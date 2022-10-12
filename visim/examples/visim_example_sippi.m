%% VISIM EXAMPLES FROM SIPPI

%%
delete('d_vol*eas');delete('visim.par');delete('randpath_visim.out');
fclose all;
clear all
%close all;drawnow;
delete('*.eas')
delete('*.out')
x=0:1:50;
y=0:1:80;
z=0;

%% prior variograḿ
m0=0;
nugget=0.0001;
sill=1;
range=35;
rotation=70;
anisotropy=0.3;

Va=sprintf('%g Gau(%g,%g,%g)',sill,range,rotation,anisotropy);

%% conditional data, d_obs
d_obs_soft=[20,21,0,1+m0,0.1
    20,52,0,-1+m0,0.6
    50,53,0,1+m0,.01];    

d_obs_hard=[0,0,0,.50+m0,0
    20,10,0,.51+m0,0];    




%% setup visim in sippi
ip=1;
prior{ip}.type='visim';
prior{ip}.x=x;
prior{ip}.y=y;
prior{ip}.Cm=Va;
prior{ip}.d_obs=[d_obs_hard;d_obs_soft];
%prior{ip}.d_obs=[d_obs_soft];
%prior{ip}.d_obs=[d_obs_hard];

[m,prior]=sippi_prior(prior);
sippi_plot_prior(prior,m)

 
%% simulation visim style
V=prior{1}.V;
V.nsim=100;
V=visim(V);
figure(2);clf
visim_plot_etype(V,1);

%% estimation visim style
Vest=prior{1}.V;
Vest.nsim=0;
Vest=visim(Vest);
figure(3);clf;
visim_plot_etype(Vest,1);


return
%%

V=visim_init(x,y,z);

%%
useTarget=1;
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

useHardPoint=0;
if useHardPoint==1
    V.fconddata.fname='d_obs.eas';
    d_obs=[0,0,0,.50+m0
        20,10,0,.51+m0];    
    %V.cols=[1:1:4]';
    write_eas(V.fconddata.fname,d_obs);
    V.cond_sim=2; % only point data

end


useSoftPoint=1;
if useSoftPoint==1
    
    d_obs=[20,21,0,1+m0,0.1
        20,22,0,1+m0,0.0001    
        50,53,0,1+m0,.2
        ];    
   
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
V=visim_set_variogram(V,Va);
V.Va.nugget=nugget;
V.gmean=m0;

%V.debuglevel=-1;
%V.densitypr=0;
%V.volnh.method=2;

% run visim
V.volnh.method=0; 
V.densitypr=0;
V.debuglevel=-1;
V.nsim=30;

%V.read_randpath=0;
%V.read_volnh=-1;
%V.read_covtable=-1;
tic
V=visim(V);
toc

figure(1);clf;
visim_plot_etype(V,1);
subplot(1,2,1);
subplot(1,2,2);caxis([0 2])
drawnow;

return
%%
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

