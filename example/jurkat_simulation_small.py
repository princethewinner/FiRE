import sys
sys.path.append('utils')

import numpy as np
import gzip
from scipy import stats

import preprocess as pp
import misc
import FiRE

#Create output directory
outputFolder = 'output_small/python/jurkat'
misc.mkdirp(outputFolder)

#Read preprocessed data
with gzip.GzipFile('data/preprocessedData_jurkat_two_species_1580.txt.gz', 'r') as fid:
    preprocessedData = np.genfromtxt(fid)
print('shape of matrix after preprocessing {}'.format(preprocessedData.shape))

#Read labels
labels = np.genfromtxt('data/labels_jurkat_two_species_1580.txt', dtype=np.int) #Cells with label '1' represent abundant, while cells with label '2' represent rare.
print('shape of labels : {}'.format(labels.shape))

#Genes
selGenes = np.genfromtxt('data/genesSel_jurkat_two_species_1580.txt')
print('shape of selected Genes after preprocessing {}'.format(selGenes.shape))

#Create model of FiRE.
model = FiRE.FiRE(L=100, M=50)
model.fit(preprocessedData)

#Assign FiRE score to every cell.
scores = np.array(model.score(preprocessedData))
np.savetxt('{}/scores_jurkat.txt'.format(outputFolder), scores)

#Apply IQR-based criteria to identify rare cells for further downstream analysis.
q3 = np.percentile(scores, 75)
iqr = stats.iqr(scores)
th = q3 + (1.5*iqr)

#Select indexes that satisfy IQR-based thresholding criteria.
indIqr = np.where(scores >= th)[0]
print('shape of selected cells : {}'.format(indIqr.shape))

#Create a file with binary predictions
predictions = np.zeros(preprocessedData.shape[0])
predictions[indIqr] = 1 #Replace predictions for rare cells with '1'.
np.savetxt('{}/predictions_jurkat.txt'.format(outputFolder), predictions, fmt="%d")

#For plot-only purpose, we had removed two outlying cells (For illustration purpose only)
indexesTokeep = np.array(range(1263) + range(1264,1537) + range(1538,1580)).astype(np.int)

scoresKeep = scores[indexesTokeep]
labelsKeep = labels[indexesTokeep]
predictionsKeep = predictions[indexesTokeep]

proj = np.genfromtxt('data/tsne_jurkat_1k_outlierRemoved.txt')

plotFileName = '{}/jurkat.png'.format(outputFolder)

misc.plot(proj, labelsKeep, scoresKeep, predictionsKeep, fname=plotFileName)



