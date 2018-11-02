
# FiRE - Finder of Rare Entities
## Contents
  [Introduction](#introduction)<br />
  [External dependencies](#etc)<br/>
  [Python Package](#python-demo)<br />
   -[Prerequisites](#pre-python)<br />
   -[Installation Steps](#install-steps-python)<br />
   -[Usage](#usage-python)<br />
  [R Package](#r-demo)<br />
   -[Prerequisites](#pre-R)<br />
   -[Installation Steps](#install-steps-R)<br />
   -[Usage](#usage-R)<br />
  [Publication](#publication)<br />
  [Copyright](#copyright)<br />
  [Patent](#patent)<br />

<a name="introduction"></a>
## Introduction

Tested on Ubuntu 14.04 and Ubuntu 16.04.

All results in manuscript have been generated using `python` module.

FiRE is available for `python` and `R`. Required versions and modules for both are as mentioned below. `cpp` modules are necessary for both of them.

<a name="etc"></a>
## External Dependencies
Following packages are required to run/install the FiRE software.

<h4>Required cpp modules</h4>

```cpp
    g++ >= 4.8.4
    boost >= 1.54.0
```

FiRE only needs `<boost/random.hpp>` from boost. So, full installation is not necessary. It can be downloaded from [boost.org](https://www.boost.org/) and used as is.


<a name="python-demo"></a>
## Python Package

<a name="pre-python"></a>
### Prerequisites

<h4>Required python modules</h4>

```python
    python 2.7
```

For FiRE module
```python
    cython >= 0.23.4
    distutils >= 2.7.12
```

For preprocessing
```python
    numpy >= 1.13.3
    pandas >= 0.20.3
    statsmodels >= 0.8.0
```

For demo
```python
    gzip >= 1.2.11 (zlib)
    scipy >= 1.1.0
    matplotlib >= 2.1.0
    cmocean >= 1.2
    sklearn >= 0.19.1
```

<a name="install-steps-python"></a>
### Installation Steps

```bash
    [sudo] ./INSTALL [ --boost-path <boost-path> | --log-file <log-file> | --inplace | --py | --R | --help ]
    [sudo] ./UNINSTALL_python
    [sudo] ./UNINSTALL_R

    --boost-path <boost-path>  : python | R    : Path to boost-library, if boost is not installed at default location, this value needs to be provided.
    --inplace                  : python        : Required only for python, if set, inplace build will be run and resulting lib will be stored in python/FiRE.
    --log-file <log-file>      : python        : Required only for python, ignored with --inplace set.
    --py                       : python        : Install FiRE in python environment.
    --R                        : R             : Install FiRE in R environment.
    --help                     : python | R    : Display this help.

    Info:

    UNINSTALL_[python | R] files are generated upon installation.
```

Typically, FiRE module takes a few seconds to install. A snippet of installation time taken by FiRE (in seconds) on a machine with Intel® Core™ i5-7200U (CPU @ 2.50GHz × 4), with 8GB memory, and OS Ubuntu 16.04 LTS is as follows

```bash
real 2.92
user 2.73
sys 0.18
```

<h4>If boost is installed at default location</h4>

```bash
    sudo ./INSTALL --py
```

<h4>If boost is installed at custom location</h4>

```bash
    sudo ./INSTALL --boost-path <full-path> --py

```
Example:
```bash
    sudo ./INSTALL --boost-path $HOME/boost/boost_1_54_0 --py
```
Above installation steps will generate `fireInstall.log` file. It is advisable to keep this file, since it will be needed for uninstallation. Name of the log file can be modified during installation.

```bash
    ./INSTALL --log-file <log-file-name> --py
```

Above steps will install `FiRE` at the default location.

<h4>For `inplace` installation</h4>

```bash
    ./INSTALL --inplace [--py]
```

<h4> Uninstallation of FiRE Software.</h4>

```bash
    [sudo] ./UNINSTALL_python
```

<a name="usage-python"></a>
### Usage

<a name="r-demo"></a>
## R Package

<a name="pre-R"></a>
### Prerequisites

<h4>Required R modules</h4>

```R
    R >= 3.2.3
    Rcpp >= 0.12.19
```

<a name="install-steps-R"></a>
### Installation Steps

<a name="usage-R"></a>
### Usage

<a name="publication"></a>
## Publication

<a name="copyright"></a>
## Copyright

<a name="patent"></a>
## Patent
