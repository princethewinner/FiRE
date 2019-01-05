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

// [[Rcpp::depends(BH)]]

#include "Rcpp.h"
#include <vector>
#include <cstddef>
#include <boost/random.hpp>                             //Required for random number generator (mersenne_twister). Needs boost library.
#include <cfloat>                                       //Required for FLT_MAX macro.
#include <cmath>                                        //Required for log function.
#include <sstream>
#include <fstream>


std::string IntToString(int x){

    std::ostringstream ss;
    ss << x;
    return ss.str();

}

class FiRE{
    //Class Variables
    public: int L;                                                              //Number of estimators
    public: int M;                                                              //Number of features to be randomly sampled for each estimator
    public: unsigned int H;                                                     //Number of bins
    public: unsigned int seed;                                                  //Seed for random number generator
    public: int verbose;                                                        //Controls verbosity of program (0/1)
    private: int size_;                                                         //Total number of samples in provided data
    private: int dim;                                                           //Total number of features in provided data
    private: double min_;                                                       //Minimum value in the whole data
    private: double max_;                                                       //Maximum value in the whole data
    private: std::vector< std::vector < int > > dims;                           //Container for randomly generated M feature index for each estimator
    private: std::vector< std::vector< double > > thresholds;                   //Container for randomly generated M threshold
                                                                                //corresponding to randomly generated M feature indexes for each estimator
    private: std::vector< std::vector< unsigned int > > weights;                         //Containder for randomly generated M weights
                                                                                //corresponding to randomly generated M feature indexes for each estimator
    private: std::vector< std::vector< std::vector< int > > > bins;             //Containder for hash table for each estimator

    //Class methods Private
    private: void __getTables();                                                     //Private method for generating random tables.
    private: void __getBins(Rcpp::NumericMatrix& X);                                 //Private method for generating hash table.

    //Class methods Public
    public: FiRE(int L, int M, int H, int seed, int verbose);                           //Class constructor
    public: void fit(Rcpp::NumericMatrix& X);                                           //Public method to fit data - This method call for random table
                                                                                        //generation and hash table generation
    public: Rcpp::NumericVector score(Rcpp::NumericMatrix& X);                          //Public method to compute score
    public: Rcpp::NumericMatrix ths();
    public: Rcpp::IntegerMatrix d();
    public: Rcpp::NumericMatrix w();
    public: void dump_w(Rcpp::String);
    public: Rcpp::List b();
};


//Random Number generator setup
typedef boost::mt19937 Rng;                             //mersenne_twister random number generator
typedef boost::uniform_real<float> uniform;             //real number generator
typedef boost::uniform_int<int> uniformInt;             //integer number generator
typedef boost::uniform_int<unsigned> uniformUnsigned;   //unsigned integer number generator


//' @param x An integer vector
//' @return None
// [[Rcpp::export]]
FiRE::FiRE(int L, int M, int H=1017881, int seed=5489u, int verbose=0){
    this->L = L;
    this->M = M;
    this->H = H;
    this->seed = seed;
    this->verbose = verbose;
}

void FiRE::__getTables(){

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

void FiRE::__getBins(Rcpp::NumericMatrix& X){

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
                _a = (X(j, _d) > _t)?1:0;
                index += (_p * _a);
            }
            index = index % this->H;                //Computing bin index of hash table.
            this->bins[i][index].push_back(j);      //Inserting sample index in the computed bin of the hash table.
        }
    }

}

void FiRE::fit(Rcpp::NumericMatrix& X){

    float _min;
    float _max;

    int i, j;

    this->size_ = X.rows();
    this->dim = X.cols();

    _min = FLT_MAX;                                                 //Default value of min
    _max = -1 * FLT_MAX;                                            //Default value of max

    if(this->verbose > 0)
        Rcpp::Rcout << "Getting min and max of data\n";

    for(i=0; i<this->size_; i++){
        for(j=0; j<this->dim; j++){
            if(X(i, j) > _max) _max = X(i, j);
            if(X(i, j) < _min) _min = X(i, j);
        }
    }

    this->min_ = _min;                                              //minimum value of dataset
    this->max_ = _max;                                              //maximum value of dataset

    if(this->verbose > 0)
        Rcpp::Rcout << "Getting tables\n";
    this->__getTables();                                            //Call for getting random tables

    if (this->verbose > 0)
        Rcpp::Rcout << "Getting bins\n";
    this->__getBins(X);                                             //Call for filling hash tables

}

Rcpp::NumericVector FiRE::score(Rcpp::NumericMatrix& X){

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
                _a = (X(j, _d) > _t)?1:0;
                index += (_p * _a);
            }
            index = index % this->H;                                    //Getting bin index of hash table
            lf += log((this->bins[i][index].size()*1.0)/this->size_);   //Gathering neighborhood information
        }
        _scores[j] = -2 * lf;                                           //Computing scores
    }
    return Rcpp::NumericVector(_scores.begin(), _scores.end());
}

Rcpp::NumericMatrix FiRE::ths(){

    Rcpp::NumericMatrix mat(this->L, this->M);
    Rcpp::CharacterVector rname;

    for(int i=0; i < this->L; i++){

        Rcpp::NumericVector temp(this->thresholds.at(i).begin(), this->thresholds.at(i).end());

        mat(i, Rcpp::_) = temp;
        rname.push_back("estimator_" + IntToString(i+1));
    }

    rownames(mat) = rname;

    return mat;

}

Rcpp::NumericMatrix FiRE::w(){

    Rcpp::NumericMatrix mat(this->L, this->M);
    Rcpp::CharacterVector rname;

    for(int i=0; i < this->L; i++){

        Rcpp::NumericVector temp(this->weights.at(i).begin(), this->weights.at(i).end());

        mat(i, Rcpp::_) = temp;
        rname.push_back("estimator_" + IntToString(i+1));
    }

    rownames(mat) = rname;

    return mat;

}

Rcpp::IntegerMatrix FiRE::d(){

    Rcpp::IntegerMatrix mat(this->L, this->M);
    Rcpp::CharacterVector rname;

    for(int i=0; i < this->L; i++){

        Rcpp::IntegerVector temp(this->dims.at(i).begin(), this->dims.at(i).end());

        mat(i, Rcpp::_) = temp;
        rname.push_back("estimator_" + IntToString(i+1));
    }

    rownames(mat) = rname;

    return mat;

}

Rcpp::List FiRE::b(){

    Rcpp::List list3d = Rcpp::List::create();
    std::string rname;
    std::string bname;

    for(int i = 0; i < this->L; i++){

        Rcpp::List list2d = Rcpp::List::create();

        for(int j=0; j < this->H; j++){

            if(this->bins.at(i).at(j).size() > 0){
                Rcpp::NumericVector temp(this->bins.at(i).at(j).begin(), this->bins.at(i).at(j).end());
                bname = "bin_" + IntToString(j+1);
                list2d.push_back(temp, bname);
            }
        }

        rname = "estimator_" + IntToString(i+1);
        list3d.push_back(list2d, rname);
    }

    return list3d;

}
