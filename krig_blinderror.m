% krig_blinderror : Cross validation blind error
% CALL : 
%   [d_est,d_var,be,d_diff,L]=krig_blinderror(pos_known,val_known,pos_est,V,options,nleaveout)
%
function [d_est,d_var,be,d_diff,L]=krig_blinderror(pos_known,val_known,pos_est,V,options,nleaveout);

options.dummy='';

nknown=size(pos_known,1);
pos=1:1:nknown;

d_est=zeros(nknown,1);
d_var=zeros(nknown,1);

d2u=precal_cov(pos_known,pos_known,V,options);

% Make sure not to go into recirsive call loop
if isfield(options,'xvalid');
  options=rmfield(options,'xvalid');
end

for i=1:nknown
  if ((i/20)==round(i/20))
    %progress_txt(i,nknown,'Crossvalidation')
  end
  used=find(pos~=i);
  
  options.d2u=d2u(used,i);
  options.d2d=d2u(used,used);
  
  [d_est(i),d_var(i)]=krig(pos_known(used,:),val_known(used,:),...
                    pos_known(i,:),V,options);

  if (i/10)==round(i/10), 
    % mgstat_verbose(sprintf('%s cross validation  %d/%d',mfilename,i,nknown));
  end
end


d_diff=d_est-val_known(:,1);
be=mean(abs(d_diff));

  
if nargout==5
  % CALULATE LIKELIHOOD
  nd=size(val_known,1);
  Cd=zeros(nd,nd);
  for i=1:nd
    Cd(i,i)=d_var(i);
  end
  L=exp(-.5*d_diff'*inv(Cd)*d_diff);
  
end