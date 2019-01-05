\name{FiRE}
\alias{Rcpp_FiRE-class}
\docType{class}
\title{
  Constructor for Class
}
\description{
  Constructor to create an instance of FiRE class. There are two ways in which constructor can be created.
}
\details{
    For usage see example.
}
\arguments{
    \item{L}{Number of estimators.}
    \item{M}{Number of dimensions to be sampled per estimator.}
    \item{H}{Number of bins. Default=1017881}
    \item{seed}{seed for random number generator. Default=0}
    \item{verbose}{verbose level. Default=1}
}

\examples{

     L <- 100
     M <- 50
     H <- 107881
     seed <- 0
     verbose <- 1

     ## Creating class object with required arguments
     model <- new(FiRE::FiRE, L, M)

     ## Creating class object with all arguments
     model <- new(FiRE::FiRE, L, M, H, seed, verbose)

}

