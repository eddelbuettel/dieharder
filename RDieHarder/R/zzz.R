# $Id$
#.onLoad <- function(lib, pkg) {
#  library.dynam("RDieHarder", pkg, lib )
#}

.First.lib <- function(lib, pkg) {
  library.dynam("RDieHarder", pkg, lib )
  .dieharder.generators <<- dieharderGenerators()
}

