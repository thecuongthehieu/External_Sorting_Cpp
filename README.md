# External Sorting Cpp
Implementation of External Sorting In C++

# Build
  `$ make`
 
# Usage
  `$ ./main {intput-file} {output-file} {RAM-size (Bytes)}`
  
# Example
  `$ make` <br />
  `$ ./main input.txt output.txt 4000000000`
  
# Review 

Code is written fine: correct processing big files, memory limit more than 2gb, getline() is checked carefully.
But no checks for input agruments, debug output is written to stdout (instead of stderr), using C function instead of C++ versions (stoull, time).
Over complicated comparator instead of line < line
Solution works too slow (10 minutes) due to flushing each line (endl instead '\n').
Ok to call him!
Solution works correctly
