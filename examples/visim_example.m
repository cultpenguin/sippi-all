%% VISIM EXAMPLES

%% The simplest Examples
clear all;
V=visim_init;
visim;
V=visim(V);

%%
clear all
x=0:5:100;
y=0:5:110;
z=0;

m0=5;
sill=1;
range=30;
rotation=70;
anisotropy=0.15;
Va=sprintf('%g Sph(%g,%g,%g)',sill,range,rotation,anisotropy);

V=visim_init(x,y,z);

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


useHard=1;
if useHard==1
    V.fconddata.fname='d_obs.eas';
    d_obs=[20,20,0,1
        80,80,2,0];    
    %V.cols=[1:1:4]';
    write_eas(V.fconddata.fname,d_obs);
    V.cond_sim=2; % only point data
end

V.nsim=50;

V=visim_set_variogram(V,Va);
V.gmean=m0;

% run visim
V=visim(V);

figure(1);clf;
visim_plot_etype(V,1);

%figure(2);clf;
%visim_plot_sim(V);


%% VISIM through SIPPI - random walk
clear prior

ip=1;
prior{ip}.type='visim';
prior{ip}.x=1:1:80;
prior{ip}.y=1:1:80;
prior{ip}.Cm='1 Gau(20,30,.5)';

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
