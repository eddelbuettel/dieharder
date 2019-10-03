
.dieharder.generators <- c("borosh13",
                           "cmrg",
                           "coveyou",
                           "fishman18",
                           "fishman20",
                           "fishman2x",
                           "gfsr4",
                           "knuthran",
                           "knuthran2",
                           "knuthran2002",
                           "lecuyer21",
                           "minstd",
                           "mrg",
                           "mt19937",
                           "mt19937_1999",
                           "mt19937_1998",
                           "r250",
                           "ran0",
                           "ran1",
                           "ran2",
                           "ran3",
                           "rand",
                           "rand48",
                           "random128-bsd",
                           "random128-glibc2",
                           "random128-libc5",
                           "random256-bsd",
                           "random256-glibc2",
                           "random256-libc5",
                           "random32-bsd",
                           "random32-glibc2",
                           "random32-libc5",
                           "random64-bsd",
                           "random64-glibc2",
                           "random64-libc5",
                           "random8-bsd",
                           "random8-glibc2",
                           "random8-libc5",
                           "random-bsd",
                           "random-glibc2",
                           "random-libc5",
                           "randu",
                           "ranf",
                           "ranlux",
                           "ranlux389",
                           "ranlxd1",
                           "ranlxd2",
                           "ranlxs0",
                           "ranlxs1",
                           "ranlxs2",
                           "ranmar",
                           "slatec",
                           "taus",
                           "taus2",
                           "taus113",
                           "transputer",
                           "tt800",
                           "uni",
                           "uni32",
                           "vax",
                           "waterman14",
                           "zuf",
                           "dev/random",
                           "dev/urandom",
                           "empty",
                           "file_input",
                           "file_input_raw",
                           "ca",
                           "R_wichmann_hill",
                           "R_marsaglia_multic.",
                           "R_super_duper",
                           "R_mersenne_twister",
                           "R_knuth_taocp",
                           "R_knuth_taocp2"
                           )

dieharderGenerators <- function() {
  val <- .Call("dieharderGenerators", PACKAGE="RDieHarder")
  return(val[[1]])
}

.dieharder.tests <- c("birthdays",
                      "operm5",
                      "rank_32x32",
                      "rank_6x8",
                      "bitstream",
                      "opso",
                      "oqso",
                      "dna",
                      "count_1s_stream",
                      "count_1s_byte",
                      "parking_lot",
                      "2dsphere",
                      "3dsphere",
                      "squeeze",
                      "sums",
                      "runs",
                      "craps",
                      "marsaglia_tsang_gcd",
                      "marsaglia_tsang_gorilla",
                      "rgb_timing",
                      "rgb_persist",
                      "rgb_bitdist",
                      "rgb_lmn",
                      "sts_monobit",
                      "sts_runs",
                      "sts_block")
 
dieharder <- function(rng="mt19937",
                      test=1,
                      psamples=100,
                      seed=0,
                      verbose=FALSE) {
  UseMethod("dieharder")
}

dieharder.default <- function(rng="mt19937",
                              test="runs",
                              psamples=100,
                              seed=0,
                              verbose=FALSE) {

  if (length(rng) > 1) {
    warning("Only one rng argument supported in dieharder")
  }
  if (is.character(rng)) {
    ## dieharder's C code indexes these from 0 to 73, not 1 to 74
    gen <- charmatch(rng, .dieharder.generators) - 1
  } else {
    gen <- rng - 1
  }
  if (is.na(gen)) {
    warning("rng argumement ", rng, " unknown")
    return(NULL)
  }
  if (gen == 0 && rng != 1) {
    warning("rng argumment ", rng, " ambiguous")
    return(NULL)
  }

  if (length(test) > 1) {
    warning("Only one test argument supported in dieharder")
  }
  if (is.character(test)) {
    ## note that dieharder uses a 0-based C array, so subtract one
    runtest <- charmatch(test, .dieharder.tests) 
  } else {
    runtest <- test 
  }
  if (is.na(runtest)) {
    warning("test argumement ", test, " unknown")
    return(NULL)
  }
  if (runtest == 0) {
    warning("test argumment ", test, " ambiguous")
    return(NULL)
  }

  
  val <- .Call("dieharder",
               as.integer(gen),
               as.integer(runtest),
               as.integer(seed),
               as.integer(psamples),
               as.integer(verbose),
               PACKAGE="RDieHarder")
  ##class(val) <- c("dieharder", "test")
  obj <- list(p.value=val[[1]],
              data=val[[2]],      ## not used bu htest methods
              method=val[[3]],
              data.name=paste("Created by RNG `",
                .dieharder.generators[gen+1], "' with seed=",
                as.integer(seed), ", sample of size ",
                as.integer(psamples), sep=""),
              generator=.dieharder.generators[gen+1]
              )
  class(obj) <- c("dieharder", "htest")
  return(obj)
}

plot.dieharder <- function(x, ...) {

  local.par <- par(mfrow=c(2,1), mar=c(2,4,3,1), oma=c(0,0,2.5,0))

  vec <- x$data

  hist(vec, probability=TRUE, main="Histogram and Density estimate",
       xlab="", ylab="density")
  lines(density(x$data, from=0, to=1), col='darkgray')

  #qqplot(vec, seq(0, 1, length.out=length(vec)),
  #       main="QQ-Plot", ylab="Uniform sequence", xlab="")

  plot(ecdf(x$data), main="ECDF", ylab="", xlab="",
       verticals= TRUE, do.p = FALSE)
  segments(0,0,1,1, col='darkgray', lty="dotted")

  mtext(text = paste(x$method, "applied to data generated by", 
          x$generator), outer = TRUE, cex = 1.3, font = 2, line = 1)
  pksk <- round(x$p.value, 4)
  pks <- round(ks.test(x$data, "punif", 0, 1, exact=TRUE)$p.value, 4)
  pw <- round(wilcox.test(x$data, mu=0.5)$p.value, 4)
  mtext(text = paste("Test p-values: ",
          pksk, " (Kuiper-K-S),  ",
          pks, " (K-S),  ",
          pw, "  (Wilcoxon)"),
        outer = TRUE, cex = 1.0, font = 1, line = 0)
  
  par(local.par)
  invisible(x)
}

print.dieharder <- function(x, ...) {
  stats:::print.htest(x, ...)     # why do I need to explicitly dispatch?
  invisible(x)
}

summary.dieharder <- function(object, ...) {
  print(object, ...)
  cat("\nSummary for test data\n")
  print(summary(object$data))
  cat("\n\nStem and leaf plot for test data\n")
  print(stem(object$data))
  print(ks.test(object$data, "punif", 0, 1, exact=TRUE))
  print(wilcox.test(object$data, mu=0.5))
  invisible(object)
}

##

#dieharderVec <- function(gen=c(22,69), test=1, verbose=FALSE) {
#
#  val <- .Call("dieharderVectorised",
#                as.list(as.integer(gen)),
#                as.integer(test),
#                as.integer(verbose),
#                PACKAGE="RDieHarder")
#   return(val)
#}


