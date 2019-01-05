\name{fit}
\alias{fit}
\title{
  Hash samples in bins
}
\description{
  Hashing samples into bins, using Sketching as hash function. This function repeats hashing process \code{L} times by sampling \code{M} dimensions at a time and hashes samples into one of \code{H} bins.
}
\details{
    For usage see example.
}
\arguments{
    \item{data}{On which rarity score needs to be computed. Required to be a \code{matrix}. (can be converted to matrix with \code{as.matrix})}
}


\note{
    This function does not do any preprocessing, so preprocessed data must be passed if required.
}

\examples{
  \dontrun{

     ## Creating class object with required arguments
     model <- new(FiRE::FiRE, L, M)
     model$fit(data)

  }
}

