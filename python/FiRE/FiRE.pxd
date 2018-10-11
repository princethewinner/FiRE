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


#
# This file is the cython wrapper of cppFiRE.h
#

#Import all required libs here.
from libcpp.vector cimport vector

#All typedef declerations here
ctypedef unsigned int uint32_t


cdef extern from "cppFiRE.h":
    cdef cppclass cppFiRE:

        #Class variables
        int L                                                   #Number of estimators
        int M                                                   #Number of features to be randomly sampled for each estimator
        size_t H                                                #Number of bins
        size_t seed                                             #Seed for random number generator
        int verbose                                             #Controls verbosity of program (0/1)
        vector[vector[uint32_t]] dims                           #Container for randomly generated M feature index for each estimator
        vector[vector[float]] thresholds                        #Container for randomly generated M threshold
                                                                #corresponding to randomly generated M feature indexes for each estimator
        vector[vector[uint32_t]] weights                        #Containder for randomly generated M weights
                                                                #corresponding to randomly generated M feature indexes for each estimator
        vector[vector[vector[uint32_t]]] bins                   #Containder for hash table for each estimator

        #Class methods
        cppFiRE(int, int, size_t, size_t, int) except +         #Class constructor
        void fit(vector[vector[float]]&)                        #Public method to fit data - This method call for random table
                                                                #generation and hash table generation
        vector[float] score(vector[vector[float]]&)             #Public method to compute score
