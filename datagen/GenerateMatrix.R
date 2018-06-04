library(matrixcalc)
library(tictoc)

################################################################################
# Source: https://stat.ethz.ch/pipermail/r-help/2008-February/153708
# Generating a random positive-definite matrix with user-specified positive
# eigenvalues. If eigenvalues are not specified, they are generated from a
# uniform distribution
Posdef <- function (n, ev = runif(n, 0, 10)) 
{
  Z <- matrix(ncol=n, rnorm(n^2))
  decomp <- qr(Z)
  Q <- qr.Q(decomp) 
  R <- qr.R(decomp)
  d <- diag(R)
  ph <- d / abs(d)
  O <- Q %*% diag(ph)
  Z <- t(O) %*% diag(ev) %*% O
  return(Z)
}
################################################################################

write.matrix.to.file <- function(A, fname) {
  bin.file <- file(fname, "wb")
  for (i in c(1:nrow(A))) {
    writeBin(A[i,], bin.file)
  }
  close(bin.file)
}

generate.matrix <- function(N) {
  A <- Posdef(N)
  fname <- sprintf("../../data/matrix%i.dat", N)
  write.matrix.to.file(A, fname)
}

for (N in c(250, 500, 1000, 2500, 5000, 7500)) {
  tic(sprintf("Generate (%i x %i) matrix", N, N))
  generate.matrix(N)
  toc()
}

# Generate (250 x 250) matrix: 0.092 sec elapsed
# Generate (500 x 500) matrix: 0.323 sec elapsed
# Generate (1000 x 1000) matrix: 1.422 sec elapsed
# Generate (2500 x 2500) matrix: 20.82 sec elapsed
# Generate (5000 x 5000) matrix: 159.007 sec elapsed
# Generate (7500 x 7500) matrix: 533.853 sec elapsed
