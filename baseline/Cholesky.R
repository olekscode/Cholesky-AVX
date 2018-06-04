library(tictoc)

N <- 5000

############################## Reading the matrix ##############################

file.name <- sprintf("../../data/matrix%i.dat", N)
bin.file <- file(file.name, "rb")

tic("Reading the matrix")

A <- matrix(readBin(bin.file, numeric(), n = N ^ 2), nrow = N)

toc()

close(bin.file)

############################ Cholesky decomposition ############################

tic("Choleski decomposition")

U <- chol(A)

toc()