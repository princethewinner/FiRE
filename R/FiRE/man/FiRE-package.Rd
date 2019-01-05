\name{FiRE-package}
\alias{FiRE}
\alias{Rcpp_FiRE}
\docType{package}
\title{
  Finder of Rare Entities (FiRE) - To assign rareness/ outlierness score to every sample
}
\description{
  This package sorts samples as per their rareness/ oultierness. Instead of dichotomized decisions, FiRE assigns rareness/ outlierness score to every sample. These scores can then be used to identify rare samples or outliers, with varying degrees of rareness. FiRE takes multiple estimations of the proximity between a pair of samples, in low-dimensional spaces to compute scores. }
\details{
  FiRE is written in c++ and wrapped with Rcpp to create an R interface. To use FiRE, an object with the number of estimators (L), number of dimensions to be sampled per estimator (M), number of bins per estimator (H = 1017881), seed for random number generator (seed = 0) and verbose level (verbose = 0/1) needs to be created. Once the object is created, \code{fit} function needs to be called to hash all the samples into bins. Once hashing is done, \code{score} function needs to be called to retrieve score for every sample. Sample commands may be seen in the Examples section. The resulting model has four model parameters which may be accessed, dimensions (d) of size M sampled per estimator, thresholds (ths) of size M per d, weights(w) of size M generated per estimator and bin (b) of size H per estimator.
}
\author{
Prashant Gupta, prashant10991@gmail.com

Aashi Jindal, jindal.aashi21@gmail.com

Jayadeva, iitjd4@gmail.com

Debarka Sengupta, debarka@gmail.com

Maintainer: Prashant Gupta <prashant10991@gmail.com>
}
\references{
  [1]Jindal, A., Gupta, P., Jayadeva and Sengupta, D., 2018. Discovery of rare cells from voluminous single cell expression data. Nature Communications, 9(1), p.4719.

  [2]Wang, Z., Dong, W., Josephson, W., Lv, Q., Charikar, M. and Li, K., 2007, June. Sizing sketches: a rank-based analysis for similarity search. In ACM SIGMETRICS Performance Evaluation Review (Vol. 35, No. 1, pp. 157-168). ACM.
}
\keyword{ Finder of Rare Entities, Rare, Rare cells, Rare events, Hashing, Outlier detection, Anomaly detection }
\examples{

     ## L <- number of estimators
     ## M <- Number of dims to sample
     ## H <- Number of bins
     ## seed <- seed for random number generator
     ## verbose <- verbose level

     library('FiRE')
     data(sample_data) #Samples * Features

     data(sample_label)
     ## Samples with label '1' represent abundant,
     ## while samples with label '2' represent rare.

     ## Saving rownames and colnames
     rnames <- rownames(sample_data)
     cnames <- colnames(sample_data)

     ## Converting data.frame to matrix
     sample_data <- as.matrix(sample_data)
     sample_label <- as.matrix(sample_label)

     L <- 100 # Number of estimators
     M <- 50 # Dims to be sampled

     # Model creation without optional parameter
     model <- new(FiRE::FiRE, L, M)

     ## There are 3 more optional parameters they can be passed as
     ## model <- new(FiRE::FiRE, L, M, H, seed, verbose)

     ## Hashing all samples
     model$fit(sample_data)

     ## Computing score of each sample
     rareness_score <- model$score(sample_data)

     ## Apply IQR-based criteria to identify rare samples for further downstream analysis.
     q3 <- quantile(rareness_score, 0.75)
     iqr <- IQR(rareness_score)
     th <- q3 + (1.5*iqr)

     ## Select indexes that satisfy IQR-based thresholding criteria.
     indIqr <- which(rareness_score >= th)

     ## Create a vector for binary predictions
     predictions <- rep(1, dim(sample_data)[1])
     predictions[indIqr] <- 2 #Replace predictions for rare samples with '2'.

     ## To access model parameters
     ## Parameters - generated weights
     # model$w

     ## Parameters - sample dimensions
     # model$d

     ## Parameters - generated thresholds
     # model$ths

     ## Parameters - Bins
     # model$b
}
