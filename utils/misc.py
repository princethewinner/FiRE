import os
import sklearn.manifold as skma
import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import cmocean
import numpy as np
import matplotlib.patches as mpatches

def mkdir(path):
    if not os.path.exists(path):
        print('Creating Dir : {}'.format(path))
        os.mkdir(path)


def mkdirp(path):

    strp = path.strip('/')
    paths = path.split('/')
    nDirs = len(paths)

    for i in range(1, nDirs + 1):
        curD = '/'.join(paths[:i])
        if(len(curD) > 0): mkdir(curD)

def getwd():
    return os.getcwd()

def run_tsne(data, **kwargs):

    tsne = skma.TSNE(**kwargs)
    projs = tsne.fit_transform(data)

    return projs

def plot(data, labels, scores, predictions, colors=['y', 'b'], rare=1, fname='plot.png'):

    fig, ax = plt.subplots(nrows=1, ncols=3, figsize=(9, 3))

    uniqueLabels = np.unique(labels)

    for c, i in enumerate(uniqueLabels):
        ind = np.where(labels == i)[0]
        ax[0].scatter(data[ind, 0], data[ind , 1], c=colors[c], s=5, label=i, edgecolors=None, marker='.', linewidths=0)
    #ax[0].legend()
    patches = [mpatches.Patch(color=colors[i], label=j) for i, j in enumerate(uniqueLabels)]
    ax[0].legend(handles=patches, ncol=1, shadow=False, frameon=True, fontsize='small', borderaxespad=0, framealpha=0.6)
    ax[0].tick_params(axis='x', which='both', bottom='off', top='off', labelbottom='off', labeltop='off', width=0, pad=0)
    ax[0].tick_params(axis='y', which='both', left='on', right='off', labelleft='off', labelright='off', width=0, pad=0)
    ax[0].set_xlabel('tSNE-1')
    ax[0].set_ylabel('tSNE-2')
    ax[0].set_xticklabels([])
    ax[0].set_yticklabels([])
    ax[0].set_title('a', fontweight='bold')

    sortedScoresInd = np.argsort(scores)
    cax = ax[1].scatter(data[sortedScoresInd, 0], data[sortedScoresInd, 1], cmap=cmocean.cm.thermal, s=5, c=scores[sortedScoresInd],
                    edgecolors=None, marker='.', linewidths=0)
    #ax[1].colorbar(cax)
    #ax[1].legend()
    ax[1].tick_params(axis='x', which='both', bottom='off', top='off', labelbottom='off', labeltop='off', width=0, pad=0)
    ax[1].tick_params(axis='y', which='both', left='on', right='off', labelleft='off', labelright='off', width=0, pad=0)
    ax[1].set_xlabel('tSNE-1')
    ax[1].set_ylabel('tSNE-2')
    ax[1].set_xticklabels([])
    ax[1].set_yticklabels([])
    ax[1].set_title('b', fontweight='bold')

    ax[2].scatter(data[:, 0], data[:, 1], s=5, c='lightgray', edgecolors=None, marker='.', linewidths=0)
    rareind = np.where(predictions == rare)[0]
    ax[2].scatter(data[rareind, 0], data[rareind, 1], s=5, c='black', edgecolors=None, marker='.', linewidths=0, label='rare')
    #ax[2].legend()
    patches = [mpatches.Patch(color='black', label='Rare')]
    ax[2].legend(handles=patches, ncol=1, fontsize='small', shadow=False, frameon=False, borderaxespad=0)
    ax[2].tick_params(axis='x', which='both', bottom='off', top='off', labelbottom='off', labeltop='off', width=0, pad=0)
    ax[2].tick_params(axis='y', which='both', left='on', right='off', labelleft='off', labelright='off', width=0, pad=0)
    ax[2].set_xlabel('tSNE-1')
    ax[2].set_ylabel('tSNE-2')
    ax[2].set_xticklabels([])
    ax[2].set_yticklabels([])
    ax[2].set_title('c', fontweight='bold')

    #plt.colorbar(ax=ax[1], cax=cax)
    plt.tight_layout()
    plt.savefig(fname, dpi=300)


