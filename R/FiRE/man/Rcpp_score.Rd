\name{score}
\alias{score}
\title{
  Compute score on hashed sample.
}
\description{
  Once hashing is done using \code{fit}, compute score via proximity estimation.
}
\details{
    For usage see example.
}
\arguments{
    \item{data}{On which rarity score needs to be computed.}
}

\examples{
  \dontrun{

     ## Creating class object with required arguments
     model <- new(FiRE::FiRE, L, M)
     model$fit(data)
     score <- model$score(data)

  }
}

