
stopifnot(require(RDieHarder, quiet=TRUE))

dh <- dieharder("rand", "runs", seed=12345)

print(summary(dh))
