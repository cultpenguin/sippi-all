%
% SOMETHING OIS VERY BAD SINCE THE LAST UPDATE
% ENESIM IS 20 TIMES SLOWER, (W/WO hard data) when MAXIT=1);
%
%

clear all;close all
TI=channels;TI=TI(2:2:end,2:2:end);
%TI=maze;TI=TI(10:1:110,10:1:110);           %  training image
%TI=TI(2:2:end,2:2:end);
SIM=zeros(50,80).*NaN; %  simulation grid
%[reals,O]=mps_cpp(TI,SIM,O);


% options for all
nhard=1;6;
nc=5; % TEMPLATE SIZE
Oorg.n_multiple_grids=1;; % --> !!
Oorg.shuffle_simulation_grid=1;

%nc=5;Oorg.n_multiple_grids=0;; % --> !!
%nc=2;Oorg.n_multiple_grids=3;; % --> !!

Oorg.n_real=100;             %  optional number of realization
Oorg.rseed=1;             %  optional number of realization

Oorg.template_size=[nc nc 1]; % SNESIM TYPE COND

Oorg.n_cond=nc^2; % ENESIM TYPE COND

%% different methods
io=0;

io=io+1;
O{io}=Oorg;
O{io}.method='mps_snesim_tree';
O{io}.n_cond=-1;

% io=io+1;
% O{io}=Oorg;
% O{io}.method='mps_snesim_list';
% 
% io=io+1;
% O{io}=Oorg;
% O{io}.method='mps_enesim_general';
% O{io}.n_max_cpdf_count=100000;
% O{io}.n_max_ite=1000;
% 
% io=io+1;
% O{io}=Oorg;
% O{io}.method='mps_enesim_general';
% O{io}.n_max_cpdf_count=10;
% O{io}.n_max_ite=1000;

% io=io+1;
% O{io}=Oorg;
% O{io}.method='mps_enesim_general';
% O{io}.n_max_cpdf_count=1;
% O{io}.n_max_ite=100000;

for io=1:length(O);
    O{io}.n_real=3;
    tic
    [reals{io},O{io}]=mps_cpp(TI,SIM,O{io});
    t1(io)=toc;
end


%% cond
rng(1);
clear d_c
d_x=ceil(rand(1,nhard).*(O{1}.simulation_grid_size(1)-1));
d_y=ceil(rand(1,nhard).*(O{1}.simulation_grid_size(2)-1));
d_z=d_y.*O{1}.origin(1);
for i=1:nhard
    d_c(i)=TI(d_y(i),d_x(i));
end
f_cond='f_cond.dat';
d_cond=[d_x(:) d_y(:) d_z(:) d_c(:)];
write_eas(f_cond,d_cond);

Oc=O;
for io=1:length(Oc);
    Oc{io}.d_cond=d_cond;
    Oc{io}.n_real=Oorg.n_real;
    Oc{io}.hard_data_filename=f_cond;
    Oc{io}.hard_data_search_radius=1;
    tic
    %[reals_cond{io},Oc{io}]=mps_cpp_thread(TI,SIM,Oc{io});
    [reals_cond{io},Oc{io}]=mps_cpp(TI,SIM,Oc{io});
    t2(io)=toc;
end

%%
x=[0:1:O{1}.simulation_grid_size(1)-1].*O{1}.grid_cell_size(1)+O{1}.origin(1);
y=[0:1:O{1}.simulation_grid_size(2)-1].*O{1}.grid_cell_size(2)+O{1}.origin(2);

try;close(1);end
figure(1);set_paper;
cmap_ref=colormap;
subplot(1,2,1);
imagesc(TI);axis image;
xlabel('X [pixel #]')
ylabel('Y [pixel #]')
title('a) Training image')
%print_mul('mps_softwareX_ti')
caxis([-1 1])
D=SIM.*0-1;
for i=1:nhard
    D(d_y(i),d_x(i))=d_c(i);;
end

subplot(1,2,2);
imagesc(D);axis image;
caxis([-1 1])
xlabel('X')
ylabel('Y')
hold on
scatter(d_cond(:,1),d_cond(:,2),40,d_cond(:,4),'filled')
hold off
title('b) Simulation grid with hard data')
colormap(cmap_linear([1 1 1; cmap_ref(1,:) ; cmap_ref(end,:)]))
print_mul('mps_softwareX_ti_sim')

%% plot uncond
figure(3);clf;set_paper;%('portrait');
nr=Oorg.n_real;
nr=3;
nr_use=5;
nO=length(reals);
for io=1:nO;
    j=(io-1)*nr_use;
    for ir=1:nr;
        subplot(nO,nr_use,j+ir);
        imagesc(x,y,reals{io}(:,:,ir));
        set(gca,'FontSize',8)
        axis image;
        if ir==1;
            txt=sprintf('%s) %s',char(96+io),O{io}.method);
            try
                txt=sprintf('%s) %s (Nmax=%d)',char(96+io),O{io}.method,O{io}.n_max_cpdf_count);
            end
            h_t=title(txt,'interp','none','FontSize',16);
            set(h_t,'HorizontalAlignment','Left')
            pos=get(h_t,'Position');
            pos(1)=-50;
            set(h_t,'Position',pos);
        end
    end
    [em,ev]=etype(reals{io});
    subplot(nO,nr_use,j+ir+1);
    imagesc(x,y,em);caxis([0 1]);axis image
    subplot(nO,nr_use,j+ir+2);
    imagesc(x,y,ev);caxis([0 .2]);axis image
  
    
end
print_mul('mps_softwareX_reals')


%% plot cond
figure(4);clf;set_paper;%('portrait');
nO=length(reals);
for io=1:nO;
    j=(io-1)*nr_use;
    for ir=1:nr;
        subplot(nO,nr_use,j+ir);
        imagesc(x,y,reals_cond{io}(:,:,ir));
        set(gca,'FontSize',8)
        axis image;
        if ir==1;
            txt=sprintf('%s) %s',char(96+io),O{io}.method);
            try
                txt=sprintf('%s) %s (Nmax=%d)',char(96+io),O{io}.method,O{io}.n_max_cpdf_count);
            end
            h_t=title(txt,'interp','none','FontSize',16);
            set(h_t,'HorizontalAlignment','Left')
            pos=get(h_t,'Position');
            pos(1)=-50;
            set(h_t,'Position',pos);
        end
    end
    [em,ev]=etype(reals_cond{io});
    d=reals_cond{1}(:);P0=(sum(d==0))/length(d);

    subplot(nO,nr_use,j+ir+1);
    imagesc(x,y,em);caxis([0 1]);axis image
    hold on
    plot(d_cond(:,1),d_cond(:,2),'w.','MarkerSize',30)
    scatter(d_cond(:,1),d_cond(:,2),30,d_cond(:,4),'filled')
    hold off

    subplot(nO,nr_use,j+ir+2);
    imagesc(x,y,ev);caxis([0 .2]);axis image
    
    title(sprintf('P_{1Dmarg}=[%3.2f %3.2f]',P0,1-P0))
    
end
fname=sprintf('mps_softwareX_NMG%d_NC%d_TS%d_SH%d_NH%d',Oc{1}.n_multiple_grids,Oc{1}.n_cond,Oc{1}.template_size(1),Oc{1}.shuffle_simulation_grid,nhard);
s=suptitle(fname);
set(s,'interpreter','none')
print_mul(fname)


