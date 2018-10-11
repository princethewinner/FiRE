# distutils: language = c++
# distutils: sources = cppFiRE.cpp

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


'''
    Usage:
        import FiRE
        model = FiRE.FiRE(L=100, M=50, H=1017881, seed=5489, verbose=0)
        model.fit(data)
        scores = model.score(data)
'''

#
# This file is the cython wrapper of cppFiRE.c
#

#Import all required libs here
cimport FiRE
from FiRE cimport FiRE

#FiRE class definition
cdef class FiRE:
    '''
        Signature:
            FiRE(L, M, H=1017881, seed=5489, verbose=0)

        Input:
            L       : [required] : int      : scalar :                          : Total number of estimators
            M       : [required] : int      : scalar :                          : Number of features to be sampled
            H       : [optional] : size_t   : scalar : Default Value - 1017881  : Number of bins in hash table
            seed    : [optional] : size_t   : scalar : Default Value - 5489     : Seed for random number generator
            verbose : [optional] : [0/1]    : scalar : Default Value - 0        : Controls verbosity of output

    '''
    cdef cppFiRE* fire                                                                          #Class object holder
    def __cinit__(self, int L, int M, size_t H=1017881, size_t seed=5489, int verbose=0):       #Class constructor
        self.fire = new cppFiRE(L, M, H, seed, verbose)                                         #Since c++ constructor is not default, heap allocation
                                                                                                #is needed. (Don't forget to free memory later)

    def fit(self, vector[vector[float]]& X):                                                    #Method for generaing random values and tables
        '''
            Signature:
                FiRE.fit(X)

            Input:
                X : [required] : float : [samples x features] : Dataset
        '''
        self.fire.fit(X)

    def score(self, vector[vector[float]]& X):                                                  #Method for generating scores
        '''
            Signature:
                FiRE.score()
        '''
        return self.fire.score(X)

    def __repr__(self):                                                                         #Inter function to pretty print the class object
        return '<FiRE(L={}, M={}, H={}, seed={}, verbose={})>'.format(self.fire.L, self.fire.M, self.fire.H, self.fire.seed, self.fire.verbose)

    def __dealloc__(self):                                                                      #Deallocating the constructed object from heap

        if self.fire is not NULL:
            del self.fire

    @property
    def L(self):
        '''
            int : scalar : Number of estimators
        '''
        return self.fire.L

    @property
    def M(self):
        '''
            int : scalar : Number of feature to be sampled
        '''
        return self.fire.M

    @property
    def H(self):
        '''
            unsigned int : scalar : Numbers of bins in hash table
        '''
        return self.fire.H

    @property
    def seed(self):
        '''
            unsigned int : scalar : Seed for random number generator
        '''
        return self.fire.seed

    @property
    def verbose(self):
        '''
            0/1 : scalar : Controls verbosity
        '''
        return self.fire.verbose

    @property
    def bins(self):
        '''
            unsigned int : [L x H x -1] : Hash table across estimators
                                        : -1 represents dynamic size of dimension
        '''
        return self.fire.bins

    @property
    def dims(self):
        '''
            unsigned int : [L x M] : randomly generated dimensions
        '''
        return self.fire.dims

    @property
    def thresholds(self):
        '''
            float : [L x M] : randomly generated thresholds
        '''
        return self.fire.thresholds

    @property
    def weights(self):
        '''
            unsigned int : [L x M] : randomly generated weights
        '''
        return self.fire.weights
