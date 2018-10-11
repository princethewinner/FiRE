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
 * This file contains definitions of declerations of cppFiRE.h
 *
 */

//Include all header file here.
#include "cppFiRE.h"                                    //Carries declaration of cppFiRE class
#include <boost/random.hpp>                             //Required for random number generator (mersenne_twister). Needs boost library.
#include <cfloat>                                       //Required for FLT_MAX macro.
#include <cmath>                                        //Required for log function.

//Random Number generator setup
typedef boost::mt19937 Rng;                             //mersenne_twister random number generator
typedef boost::uniform_real<float> uniform;             //real number generator
typedef boost::uniform_int<int> uniformInt;             //integer number generator
typedef boost::uniform_int<unsigned> uniformUnsigned;   //unsigned integer number generator


/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * cppFiRE : Class constructor                                                                                                                      *
 *                                                                                                                                                  *
 * Input -                                                                                                                                          *
 * L :          [required], int                                     Total number of estimators                                                      *
 * M :          [required], int                                     Number of random features to be sampled for each estimator                      *
 * H :          [optional], unsigned int, Default:1017881           Number of bins                                                                  *
 * seed :       [optional], unsigned int, Default:5489              Seed for random number generator                                                *
 * verbose :    [optional], 0/1, Default:0                          Controls verbosity of program at run time (0 - silent, 1 - display progress)    *
 *                                                                                                                                                  *
 * Returns -                                                                                                                                        *
 * Object Instance.                                                                                                                                 *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/
cppFiRE::cppFiRE(int L, int M, unsigned int H=1017881, unsigned int seed=5489u, int verbose=0){
    this->L = L;
    this->M = M;
    this->H = H;
    this->seed = seed;
    this->verbose = verbose;
}


/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * __getTables : private class methods                                                                                                              *
 *                                                                                                                                                  *
 * Input-                                                                                                                                           *
 * None                                                                                                                                             *
 *                                                                                                                                                  *
 *              This function generates random numbers for all the required parameters.                                                             *
 *              This function also sets up three class variables.                                                                                   *
 *                  dims :      [L x M] : unsigned int 2D vector    : This container stores M times randomly sampled feature index from uniform     *
 *                                                                    distribution in the range [0, data dim] for all L projectors.                 *
 *                                                                                                                                                  *
 *                  threshods : [L x M] : float 2d vector           : This container stores M times randomly sampled threshold from uniform         *
 *                                                                    distribution in the range [data min, data max]. These thresholds are          *
 *                                                                    in accord with values stored in dims container for all L projectors.          *
 *                                                                                                                                                  *
 *                  weights :   [L x M] : unsigned int 2D vector    : This container stores randomly sampled weight vectors of M dimensions for all *
 *                                                                    L projectors.                                                                 *
 *                                                                                                                                                  *
 * Returns -                                                                                                                                        *
 * void                                                                                                                                             *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/
void cppFiRE::__getTables(){

    std::vector< std::vector< std::vector<float> > > _tables;
    int i, j;

    Rng rng(this->seed);
    this->dims.resize(this->L);
    this->thresholds.resize(this->L);
    this->weights.resize(this->L);

    for(i=0; i<this->L; i++){

        this->dims[i].resize(this->M);
        this->thresholds[i].resize(this->M);
        this->weights[i].resize(this->M);

        for(j=0; j<this->M; j++){
            this->dims[i][j] = boost::variate_generator<Rng &, uniformUnsigned>(rng, uniformUnsigned(0, this->dim - 1))();  //Generating random feature index

            if(this->min_ != this->max_)
                this->thresholds[i][j] = boost::variate_generator<Rng &, uniform>(rng, uniform(this->min_, this->max_))();  //Generating random threshold for corresponding feature index
            else
                this->thresholds[i][j] = this->min_;
        }
        for(j = 0; j<this->M; j++)
            this->weights[i][j] = rng();                                                                                    //Generating random weight vector
    }
}


