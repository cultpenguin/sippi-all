% mps_example_softwarex
%
% Figure for SoftwareX manuscript 
clear all;close all

n_real=4;

x=1:1:80;nx=length(x);
y=1:1:40;ny=length(y);


f_ti{1}='ti_cb_6x6_40_40_1.dat';
f_ti{2}='ti_strebelle_125_125_1.dat';
f_ti{3}='ti_cb_101_101_4cat.dat';
f_ti{4}='ti_cb_6x6_102_102_1.dat';
i_ti=4;


% load TI
TI=read_eas_matrix(['..',filesep,'TI',filesep,f_ti{i_ti}]);

% setup simulation grid
SIM=zeros(ny,nx);

O.n_real=n_real;            
O.debug=1;
O.n_cond=25;

%% UNCONDITIONAL
% MPS_SNESIM_TREE
O.parameter_filename='mps_snesim_unconditional.txt';
O.method='mps_snesim_tree'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

% MPS_GENESIM_TREE / ENESIM1e+9 / DSIM
O.n_max_ite=1e+9;
O.n_max_cpdf_count=1e+9;
O.parameter_filename='mps_genesim_enesim_unconditional.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

O.n_max_ite=1e+9;
O.n_max_cpdf_count=10;
O.parameter_filename='mps_genesim_unconditional.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

O.n_max_ite=1e+9;
O.n_max_cpdf_count=1;
O.parameter_filename='mps_genesim_dsam_unconditional.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);


%% MAKE HARD DATA
m_ref=[reals(:,:,1)];
m_ref=TI(1:ny,1:nx);
[xx,yy]=meshgrid(O.x,O.y);
nxy=prod(size(xx));
n_hard=12;
i_hard=randomsample(nxy,12);
pos_hard(:,1)=xx(i_hard);
pos_hard(:,2)=yy(i_hard);
pos_hard(:,3)=O.z(1);
val_hard(:,1)=m_ref(i_hard);
write_eas('mps_hard_data.dat',[pos_hard val_hard]);

% soft data
ind=unique(m_ref(:));
clear p_soft;
[h,hx]=hist(m_ref(:),ind)
marg_1d=h./sum(h);
for i=1:length(ind);
    p_soft(:,i)=zeros(nxy,1);
    ii=find(m_ref==ind(i));
    p_soft(ii,i)=1;    
end
p_soft=p_soft+.7;
sum_p_soft=sum(p_soft')';
NM=repmat(sum_p_soft,[1,length(ind)]);
p_soft=p_soft./NM;

write_eas('mps_soft_data.dat',[xx(:) yy(:) yy(:).*0+O.z(1) p_soft]);


%%
% figures
figure(1);
imagesc(O.x,O.y,m_ref);
xlabel('X')
ylabel('Y')
title('Reference model')
axis image;
ax=axis;
colorbar
set(gca,'ydir','reverse');
print -dpng mps_examples_reference.png

figure(2);
scatter(pos_hard(:,1),pos_hard(:,2),40,val_hard,'filled');
xlabel('X')
ylabel('Y')
title('Hard data')
axis image
axis(ax);
box on
colorbar;
set(gca,'ydir','reverse');
print -dpng mps_examples_hard_data.png

figure(3);
for i=1:length(ind);
    subplot(2,2,i);
    scatter(xx(:),yy(:),40,p_soft(:,i),'filled');
    caxis([0 1])
    xlabel('X')
    ylabel('Y')
    title(['soft data, ind=',num2str(ind(i))])
    axis image
    axis(ax);
    box on
    colorbar;
    set(gca,'ydir','reverse');
end
print -dpng mps_examples_soft_data.png






%% CONDITIONAL
O.hard_data_filename = 'mps_hard_data.dat';

% MPS_SNESIM_TREE
O.parameter_filename='mps_snesim_hard.txt';
O.method='mps_snesim_tree'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

% MPS_GENESIM_TREE / ENESIM1e+9 / DSIM
O.n_max_ite=1e+9;
O.n_max_cpdf_count=1e+9;
O.parameter_filename='mps_genesim_enesim_hard.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

O.n_max_ite=1e+9;
O.n_max_cpdf_count=10;
O.parameter_filename='mps_genesim_hard.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

O.n_max_ite=1e+9;
O.n_max_cpdf_count=1;
O.parameter_filename='mps_genesim_dsam_hard.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);



%% CONDIIONAL SOFT DATA
O.hard_data_filename = 'mps_hard_data.dat';
O.soft_data_filename = 'mps_soft_data.dat';
O.soft_data_categories='0;1;2;3';

% MPS_SNESIM_TREE
O.parameter_filename='mps_snesim_hard_soft.txt';
O.method='mps_snesim_tree'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);


% MPS_GENESIM_TREE / ENESIM1e+9 / DSIM
O.n_max_ite=1e+9;
O.n_max_cpdf_count=1e+9;
O.parameter_filename='mps_genesim_enesim_hard_soft.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);

O.n_max_ite=1e+9;
O.n_max_cpdf_count=10;
O.parameter_filename='mps_genesim_hard_soft.txt';
O.method='mps_genesim'; 
O.n_real=n_real;            
[reals,O]=mps_cpp(TI,SIM,O);


%%
%% CONDIIONAL SOFT DATA
O.hard_data_filename = 'mps_hard_data.dat';
O.soft_data_filename = 'mps_soft_data.dat';
O.soft_data_categories='0;1;2;3';

% MPS_SNESIM_TREE
O.parameter_filename='mps_snesim_hard_soft_200.txt';
O.method='mps_snesim_tree'; 
O.n_real=200;            
[reals,O]=mps_cpp(TI,SIM,O);
[Emean,Estd,Emode]=etype(reals);
figure(4);
imagesc(O.x,O.y,Emode);


