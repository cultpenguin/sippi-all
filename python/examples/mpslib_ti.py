#

import numpy as np
import matplotlib.pyplot as plt
import mpslib as mps

plt.ion()

'''
Show different types of well known training images
'''

plt.figure(1)
fig, axs = plt.subplots(2, 3, figsize=(15, 12), facecolor='w', edgecolor='k')
axs = axs.ravel()
for i in range(5):
    if i==0:
        TI, TI_filename = mps.trainingimages.lines()
    elif i==1:
        TI, TI_filename = mps.trainingimages.bangladesh()
    elif i==2:
        TI, TI_filename = mps.trainingimages.maze()
    elif i==3:
        TI, TI_filename = mps.trainingimages.strebelle()
    elif i==4:
        TI, TI_filename = mps.trainingimages.stones()

    axs[i].imshow(TI)
    axs[i].set_title(TI_filename)

plt.show(block=False)

'''
Show examples of simple 2D checkeboard training images
'''
plt.figure(2)
fig, axs = plt.subplots(2,2, figsize=(10, 10), facecolor='w', edgecolor='k')
axs = axs.ravel()
for i in range(4):
    TI, TI_filename = mps.trainingimages.checkerboard(40,40,i+1)
    axs[i].imshow(TI)
    axs[i].set_title(TI_filename + " - N%d " %i)

plt.suptitle('trainingimages.checkerboard')
plt.show(block=False)

'''
Show examples of simple 2D checkeboard training images
'''
plt.figure(3)
fig, axs = plt.subplots(2,3, figsize=(10, 6), facecolor='w', edgecolor='k')
axs = axs.ravel()
cell_x=8
cell_y=4
for i in range(6):
    TI, TI_filename = mps.trainingimages.strebelle(di=i+1)
    axs[i].imshow(TI)
    axs[i].set_title(TI_filename + " - N%d " %(i+1), fontsize=8)

plt.suptitle('Channels',fontsize=14)
plt.show(block=False)


'''
Show examples of the channels TI in different resolution
'''
plt.figure(3)
fig, axs = plt.subplots(2,4, figsize=(10, 6), facecolor='w', edgecolor='k')
axs = axs.ravel()
cell_x=8
cell_y=4
for i in range(4):
    TI, TI_filename = mps.trainingimages.checkerboard2(nx=40,ny=42, cell_x=4, cell_y=4, cell_2=2*i+1)
    axs[i].imshow(TI)
    axs[i].set_title(TI_filename + " - N%d " %i, fontsize=8)

plt.suptitle('trainingimages.checkerboard',fontsize=14)
plt.show(block=False)






#TI, TI_filename=mps.trainingimages.bangladesh()
#plt.subplot(222)
#plt.imshow(TI)
#plt.title(TI_filename)
#TI, TI_filename=mps.trainingimages.checkerboard()
#TI, TI_filename=mps.trainingimages.checkerboard2()
#TI, TI_filename=mps.trainingimages.checkerboard2(nx=40, ny=50, cell_x=8, cell_y=4, cell_2=10)
#TI, TI_filename=mps.trainingimages.checkerboard2(nx=100, ny=100, cell_x=4, cell_y=4, cell_2=3)
#TI, TI_filename=mps.trainingimages.checkerboard2(nx=100, ny=100, cell_x=14, cell_y=8, cell_2=5)
#TI, TI_filename=mps.trainingimages.checkerboard2(nx=40, ny=40, cell_x=14, cell_y=4, cell_2=9)
#TI, TI_filename=mps.trainingimages.maze()

plt.show()

'''
fig, axs = plt.subplots(2,5, figsize=(15, 6), facecolor='w', edgecolor='k')
fig.subplots_adjust(hspace = .5, wspace=.001)

axs = axs.ravel()

for i in range(10):

    axs[i].contourf(np.random.rand(10,10),5,cmap=plt.cm.Oranges)
    axs[i].set_title(str(250+i))
'''