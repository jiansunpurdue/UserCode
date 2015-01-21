# D meson reconstruction code

In this repository you can find the code to reconstruct D mesons in pp, pPb and PbPb collisions.
This code runs on RECO files and produce HiForest files that contains D meson candidates.
To run the D meson reconstruction and the foresting you need to follows the instructions below.

**Set up a forest environment and download the HF code**:

Please follows carefully the following steps:

 * cmsrel CMSSW_5_3_20
 * cd CMSSW_5_3_20/src
 * cmsenv
 * git cms-merge-topic -u CmsHI:forest_CMSSW_5_3_20
 * git clone https://github.com/jiansunpurdue/UserCode.git
 * scram b -j20
 
ATTENTION: the standard CMSSW_5_3_20 forest framework does not work for official MC samples, which will be produced in CMSSW_5_3_24.
So we need to use CMSSW_5_3_24 to forest the official MC samples (just change "cmsrel CMSSW_5_3_20" to "cmsrel CMSSW_5_3_24" in above
instruction). Whether to move to CMSSW_5_3_24 for PbPb and pp data forest prodution needs to be checked.

**Submit the foresting jobs**:

 * cd UserCode/OpenHF/test
 * In this directory you can find 4 python scripts that corresponds to MC pp/PbPb, Data pp/PbPb
 * Before running, remember to change the input/output file names and to set the proper number of events 
 * Now you are ready to run: cmsRun <filename>.py 

**Credits**:This package is built largely based on the Bs2MuMu p+p BPH software, therefore the credit should be mostly given to the corresponding authors.
