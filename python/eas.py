#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 27 16:06:14 2017

@author: tmeha
"""
import numpy as np

def read(filename='eas.dat'):
    
    file = open(filename,"r") ;
    
    eas={};
    eas['title'] = (file.readline()).strip('\n');    
    
    dim_arr=eas['title'].split()
    if len(dim_arr)==3:
        eas['dim'] = {};
        eas['dim']['nx'] = np.int(dim_arr[0])        
        eas['dim']['ny'] = np.int(dim_arr[1])        
        eas['dim']['nz'] = np.int(dim_arr[2])        
    
    eas['n_cols'] = np.int(file.readline());    
    
    eas['list'] = [];
    for i in range(0, eas['n_cols']):
        print (i)
        h_val = (file.readline()).strip('\n');
        eas['list'].append(h_val);
    
        
    D_temp = file.read().splitlines();   
    eas['D']=np.zeros( (len(D_temp),1) );
    for i in range(0, len(D_temp)):
        eas['D'][i] = np.float(D_temp[i]);
        
    eas['Dmat']=eas['D'].reshape((eas['dim']['ny'],eas['dim']['nx']));   
#   

 # READ THE REAST OF THE DATA
#    data = [];
#    line="-"; 
#    i=0;
#    while line!="":
#        i=i+1;
#        cur_line = (file.readline()).strip('');
#        cur_line.strip('\n');
#        print ("%d +  -- " % i)
#        #print (eas['line'])
#        #data.append(1);  
#        
#        #= np.genfromtxt(file.readline());    
#    #header['label'] = file.readline();    
#    #header['line4'] = file.readline();    
#         
#    
#    
    
    file.close();
    
    return eas;

