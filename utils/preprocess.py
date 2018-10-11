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

import numpy as np
import pandas as pd
from statsmodels import robust

# ---------------------------------------------
# Preprocessesing : similar to pipeline
# described in Zheng et al.
# ---------------------------------------------

def ranger_preprocess(data, genes, ngenes_keep=1000, dataSave='./', optionToSave=False, minLibSize=0, verbose=True):
    '''
        Signature :

            ranger_preprocess(data, genes, ngenes_keep, dataSave, optionToSave, minLibSize, verbose)

        Input :

            data          [required] :    [nSamples, nGenes] : 2-D numpy.array of floats
            genes         [required] :    [nGenes]           : 1-D numpy.array of string
            ngenes_keep   [optional] :    [integer]          : Total number of genes to keep
            dataSave      [optional] :    [string]           : string to dir to save results
            optionToSave  [optional] :    [boolean]          : variable to decide whether to save results or not
            minLibSize    [optional] :    [integer]          : Minimum count of genes expressed in a cell
            verbose       [optional] :    [boolean]          : Intermediate progress to be shown or not

    '''

    dataFil,selGenesInd = normalize_by_umi_2(data, dataSave, minLibSize, verbose)
    selGenesInd = selGenesInd.astype(np.int)
    selGenesNames = genes[selGenesInd]

    if verbose:
        print('Select variable genes\n')
    df = get_variable_gene(dataFil)

    if verbose:
        print('Sort top Genes\n')
    disp_cut_off = np.sort(df['dispersion_norm'])[::-1][ngenes_keep-1]

    df['used'] = df['dispersion_norm'] >= disp_cut_off
    df_ = df.sort_values(['dispersion_norm'], ascending=False, inplace=False)
    featuresInd = df_.index[:ngenes_keep].astype(np.int)
    selGenes = selGenesNames[featuresInd]
    dataFilGene = dataFil[:, featuresInd]

    logDataFil = np.log2(dataFilGene+1)

    if optionToSave:
        if verbose:
            print('Writing selected genes\n')
        np.savetxt('{}/genesSel.txt'.format(dataSave), selGenes, fmt="%s")
        if verbose:
            print('Writing log normalized matrix\n')
        np.savetxt('{}/dataSel.txt'.format(dataSave), logDataFil)
    return logDataFil,selGenes

# --------------------------------------------------
# Get variable genes from normalized UMI counts
# --------------------------------------------------

def get_variable_gene(m):
    '''
        Signature :

            get_variable_gene(m)

        Input :

            m   [required] :  [nSamples, nGenes] : 2-D numpy.array of floats

    '''

    df = pd.DataFrame()
    colMeans = np.mean(m, axis=0)
    cv = np.std(m, axis=0)/colMeans
    var = np.var(m, axis=0)

    df['mean'] = colMeans
    df['cv'] = cv
    df['var'] = var
    dispersion = var/colMeans
    df['dispersion'] = dispersion

    listQ = range(10,105,5)
    quant = np.append(-np.inf, np.percentile(df['mean'], listQ))
    quant = np.append(quant, np.inf)
    df['mean_bin'] = pd.cut(df['mean'], quant)

    var_by_bin = {}
    for i in np.unique(df['mean_bin']):
        ind = np.where(df.values[:, -1] == i)[0]
        selDf = df.values[ind, 3]
        var_by_bin[i] = [np.median(selDf), robust.mad(selDf.astype(np.float))]

    df['bin_disp_median'] = np.zeros(df.values.shape[0])
    df['bin_disp_mad'] = np.zeros(df.values.shape[0])
    for i in range(df.values.shape[0]):
        bin_ = df.values[i, 4]
        df.at[i, 'bin_disp_median'] = var_by_bin[bin_][0]
        df.at[i, 'bin_disp_mad'] = var_by_bin[bin_][1]
    df['dispersion_norm'] = np.abs(df['dispersion'] - df['bin_disp_median'])/df['bin_disp_mad']
    return df

# ---------------------------------------------
# Normalize the gene barcode matrix by umi
# Filter based on read count first
# ---------------------------------------------

def normalize_by_umi_2(data, dataSave, minLibSize, verbose):
    '''
        Signature :

            normalized_by_uni_2(data, dataSave, cellFiltering, minLibSize, verbose)

        Input:

            data          [required] :    [nSamples, nGenes] : 2-D numpy.array of floats
            dataSave      [optional] :    [string]           : string to dir to save results
            minLibSize    [optional] :    [integer]          : Minimum count of genes expressed in a cell
            verbose       [optional] :    [boolean]          : Intermediate progress to be shown or not

    '''

    if minLibSize > 0:
        keepCellIndex = []
        for i in range(data.shape[0]):
            counts = np.sum(data[i,:] > 0)
            if counts > minLibSize:
                keepCellIndex += [i]
        keepCellIndex = np.array(keepCellIndex).astype(np.int)
        np.savetxt('{}/keepCellIndex.txt'.format(dataSave), keepCellIndex, fmt="%d")
        data = data[keepCellIndex,:]
        if verbose:
            print('Dimensions of matrix after cell filtering {}'.format(data.shape))

    dataBin = np.zeros((data.shape))
    for i in range(data.shape[1]):
        ind = np.where(data[:, i] > 2)[0]
        dataBin[ind, i] = 1

    cs = np.sum(dataBin, axis=0)
    selGenesInd = np.where(cs > 3)[0]
    dataFil = data[:, selGenesInd]
    if verbose:
        print('Dimensions of matrix after genes filtering : {}\n'.format(dataFil.shape))

    rs = np.sum(dataFil, axis=1, keepdims=True)
    rs_med = np.median(rs)
    umi_count_norm = rs/rs_med
    dataFil_norm = dataFil/umi_count_norm
    return dataFil_norm, selGenesInd



