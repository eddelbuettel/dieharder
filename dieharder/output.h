 /*
  * We need a set of binary flags to control table-style output
  */
 typedef enum {
   TALL = 0,
   THEADER = 1,
   TGEN = 2,
   TNAME = 4,
   TSNAME = 8,
   TNTUPLE = 16,
   TTSAMPLES = 32,
   TPSAMPLES = 64,
   TPVALUE = 128,
   TASSESSMENT = 256
 } Table;

