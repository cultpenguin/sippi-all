%% VISIM EXAMPLES

%% The simplest Examples
% clear all;
% V=visim_init;
% visim;
% V=visim(V);

%%
fclose all;
clear all
delete('*.eas')
delete('*.out')
delete('d_vol*eas');delete('visim.par');delete('randpath_visim.out');


x=0:5:100;
y=0:5:110;
z=0;

m0=5;
nugget=0.000001;
sill=1;
range=50;
rotation=70;
anisotropy=0.25;
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


useHardPoint=0;
if useHardPoint==1
    V.fconddata.fname='d_obs.eas';
    d_obs=[20,20,0,1
        80,80,0,2];    
    %V.cols=[1:1:4]';
    write_eas(V.fconddata.fname,d_obs);
    V.cond_sim=2; % only point data

end


useSoftPoint=1;
if useSoftPoint==1
    
    d_obs=[x(1),y(1),0,1,0.001
        x(2),y(1),0,1,0.01    
        x(end),y(end),0,2,0.02];    
   
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


V.read_randpath=0;
V.nsim=50;

V=visim_set_variogram(V,Va);
V.Va.nugget=nugget;
V.gmean=m0;

V.debuglevel=-1;
V.densitypr=0;
V.volnh.method=0;


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

% run visim
tic
V=visim(V);
toc

figure(1);clf;
visim_plot_etype(V,1);

figure(2);
r=f77strip('randpath_visim.out');
subplot(1,2,1);
plot(std(r));
subplot(1,2,2);
plot(r(:,1:20),'.')
%axis image
%figure(2);clf;
%visim_plot_sim(V);

return
%% VISIM through SIPPI - random walk
clear prior

ip=1;
prior{ip}.type='visim';
prior{ip}.x=1:1:80;
prior{ip}.y=1:1:80;
prior{ip}.Cm='1 Gau(50,30,.5)';

% SGSIM
prior{ip}.method='sgsim';

% DSSIM: use target historgram
prior{ip}.method='dssim';
d_target=[-2 -1.5 0 1.5 1.5];
prior{ip}.d_target=d_target;

m=sippi_prior(prior);
sippi_plot_prior(prior,m)

prior{ip}.seq_gibbs.type=2;%
prior{ip}.seq_gibbs.step=.95; % Resim 95% of data
[m,prior]=sippi_prior(prior);
for i=1:100;
    [m,prior]=sippi_prior(prior,m);
    sippi_plot_prior(prior,m);caxis([-2 2])
    drawnow;
end
