#! /bin/csh -f


# ----------------------------------------------------------------------
# example submission:
# -------------------
# run -t ../../../../pcl.tar.gz -m batch -c ./prodNoComp.csh -r 'PFNS srm://t3se01.psi.ch:8443/srm/managerv2\?SFN=/pnfs/psi.ch/cms/trivcat%STORAGE1 /store/user/ursl/pixel/pcl%SITE T3_CH_PSI' pcl-data.py
# ----------------------------------------------------------------------

setenv CMSSW
setenv SCRAM_ARCH
setenv SRMCP

setenv JOB
setenv FILE1    $JOB.txt
setenv STORAGE1
setenv PFNS
setenv SITE

echo "========================"
echo "====> SGE  wrapper <===="
echo "========================"

echo "--> Running grid job wrapper"

# ----------------------------------------------------------------------
# -- The Basics
# ----------------------------------------------------------------------
echo "--> Environment"
date
echo "--> hostname"
hostname
echo "--> cat /proc/cpuinfo"
cat /proc/cpuinfo
echo "--> uname -a"
uname -a
echo "--> df -kl"
df -kl
echo "--> printenv"
printenv

echo $VO_CMS_SW_DIR
ls -l $VO_CMS_SW_DIR
source $VO_CMS_SW_DIR/cmsset_default.csh
echo "-> which edg-gridftp-ls"
which edg-gridftp-ls
echo "-> which globus-url-copy"
which globus-url-copy
echo "-> which srmcp"
which srmcp

pwd
echo "--> End of env testing"

# BATCH START

# ----------------------------------------------------------------------
# -- Setup CMSSW
# ----------------------------------------------------------------------
echo "--> Setup CMSSW"
pwd
date
cmsrel $CMSSW
cd $CMSSW
eval `scramv1 runtime -csh`
pwd

echo "--> Extract tar file"
date
tar zxf ../$JOB.tar.gz
# -- must run in test to have access to maps/*.txt files!
cd src/DPGAnalysis-SiPixelTools/PCL/test
mv ../../../../../$JOB.py .


# ----------------------------------------------------------------------
# -- Run cmsRun
# ----------------------------------------------------------------------
echo "--> Run cmsRun"
pwd
date
which cmsRun
echo "cmsRun $JOB.py "
cmsRun $JOB.py |& tee $JOB.log
date
pwd
ls -rtl
echo "double column counts"
cat $JOB.txt
ls -rtl

setenv ROOTFILE `ls *.txt`

# ----------------------------------------------------------------------
# -- Save Output to SE
# ----------------------------------------------------------------------
# create $PFNS/$STORAGE1
echo "--> Save output to SE: $PFNS/$STORAGE1/$FILE1"
echo " local txtfile: $ROOTFILE"
echo " job   txtfile: $FILE1"

echo lcg-del -b -D srmv2 -l  "$PFNS/$STORAGE1/$FILE1"
lcg-del -b -D srmv2 -l "$PFNS/$STORAGE1/$FILE1"
lcg-cp -b -D srmv2 $FILE1  "$PFNS/$STORAGE1/$FILE1"

# -- switch to data_replica.py
ls `pwd`/$FILE1 > dr.list
echo "--> cat dr.list: "
cat dr.list
echo "--> AM running data_replica.py: "
/mnt/t3nfs01/data01/swshare/psit3/bin/data_replica.py --from-site LOCAL --to-site $SITE dr.list "$STORAGE1"

echo "--> lcg-ls : $PFNS/$STORAGE1/$FILE1"
echo lcg-ls -b -D srmv2 -l  "$PFNS/$STORAGE1/$FILE1"
lcg-ls -b -D srmv2 -l  "$PFNS/$STORAGE1/$FILE1"


date

# BATCH END

echo "run: This is the end, my friend"
