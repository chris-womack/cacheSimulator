# cacheSimulator
Cache Simulator for ECEN 4593 (Comp Org)

## GitHub Crash Course

```bash
	# clones repo to local
	git clone https://github.com/chris-womack/cacheSimulator.git

	# this will add all new files in the dir that you've created
	git add .

	# add specific file
	git add <file Name>

	# commits or saves the changes you've made
	git commit -am "comment"

	# push to the master repo. this is were we run into confilcts
	git push origin master

	# creates branch that will be seperate from master
	git branch nate

	# moves from current branch or master to branch specified
	git checkout nate

	# this will merge branch with master so its another point of conflicts
	# while on seperate branch
	git merge master

```

## Using cacheSim
### Unzipped
```bash
./<simulator name> <config file> < <tracefile name>
./cachSim config.txt < traces-short/tr1
```
### Zipped
```bash
zcat <tracefile name> | <simulator name> <config file>
zcat bzip2.gz | cacheSim config.txt
```

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

* bzip2.gz 			Block sorting compression
* h264ref.gz 		Video compression
* omnetpp.gz 		Discrete event simulation library
* libquantum.gz 	Quantum computer simulation
* sjeng.gz 			Chess game

### Other Traces
The traces-1M and traces-5M directories contain the first 1 million
and 5 million instruction of the long traces. There are compressed and uncompressed versions of
the traces.
