# BGOFit
This program is designed to fit the output of MakeBGOHistograms and suggest
BGO HV changes to align the 137Cs peaks

## Table of Contents
  * [Installation](#installation)
  * [Running MakeBGOHistograms](#running-makebgohistograms)
  * [Helper scripts](#helper-scripts)
    + [Compile.sh](#compilesh)

# Installation
1. Get the code, either via tarball or from github
```
git clone https://github.com/cnatzke-TRIUMF/BGOFit.git
```
2. Compile the programs via the included script
 ``` 
 cd BGOFit
 ./CompileBGOFit.sh
 ```

# Running BGOFit
The general form of input is:
```
./BGOFit histogram_file experiment enabled_channels
```

##### Parameters
```
histogram_file    The file containing sorted BGO-HPGe histograms
experiment        griffin/tigress
enabled_channels  A/B/BOTH
```

##### Outputs
```
HV_Change.txt  Text file containing channels and their respective voltage changes
BGO-Fits.root  ROOT file containing fitted histograms
```

# Helper scripts
Included is a simple (very simple) bash script to make compiling easier.

### Compile.sh
This script compiles the main code.
```
./CompileBGOFit.sh 
```


