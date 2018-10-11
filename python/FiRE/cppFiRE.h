/*
 * Copyright (C) 2018 Aashi Jindal, Prashant Gupta, Jayadeva, Debarka Sengupta (aashi.jindal@ee.iitd.ac.in, prashant.gupta@ee.iitd.ac.in, jayadeva@ee.iitd.ac.in, debarka@iiitd.com). All Rights Reserved.
 *
 * This file is part of FiRE.
 *
 * FiRE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FiRE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FiRE.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 *
 * This file contains declerations for FiRE interface.
 *
 */

#ifndef __FiRE__
#define __FiRE__

//Include all header file here.
#include <vector>
#include <cstddef>


//All typedef declerations here
typedef unsigned int uint32_t;

class cppFiRE{
    //Class Variables
    public: int L;                                                          //Number of estimators
    public: int M;                                                          //Number of features to be randomly sampled for each estimator
    public: unsigned int H;                                                 //Number of bins
    public: unsigned int seed;                                              //Seed for random number generator
    public: int verbose;                                                    //Controls verbosity of program (0/1)
    private: int size_;                                                     //Total number of samples in provided data
    private: int dim;                                                       //Total number of features in provided data
    private: float min_;                                                    //Minimum value in the whole data
    private: float max_;                                                    //Maximum value in the whole data
    public: std::vector< std::vector < uint32_t > > dims;                   //Container for randomly generated M feature index for each estimator
    public: std::vector< std::vector< float > > thresholds;                 //Container for randomly generated M threshold
                                                                            //corresponding to randomly generated M feature indexes for each estimator
    public: std::vector< std::vector< uint32_t > > weights;                 //Containder for randomly generated M weights
                                                                            //corresponding to randomly generated M feature indexes for each estimator
    public: std::vector< std::vector< std::vector< uint32_t > > > bins;     //Containder for hash table for each estimator

    //Class methods Private
    private: void __getTables();                                                     //Private method for generating random tables.
    private: void __getBins(std::vector< std::vector<float> >& X);                   //Private method for generating hash table.

    //Class methods Public
    public: cppFiRE(int L, int M, unsigned int H, unsigned int seed, int verbose);      //Class constructor
    public: void fit(std::vector< std::vector<float> >& X);                             //Public method to fit data - This method call for random table
                                                                                        //generation and hash table generation
    public: std::vector<float> score(std::vector< std::vector<float> >& X);             //Public method to compute score
};

#endif
