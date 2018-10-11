#
# Copyright (C) 2018 Aashi Jindal, Prashant Gupta, Jayadeva, Debarka Sengupta (aashi.jindal@ee.iitd.ac.in, prashant.gupta@ee.iitd.ac.in, jayadeva@ee.iitd.ac.in, debarka@iiitd.com). All Rights Reserved.
#
# This file is part of FiRE.
#
# FiRE is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FiRE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FiRE.  If not, see <http://www.gnu.org/licenses/>.
#

from __future__ import division
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import seaborn as sns

def plotViolinCalander(expressions, clusterNames, geneNames, colorInfo=None, figsize=(6,6), linewidth=0.2, cm='jet', filename='violincalander', saveas=['png'], dpi=600, axisFontSize=11, gridColor='snow', rotationX=90):

    '''
        Signature:

            plotViolinCalander(expressions, clusterNames, geneNames, colorInfo=None, linewidth=0.2, cm='jet', filename='violincalander', saveas=['png'], dpi=600, axisFontSize=11, gridColor='snow', rotationX=90)

        Input:

            expressions     [required]  :   [nClusters, nGenes, -1] : 2-D numpy.array of list of floats
                                            This variable will contain expression of each gene for each sample of cluster

            clusterNames    [required]  :   [nClusters] : 1-D numpy.array or list (free of datatype - preferably string)
                                            Content of this variable will be displayed as rownames in plot

            geneNames       [required]  :   [nGenes] : 1-D numpy.array or list (free of datatype  - preferably string)
                                            Content of this varialbe will be displayed as columnnames in plot

            colorInfo       [optional]  :   [nGenes] : 1-D numpy.array or list of integer/float
                                            Content of this variable will decide the number of colors to be selected from the colormap.
                                            Genes with the same number in the variable will appear with the same color.
                                            If this variable is not set, number of different colors will be equal to nGenes.

            figsize         [optional]  :   Size of the figure : a tuple (xAxisLength, yAxisLength) of floats

            linewidth       [optional]  :   width of the line around violin plot : scalar float

            cm              [optional]  :   colormap : string or matplotlib.colors.LinearSegmentedColormap object

            filename        [optional]  :   Name of the output file (Avoid using format) : string

            saveas          [optional]  :   format of the figures (multiple formats can be provided together) : list of string

            dpi             [optional]  :   figure dpi : integer

            axisFontSize    [optional]  :   font size of the text diplayed on the axis as ticks.

            gridColor       [optional]  :   color of grid : string/hex code/rgba

            rotationX       [optional]  :   rotation angle for xAxis albels : float
    '''

    sharex='col'
    sharey='row'

    nrows, ncols = expressions.shape
    color = np.zeros((nrows, ncols), dtype=np.object)

    vmin = 0
    vmax = ncols
    if colorInfo is not None:
        vmax = np.unique(colorInfo).size

    norm = matplotlib.colors.Normalize(vmin=vmin, vmax=vmax)
    cmap = matplotlib.cm.get_cmap(cm)

    for i in range(nrows):
        for j in range(ncols):
            c = j
            if colorInfo is not None:
                c = colorInfo[c]
            color[i, j] = cmap(norm(c))

    print('[ViolinCalander] Making Calander ...')
    fig, ax = plt.subplots(nrows=nrows, ncols=ncols, sharex=sharex, sharey=sharey, gridspec_kw={'wspace':0, 'hspace':0}, figsize=figsize)

    print('[ViolinCalander] Plotting violin plots ...')
    for i in range(nrows):
        for j in range(ncols):
            cax = sns.violinplot(x=expressions[i, j], ax=ax[i, j], color=color[i, j], linewidth=linewidth)
            ax[i, j].tick_params(axis='x', which='both', bottom='off', top='off', labelbottom='off', labeltop='off', width=0, pad=0)
            ax[i, j].tick_params(axis='y', which='both', left='on', right='off', labelleft='off', labelright='off', width=0, pad=0)
            ax[i, j].spines['bottom'].set_color(gridColor)
            ax[i, j].spines['top'].set_color(gridColor)
            ax[i, j].spines['left'].set_color(gridColor)
            ax[i, j].spines['right'].set_color(gridColor)
        plt.sca(ax[i, 0])
        ax[i, 0].yaxis.tick_left()
        ax[i, 0].tick_params('y', labelsize=axisFontSize, colors='black')
        plt.yticks([0], [clusterNames[i]])

    for j in range(ncols):
        plt.sca(ax[0, j])
        ax[0, j].xaxis.tick_top()
        ax[0, j].tick_params('x', labelsize=axisFontSize, colors='black', rotation=rotationX)
        xmin, xmax = ax[0, j].get_xlim()
        xtickLocation = 0.5 * (xmax + xmin)
        plt.xticks([xtickLocation], [geneNames[j]])

    plt.tight_layout()
    for s in saveas:
        print('[ViolinCalander] saving as {}'.format(s))
        fig.savefig('{}.{}'.format(filename, s), format=s, dpi=dpi)
    plt.close()


if __name__ == '__main__':

    import itertools

    nRows = 10
    nCols = 4
    nSize = 10

    rowTicks = [''.join(list(i)) for i in list(itertools.permutations(['a', 'b', 'c', 'd', 'e'], 3))]
    colTicks = [''.join(list(i)) for i in list(itertools.permutations(['1', '2', '3', '4', '5'], 3))]


    #creating data for the plot
    np.random.seed(0)
    data = np.zeros((nRows, nCols), dtype=np.object)
    for i in range(nRows):
        for j in range(nCols):
            v = np.random.permutation(1000)[0]
            data[i, j] = np.random.random(v) * v

    plotViolinCalander(data, rowTicks[:nRows], colTicks[:nCols], filename='violincalander_test')
