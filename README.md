# Continuous Aggregate Monitor Graphical User INterface: CAMGUIN
A set of methods useful for reading a root tree, doing some analysis, and printing those results into an updatable ROOT file. This is a fully functioning macro whose methods stand alone in PANGUIN and in ROOT in general as well, and it represents several possible ways of accessing and manipulating ROOT output. 

## Usage 

Example script for how to use environment variables and PANGUIN to execute the CAMGUIN macros on data files

`setenv PATH "${ROOTSYS}/bin:${PATH}"`

`setenv LD_LIBRARY_PATH "${ROOTSYS}/lib:${LD_LIBRARY_PATH}"`

`setenv RUNNUM 0`

`setenv NRUNS -1`

`setenv QW_DATA      /lustre/expphy/volatile/halla/parity/cameronc/japan/data`

`setenv QW_ROOTFILES /lustre/expphy/volatile/halla/parity/cameronc/japan/rootfiles/`

`./panguin_wrapper -f analyze.cfg -r 1374 -n 3 -P`
