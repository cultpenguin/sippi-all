%
close all

O=mps_snesim_read_par('snesim.txt');

[d_hard]=read_eas(O.hard_data_filename);

mg=O.n_multiple_grids;
%mg=1;

for img=0:1:mg;

    D1=read_eas_matrix(sprintf('%safter_relocation_before_simulation0_level_%d.gslib',O.ti_filename,mg));
    D2=read_eas_matrix(sprintf('%safter_simulation0_level_%d.gslib',O.ti_filename,img));
    D3=read_eas_matrix(sprintf('%safter_cleaning_relocation0_level_%d.gslib',O.ti_filename,img));
    
    figure(1);set_paper;
    subplot(mg+1,3,1+(img)*3);imagesc(O.x,O.y,D1);axis image;caxis([-1 1])
    ylabel(sprintf('NMG=%d',img))
    hold on;plot(d_hard(:,1),d_hard(:,2),'ko');hold off
    %hold on
    %scatter(d_hard(:,1),d_hard(:,2),18,d_hard(:,4),'o');
    %hold off
    title('Before simulation')
    subplot(mg+1,3,2+(img)*3);imagesc(O.x,O.y,D2);axis image;caxis([-1 1])
    hold on;plot(d_hard(:,1),d_hard(:,2),'ko');hold off
    title('After simulation')
    subplot(mg+1,3,3+(img)*3);imagesc(O.x,O.y,D3);axis image;caxis([-1 1])
    hold on;plot(d_hard(:,1),d_hard(:,2),'ko');hold off
    title('After relocations')
    %if img==0;
    %    subplot(1,3,1);title('Before simulation')
    %    subplot(1,3,2);title('After simulation')
    %    subplot(1,3,3);title('After removal of relocated data')
    %end
    
end

colormap(cmap_linear([1 1 1;1 0 0;0 0 0]))
print_mul('Relocation_investigation')
