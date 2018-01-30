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
    Deas = get_remote(url,local_file)
    Dmat = Deas['Dmat']
    return Dmat, local_file


def lines():
    local_file = 'ti_lines.dat';
    url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_lines_arrows.sgems';
    Deas = get_remote(url,local_file)
    Dmat = Deas['Dmat']
    return Dmat, local_file


def stones():
    local_file = 'ti_stones.dat';
    url = 'http://www.trainingimages.org/uploads/3/4/7/0/34703305/ti_stonewall.sgems';
    Deas = get_remote(url,local_file)
    Dmat = Deas['Dmat']
    return Dmat, local_file

def bangladesh():
    local_file = 'ti_bangladesh.dat';
    url = 'http://trainingimages.org/uploads/3/4/7/0/34703305/bangladesh.sgems';
    Deas = get_remote(url,local_file)
    Dmat = Deas['Dmat']
    return Dmat, local_file

def maze():
    local_file = 'ti_maze.dat';
    url = 'https://raw.githubusercontent.com/cultpenguin/mGstat/master/ti/maze.gslib';
    Deas = get_remote(url,local_file)
    Dmat = Deas['Dmat']
    return Dmat, local_file




def checkerboard(nx=40, ny=40, cellsize=4):

    import numpy as np
    TI=np.kron([[1, 0] * cellsize, [0, 1] * cellsize] * cellsize, np.ones((ny, nx)))

    local_file = 'ti_checkerboard.dat'
    return TI, local_file

def checkerboard2(nx=40, ny=50, cell_x=8, cell_y=4, cell_2=10):

    import numpy as np
#    cell_x = 10  # Height and width of checkerboard squares.
#    cell_y = 10  # Height and width of checkerboard squares.
#
#    nx = 40
#    ny = 40
    TI=np.zeros((ny,nx))

    for ix in range(nx):             # Note that i ranges from 0 through 7, inclusive.
        for iy in range(ny):           # So does j.
            if (ix%cell_x < (cell_x/2))==(iy%cell_y < (cell_y/2)):  # The top left square is white.
                TI[iy, ix] = 0
            else:
                TI[iy,ix]=1

            if (ix%cell_2 < (cell_2/2))&(iy%cell_2 < (cell_2/2)):  # The top left square is white.
                TI[iy, ix] = 2

            if (ix+iy)%cell_x==0:
                TI[iy, ix] = 3

    local_file = 'ti_checkerboard2.dat'
    return TI, local_file
