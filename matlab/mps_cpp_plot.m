% mps_cpp_plot: plot result of MPS simulation
%
% Call:
%   [reals,O]=mps_cpp_plot(reals,O);
%
% example:
%   TI=channels;           %  training image
%   SIM=zeros(80,60).*NaN; %  simulation grid
%   O.method='mps_snesim_tree'; % MPS algorithm to run (def='mps_snesim_tree')
%   %O.method='mps_snesim_list'; %
%   %O.method='mps_genesim'; %
%   O.n_real=1;             %  optional number of realization
%   [reals,O]=mps_cpp(TI,SIM,O);
%   mps_cpp_plot(reals,O);
%
% See also: mps_snesim_read_par, mps_snesim_write_par, mps_enesim_read_par,
% mps_enesim_write_par
%

function mps_cpp_plot(reals,O);


if (length(O.z)==1)&&(length(O.y)==1);
    % 1D
    return
end

if strcmp(O.method,'mps_genesim')
    strTG{1}='Accepted Distance';
    strTG{2}='#Counts in conditional pd';
    strTG{3}='Index in TI';
    strTG{4}='#Ite in TI';
    strTG{5}='#sed conditional points';
else
    strTG{1}='#Conditional points';
    strTG{2}='maxLevel';
    strTG{3}='sumCounters';
end

if (length(O.z)==1);
    % 2D
    
    figure(1);
    nsp=ceil(sqrt(O.n_real));
    for i=1:O.n_real;
        subplot(nsp,nsp,i);
        imagesc(O.x,O.y,reals(:,:,i));
        xlabel('X');ylabel('Y');title('First realization')
        axis image;
    end
    
    
    
    if O.debug>1
        figure(2);clf;
        
        for i=1:length(strTG);
            subplot(1,length(strTG),i);
            imagesc(O.x,O.y,O.(sprintf('TG%d',i)));
            xlabel('X');ylabel('Y');
            title(strTG{i})
            axis image;
            colorbar
        end
        
        if strcmp(O.method,'mps_genesim')
            
            % plot poisition in TI
            figure(3);clf;
            TI=read_eas_matrix(O.ti_filename);
            [ny,nx]=size(TI);
            j=0;
            for i=1:prod(size(reals(:,:,end)));
                [ix,iy]=ind2sub([nx,ny],O.TG3(i));
                if ~isnan(ix);
                    j=j+1;
                    ix_all(j)=ix;
                    iy_all(j)=iy;
                    d(j)=TI(iy,ix);
                end
                %plot(ix,iy,'ro','MarkerSize',1+12*(i/prod(O.simulation_grid_size)))
            end
            imagesc(TI);title('Training image');
            axis image;
            cax=caxis;
            colormap(gray)
            hold on
            cmap=hsv;
            
            %cmap=cmap_geosoft;
            nc=size(cmap,1);
            i_col=1+ceil((nc-1)*(d-cax(1))./(1+cax(2)-cax(1)));
            for i=1:length(d);
                cmap_scatter(i,:)=cmap(i_col(i),:);
            end
            s=scatter(ix_all,iy_all,10,cmap_scatter,'filled');
            colorbar
            
        end
        
        
    end
    
end




