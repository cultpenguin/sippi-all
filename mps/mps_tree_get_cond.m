% mps_tree_get_cond: cond conditional from template
%
% Call:
%   [c_pdf,c,d_cond_use]=mps_tree_get_cond(ST,d_cond);
%
%   INPUT
%   ST: Search tree
%   d_cond: conditional data event on order of the template
%           unknown values are coded by 'NaN'
%
%   OUTPUT
%   c_pdf: conditional probabily of outcome at center note given d_cond
%   c: The counte matches leading to c_pdf
%   d_cond_use: The actual conditinoal data event after pruning
%   Nprune: number of prunings in the conditional event
%
%
%
% see also: mps_tree_get_cond_notes, mps_tree_populate
%
function [c_pdf,c,d_cond_use,d_cond,Nprune]=mps_tree_get_cond(ST,d_cond,cat,options)

% if ~isfield(options,'min_prunung')
%     options.min_prunung=0;
% end
if nargin<2, d_cond=[];end
if nargin<3, cat=0:1:(length(ST{1}.count)-1); end

i_notes=[]; % contain the index of all the 'conditional' notes to be summed
n_cond=length(d_cond);
j=0;
while isempty(i_notes) %|| j<=options.min_pruning;
  j=j+1;
  use_cond=1:(n_cond-j+1);
  i_notes=mps_tree_get_cond_notes(ST,d_cond(use_cond),cat);
  if isempty(i_notes)
    mgstat_verbose(sprintf('%s pruning conditional data ',mfilename),1);
  end
end

% if j>1
%    Nprune=(j-1)-sum(isnan(d_cond(n_cond-j+2:n_cond)));
% else
%    Nprune=0;
% end
Nprune=j-1;

for jj=1:length(i_notes)
  if jj==1
    c=ST{i_notes(jj)}.count;
  else
    c=c+ST{i_notes(jj)}.count;
  end
end

% Optionally: prune if only very few counts are found
doUsePruneForSmallCount=0;
if doUsePruneForSmallCount==1
  min_count=10;
  if sum(c)<min_count
    % d_cond(1:(length(d_cond)-1))
    [~,c,d_cond_use]=mps_tree_get_cond(ST,d_cond(1:(length(d_cond)-1)),cat,options);
  else
    d_cond_use=d_cond(use_cond);
  end
  
  else
  d_cond_use=d_cond(use_cond);
end


c_pdf=c./sum(c);




