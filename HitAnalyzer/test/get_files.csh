# dbs search --query='find file, file.numevents where run=172998 and site=caf.cern.ch and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' > bla.txt

#dbs search --query='find file where run=173236 and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' >! bla.txt
#dbs search --query='find file where run=173389 and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' >! bla.txt
#dbs search --query='find file where run=173406 and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' >! bla.txt
#dbs search --query='find file where run=173438 and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' >! bla.txt
#dbs search --query='find file where run=173439 and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' >! bla.txt


#dbs search --query='find file where run=173619 and dataset=/Interfill/Run2011A-v1/RAW' 
#dbs search --query='find file where run=175260 and dataset=/Interfill/Run2011A-v1/RAW' 
#dbs search --query='find file where run=208686 and dataset=/MinimumBias/Run2012D-v1/RAW' 

# COSMICS 
#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2015-v1/RAW run=239342'
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Commissioning2015-v1/RAW run=239342'
#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2015-PromptReco-v1/RECO run=237956'
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Commissioning2015-PromptReco-v1/RECO run=237956'

# Cosmics 
das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-v1/RAW run=248566' # NO
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-PromptReco-v1/RECO run=248566' # NO
#das_client.py --limit=0 --query='file dataset=/Cosmics/Run2015A-PromptReco-v1/RECO run=248566' # NO

# Collisions 
# RAW
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-v1/RAW run=247324' # YES
#RECO 
#das_client.py --limit=0 --query='file dataset=/ExpressPhysics/Run2015A-Express-v1/FEVT run=246963' # YES
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-PromptReco-v1/RECO run=247607' # NO
#das_client.py --limit=0 --query='file dataset=/Commissioning/Run2015A-PromptReco-v1/RECO run=248025' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-PromptReco-v1/RECO run=248025' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-PromptReco-v1/RECO run=247607' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-PromptReco-v1/RECO run=247324' # YES
# Lumi
#das_client.py --limit=0 --query='file dataset=/AlCaLumiPixels/Run2015A-LumiPixels-PromptReco-v1/ALCARECO run=248025' # YES


# 2015-A
# /eos/cms/store/data/Run2015A/MinimumBias/RECO/PromptReco-v1/000/246
#/eos/cms/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/908/00000
#/eos/cms/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/908/00000

#/eos/cms/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/963

#/eos/cms/store/data/Run2015A/MinimumBias/RAW/v1/000/246
#/eos/cms/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/246
#/eos/cms/store/data/Run2015A/Commissioning/RAW/v1/000/246/  <---
#/eos/cms/store/data/Run2015A/ZeroBias1/RAW/v1/000/246/ <---