/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * __getBins : private class method                                                                                                                 *
 *                                                                                                                                                  *
 * Input -                                                                                                                                          *
 * None                                                                                                                                             *
 *              This function creates the hash table for each estimator.                                                                            *
 *              This function sets up one class variable.                                                                                           *
 *                  bins :  [L, H, -1] : unsigned int 3D vector : This container stors the hash table for each estimator.                           *
 *                                     : -1 signifies that number of element in the dimension is dynamic.                                           *
 *                                                                                                                                                  *
 * Returns -                                                                                                                                        *
 * void                                                                                                                                             *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/
void cppFiRE::__getBins(std::vector< std::vector<float> >& X){

    uint32_t index, _a;
    uint32_t _p;
    uint32_t _d;
    int i, j, k;
    float _t;

    this->bins.resize(this->L);
    for(i=0; i<this->L; i++){
        this->bins[i].resize(this->H);

        for(j=0; j<this->size_; j++){
            index = 0;
            for(k=0; k<this->M; k++){
                _d = this->dims[i][k];
                _t = this->thresholds[i][k];
                _p = this->weights[i][k];
                _a = (X[j][_d] > _t)?1:0;
                index += (_p * _a);
            }
            index = index % this->H;                //Computing bin index of hash table.
            this->bins[i][index].push_back(j);      //Inserting sample index in the computed bin of the hash table.
        }
    }

}


/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * fit : public class method                                                                                                                        *
 *                                                                                                                                                  *
 * input -                                                                                                                                          *
 * X        [required], float, [samples x features],    Dataset                                                                                     *
 *                                                                                                                                                  *
 *              This function is called to fit the model. (Basically this fucntions first create the table of random numbers                        *
 *              and then generated hash tables.)                                                                                                    *
 *                                                                                                                                                  *
 *                                                                                                                                                  *
 * Returns -                                                                                                                                        *
 * void                                                                                                                                             *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/
void cppFiRE::fit(std::vector< std::vector<float> >& X){

    float _min;
    float _max;

    int i, j;

    this->size_ = X.size();
    this->dim = X[0].size();

    _min = FLT_MAX;                                                 //Default value of min
    _max = -1 * FLT_MAX;                                            //Default value of max

    if(this->verbose > 0)
        std::cout << "Getting min and max of data" << std::endl;

    for(i=0; i<this->size_; i++){
        for(j=0; j<this->dim; j++){
            if(X[i][j] > _max) _max = X[i][j];
            if(X[i][j] < _min) _min = X[i][j];
        }
    }

    this->min_ = _min;                                              //minimum value of dataset
    this->max_ = _max;                                              //maximum value of dataset

    if(this->verbose > 0)
        std::cout << "Getting tables" << std::endl;
    this->__getTables();                                            //Call for getting random tables

    if (this->verbose > 0)
        std::cout << "Getting bins" << std::endl;
    this->__getBins(X);                                             //Call for filling hash tables

}


/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * score : public class method                                                                                                                      *
 *                                                                                                                                                  *
 * input -                                                                                                                                          *
 * X        [required], float, [samples x features],    Dataset                                                                                     *
 *                                                                                                                                                  *
 *              This function computes the FiRE score based on the hash table.                                                                      *
 *                                                                                                                                                  *
 * Returns -                                                                                                                                        *
 * scores :         float, [samples],       Calculated Score.                                                                                       *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/
 std::vector<float> cppFiRE::score(std::vector< std::vector<float> >& X){

    std::vector<float> _scores;

    uint32_t index, _a;
    uint32_t _p;
    uint32_t _d;
    int i, j, k;
    float _t, lf;

    _scores.resize(this->size_);
    for(j=0; j<this->size_; j++){                                       //revisiting steps for index calculation
        lf = 0;
        for(i=0; i<this->L; i++){
            index = 0;
            for(k=0; k<this->M; k++){
                _d = this->dims[i][k];
                _t = this->thresholds[i][k];
                _p = this->weights[i][k];
                _a = (X[j][_d] > _t)?1:0;
                index += (_p * _a);
            }
            index = index % this->H;                                    //Getting bin index of hash table
            lf += log((this->bins[i][index].size()*1.0)/this->size_);   //Gathering neighborhood information
        }
        _scores[j] = -2 * lf;                                           //Computing scores
    }
    return _scores;
}
