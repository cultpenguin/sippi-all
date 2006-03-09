% fresnel_punch : computes the sensitivity kernel for a wave traveling from S to R.
%
% CALL : 
%    [K,RAY,timeS,timeR,raypath]=fresnel_punch(Vel,x,y,z,S,R,freq,alpha);
%
% IN : 
%    Vel : Velocity field
%    x [1:nx] :
%    y [1:ny] :
%    z [1:nz] :
%    S [1,3] : Location of Source
%    R [1,3] : Location of Receiver
%    freq : frequency
%    alpha: controls exponential decay away ray path
%
% OUT :
%    K : Sensitivity kernel
%    R : Ray sensitivity kernel (High Frequency approx)
%    timeS : travel computed form Source
%    timeR : travel computed form Receiver
%    raypath [nraydata,ndim] : the center of the raypath 
%
% TMH/2006
%
function [K,RAY,tS,tR,raypath,raylength]=fresnel_punch(Vel,x,y,z,S,R,freq,alpha,x0,y0,z0,dx);

  if nargin<7, freq=7.7; end
  if nargin<8, alpha=1; end
  
  Vpunch=Vel';
  
  tS=punch(Vpunch(:),x,y,z,S)';
  tR=punch(Vpunch(:),x,y,z,R)';

  T=tS+tR;T=T-min(T(:));

  K=munk_fresnel_2d(freq,T,alpha,1./tS,1./tR);

  % NOW FIND FIRST ARRIVAL AND RAYLENGTH  
  str_options = [.1 1000];
  [xx,yy]=meshgrid(x,y);
  [U,V]=gradient(tS);
  start_point=R;
  raypath = stream2(xx,yy,-U,-V,start_point(1),start_point(2),str_options);
  
  raypath=raypath{1};
  raypath=[raypath;S(1:2)];

  raylength=sum(sqrt(diff(raypath(:,1)).^2+diff(raypath(:,2)).^2));
  
  ix=ceil((raypath(:,1)-(x0-dx/2))./dx);
  iy=ceil((raypath(:,2)-(y0-dx/2))./dx);

  ix(find(ix<1))=1;
  iy(find(iy<1))=1;
  
  
  RAY=K.*0;
  for j=1:length(ix)
    RAY(iy(j),ix(j))=RAY(iy(j),ix(j))+1;
  end
  
  doPlot=1;
  if doPlot==1;
    figure(1);
    subplot(2,4,1)
    imagesc(x,y,Vel);axis image,
    subplot(2,4,2)
    imagesc(x,y,tS);axis image,
    subplot(2,4,3)
    imagesc(x,y,tR);axis image,
    subplot(2,4,4)
    imagesc(x,y,K);axis image,
    
    subplot(2,4,5)
    imagesc(x,y,U);axis image,
    subplot(2,4,6)
    imagesc(x,y,V);axis image,
    drawnow
    subplot(2,4,7)    
    imagesc(x,y,K);axis image
    hold on
    plot(S(1),S(2),'r*')
    plot(R(1),R(2),'ro')
    plot(raypath(:,1),raypath(:,2),'r.','Markersize',.1)

    plot(x(ix),y(iy),'ro')
    title(['l=',num2str(raylength)])
    hold off
    
    subplot(2,4,8)    
    imagesc(x,y,RAY);axis image
    
    hold off
    drawnow;
    
    
    
  end

  