# -*- coding: utf-8 -*-
"""
Created on Wed Jan 24 19:36:26 2018

@author: thoma
"""

import os.path
from . import eas
import urllib.request

 

def get_remote(url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_strebelle.sgems',local_file = 'ti.dat'):
    if not (os.path.exists(local_file)):
        print('Beginning download of ' + url + ' to ' + local_file)
        urllib.request.urlretrieve(url, local_file)  
    
    print('Loading ' + local_file)
    D = eas.read(local_file)
    return D


def strebelle():    
    local_file = 'ti_strebelle.dat';
    url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_strebelle.sgems';
    D = get_remote(url,local_file)    
    
    return D

def lines():
    local_file = 'ti_lines.dat';
    url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_lines_arrows.sgems';
    D = get_remote(url,local_file)    
    return D

def stones():
    local_file = 'ti_stones.dat';
    url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_stonewall.sgems';
    D = get_remote(url,local_file)    
    return D

def maze():
    local_file = 'ti_maze.dat';
    url = 'https://github.com/cultpenguin/mGstat/blob/master/ti/maze.dat';
    D = get_remote(url,local_file)    
    return D




