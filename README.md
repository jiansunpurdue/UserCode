# D meson reconstruction code

In this repository you can find the code to reconstruct D mesons in pp, pPb and PbPb collisions.
This code runs on RECO files and produce HiForest files that contains D meson candidates.
To run the D meson reconstruction and the foresting you need to follows the instructions below.

**Set up a forest environment and download the HF code**:

ATTENTION: the standard forest framework does not work in our case. 
Please follows carefully the following steps:

 * cmsrel CMSSW_5_3_24
 * cd CMSSW_5_3_24/src
 * cmsenv
 * git cms-merge-topic -u CmsHI:forest_CMSSW_5_3_20
 * git clone https://github.com/jiansunpurdue/UserCode.git
 * scram b -j20
 
**Submit the foresting jobs**:

 * cd UserCode/OpenHF/test
 * In this directory you can find 4 python scripts that corresponds to MC pp/PbPb, Data pp/PbPb
 * Before running, remember to change the input/output file names and to set the proper number of events 
 * Now you are ready to run: cmsRun <filename>.py 

**Credits**:This package is built largely based on the Bs2MuMu p+p BPH software, therefore the credit should be mostly given to the corresponding authors.
