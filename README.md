
# FiRE - Finder of Rare Entities
Contents
    Installation Guide ...............................................[1](#install)<br />
    -Prerequisites ......................................................................[1.1](#pre)<br />
    -Installation Steps of FiRE software ..................................[1.2](#install-steps)<br />
    Data Pre-processing ..........................................[2](#data-pre-processing)<br />
    Usage of FiRE .....................................................[3](#usage-of-fire-software)<br />

<a name="install"></a><h1>Installation Guide</h1>

<a name="pre"></a><h2>Prerequisites</h2>
1. <h3>Required python modules</h3>
```python
    python 2.7.12
    cython >= 0.23.4s
    distutils >= 2.7.12
    numpy >= 1.13.3
    pandas >= 0.20.3
    statsmodels >= 0.8.0
```

2. <h3>Required cpp modules</h3>
```cpp
    g++ >= 4.8.4
    boost >= 1.54.0
```
FiRE only needs `<boost/random.hpp>` from boost. So, full installation is not necessary. It can be downloaded from [boost.org](https://www.boost.org/) and used as is.

<a name="install-steps"></a><h2>Installation Steps of FiRE software</h2>

```bash
    sudo ./INSTALL [ --boost-path <boost-path> | --log-file <log-file> | --inplace ]
    sudo ./UNISTALL [ --log-file <log-file-name> ]

```

<h3>If boost is installed at default location</h3>

```bash
    sudo ./INSTALL
```

<h3>If boost is installed at custom location</h3>

```bash
    sudo ./INSTALL --boost-path <full-path>

```
Example:
```bash
    sudo ./INSTALL --boost-path $HOME/boost/boost_1_54_0
```
Above installation steps will generate `fireInstall.log` file. It is advisable to keep this file, since it will be needed for uninstallation. Name of the log file can be modified during installation.

```bash
    ./INSTALL --log-file <log-file-name>
```

Above steps will install `FiRE` at the default location.

<h3>For `inplace` installation</h3>

```bash
    ./INSTALL --inplace
```

<h3> Uninstallation of FiRE Software.</h3>
If name of log file is not changed during installation,
```bash
    sudo ./UNISTALL
```
If name of log file is changed during installation,
```bash
    sudo ./UNISTALL --log-file <log-file-name>
```

<a name="data-pre-processing"></a><h1>Data Pre-processing</h1>
1. <h3>Load python libraries</h3>
```python
import numpy as np
```

2. <h3>Load Data in current environment.</h3>
```python
#Data matrix should only consist of values where rows represent cells and columns represent genes.
```

3. <h3>Copy preprocess.py in current working directory and load preprocess.</h3>
```python
import preprocess as pp
```

4. <h3> Call function ranger_preprocess for selecting thousand variable genes.</h3>
```python
preprocessedData, selGenes = pp.ranger_preprocess(data, genes, ngenes_keep=1000, dataSave='./', optionToSave=False, minLibSize=0, verbose=True)
```

|Parameter | Description | Required or Optional| Datatype | Default Value |
| -----:| -----:| -----:|-----:|-----:|
|data | Data for processing | Required | `np.array [nCells, nGenes]` | - |
|genes | Names of Genes | Required | `np.array [nGenes]` | - |
|ngenes_keep | Number of genes to keep | Optional | `integer` | 1000 |
|dataSave | Path to save results | Optional | `string` | Current working Directory (Used only when optionToSave is True) |
|optionToSave | Save processed output or not | Optional | `boolean` | False(Does not save) |
|minLibSize | Minimum number of expressed features | Optional | `integer` | 0 |
|verbose | Display progress | Optional | `boolean` | True(Prints intermediate results) |

```python
'''
Returned Value :
    preprocessedData : processed data matrix (log2 transformed) : np.array [nCells, nVariableGenes]
    selGenes         : Names of thousand variable genes selected : np.array [nVariableGenes]
'''
```




<a name="usage-of-fire-software"></a><h1>Usage of FiRE Software</h1>
1. <h3>Load python module of FiRE software.</h3>
```python
import FiRE
```

2. <h3>Create model of FiRE.</h3>
```python
model = FiRE.FiRE(L=100, M=50, H=1017881, seed=5489, verbose=0)
```

|Parameter | Description | Required or Optional| Datatype | Default Value |
| -----:| -----:| -----:|-----:|-----:|
|L | Total number of estimators | Required | `int` | - |
|M | Number of features to be randomly sampled for each estimator | Required | `int` | - |
|H | Number of bins in hash table | Optional | `int` | 1017881|
|seed | Seed for random number generator | Optional | `unsigned int` | 5489|
|verbose | Controls verbosity of program at run time (0/1) | Optional | `int` | 0 (silent) |

3. <h3>Apply model to the above dataset.</h3>
```python
model.fit(preprocessedData)
```

4. <h3>Calculate FiRE score of every cell.</h3>
```python
score = np.array(model.score(preprocessedData))
scoreFile = 'Results/score_hypothalamus.txt'
np.savetxt(scoreFile,score) #Save scores
```

5. <h3>Select cells with higher values of FiRE score, that satisfy IQR-based thresholding criteria. </h3>
```python
from scipy import stats

q3 = np.percentile(score, 75)
iqr = stats.iqr(score)
th = q3 + 1.5*iqr

indIqr = np.where(score >= th)[0]

dataSel = preprocessedData[indSel,:] #Select subset of cells
dataSelFile = 'Results/dataSel_hypothalamus.txt'
np.savetxt(dataSelFile,dataSel)
```

6. <h3>Run [dropClust](https://github.com/debsin/dropClust) and predict rare clusters.</h3>
    <!--![Predicted 12 rare clusters using dropClust](image/dropClustClusters.png =250x250)-->
    <img src="image/dropClustClusters.png" width="500" height="350" />

7. <h3>Find genes up-regulated for predicted rare clusters.</h3>


<!--![violin plot of markers](image/secondRareHypoViolin.png)-->
<img src="image/secondRareHypoViolin.png" width="500" height="700" />
