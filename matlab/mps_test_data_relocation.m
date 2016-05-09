%
close all

O=mps_snesim_read_par('snesim.txt');

[d_hard]=read_eas(O.hard_data_filename);

mg=O.n_multiple_grids;
%mg=1;

for img=0:1:mg;

    
    D1=read_eas_matrix(sprintf('%safter_relocation_before_simulation0_level_%d.gslib',O.ti_filename,img));
    D2=read_eas_matrix(sprintf('%safter_simulation0_level_%d.gslib',O.ti_filename,img));
    D3=read_eas_matrix(sprintf('%safter_cleaning_relocation0_level_%d.gslib',O.ti_filename,img));
    
    figure(1);set_paper;
    subplot(mg+1,3,1+(img)*3);
    imagesc(O.x,O.y,D1);axis image;caxis([-1 1])
    ylabel(sprintf('NMG=%d',img))
    %hold on;plot(d_hard(:,1),d_hard(:,2),'ko');hold off
    hold on
    scatter(d_hard(:,1),d_hard(:,2),48,d_hard(:,4),'o');
    hold off
    %plot coarse grid
    dx=2^(img);
    hold on
    for ix=O.x(1):dx:max(O.x);
    for iy=O.y(1):dx:max(O.y);
        plot(ix,iy,'.','color',[1 1 1].*.5,'MarkerSize',8);
    end 
    end
    hold off
    
    caxis([-1 1])
    title('Before simulation')
    
    subplot(mg+1,3,2+(img)*3);imagesc(O.x,O.y,D2);axis image;caxis([-1 1])
    %hold on;plot(d_hard(:,1),d_hard(:,2),'ko');hold off
    hold on
    scatter(d_hard(:,1),d_hard(:,2),48,d_hard(:,4),'o');
    hold off
    caxis([-1 1])
    title('After simulation')
    ax=axis;   
    
    subplot(mg+1,3,3+(img)*3);
    imagesc(O.x,O.y,D3);axis image;caxis([-1 1])
    %hold on;plot(d_hard(:,1),d_hard(:,2),'o','color',[1 1 1].*.6);hold off
    hold on;scatter(d_hard(:,1),d_hard(:,2),48,d_hard(:,4),'o');hold off  
    %hold on;plot(d_hard(:,1),d_hard(:,2),'bo');hold off
    hold on
    for ix=O.x(1):dx:max(O.x);
    for iy=O.y(1):dx:max(O.y);
        plot(ix,iy,'.','color',[1 1 1].*.5,'MarkerSize',8);
    end 
    end
    hold off
    axis image;axis(ax);set(gca,'ydir','reverse');box on
    caxis([-1 1])    
    title('After relocations')
    %if img==0;
    %    subplot(1,3,1);title('Before simulation')
    %    subplot(1,3,2);title('After simulation')
    %    subplot(1,3,3);title('After removal of relocated data')
    %end
    
end

try 
    colormap(cmap_linear([1 1 1;1 0 0;0 0 0]))
    print_mul('Relocation_investigation')
end

