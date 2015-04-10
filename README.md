# cacheSimulator
Cache Simulator for ECEN 4593 (Comp Org)

## Using cacheSim
### Unzipped
'''bash
./<simulator name> <config file> < <tracefile name>
./cachSim config.txt < traces-short/tr1
'''
### Zipped
'''bash
zcat <tracefile name> | <simulator name> <config file>
zcat bzip2.gz | cacheSim config.txt
'''

## Traces
### Short Traces
Located in traces-short.
The short traces are:

  * tr1
  * tr2
  * tr3
  * tr4
  * tr5
  * tr6

### Long Traces
Located in traces-long. (will upload later)
The long (production) traces are:

* bzip2.gz        Block sorting compression
* h264ref.gz      Video compression
* omnetpp.gz      Discrete event simulation library
* libquantum.gz   Quantum computer simulation
* sjeng.gz        Chess game

### Other Traces
The traces-1M and traces-5M directories contain the first 1 million
and 5 million instruction, respectively, of the long traces.
For these, there are compressed and uncompressed versions of
the traces.
