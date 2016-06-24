###############################################################################
#
#  RunSimulator.py
#  use to run the nEXO detector simulation either on a login node or a batch
#  Somewhat of a work in progress, please direct questions to joalbert@indiana.edu
# 
#  Trying to add SLAC batch functionality without messing up other too much...
#
###############################################################################


import os, stat, math



###### Specifiy parameters for the simulation
PWD = os.getcwd()
PathTonEXOBuild = '/nfs/slac/g/exo/jbalbert/nEXOsim/built/nEXO'
PathToClusteringAlgorithm = '/nfs/slac/g/exo/jbalbert/nEXOsim/trunk/Reconstruction_Scripts/clustering.py'
#HoldingDir = PWD + "/multifiles-120214"
HoldingDir = "/nfs/slac/g/exo/user-data/jbalbert/nEXOsim/test-120714-HFE/"

PreClustRootFileName = "PreClusterTh"
PostClustRootFileName = "PostClusterTh"
#IsotopeList = ["Bi212", "Tl208", "Ac228"] # Th chain important gamma emitters
#IsotopeList = ["Bi214 Pb214 Pa234"] U chain important gamma emitters
IsotopeList = ["Tl208"]
RegionName = "HFE"


NumJobs = 10             # number of separate jobs per isotope per region
NumDecaysPerJob = 4000  # number of decays per each job
ClusterDiam = 3          # cluster diameter for use in clustering script
RandomSeedOffset = 4096  # random seeds start with this value

ClusterDepsFlag = 1      # run the script to cluster energy deposits
UseClusterFlag = 1       # use a batch cluster to run many jobs
### End Parameter Region



###### Specify region code associated with region name. This will need to be
     # changed as the geometry changes
def RegionBuilder(RegionName):
    if (RegionName == "ActiveRegion"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 200 cm
/gps/pos/radius 200 cm
/gps/pos/confine /nEXO/TPCInternals/LXe/ActiveRegion"""
    elif (RegionName == "InactiveRegion"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 200 cm
/gps/pos/radius 200 cm
/gps/pos/confine /nEXO/TPCInternals/LXe/InactiveRegion"""
    elif (RegionName == "Cathode"):
        FullRegionCode = """
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/pos/radius 64.6 cm
/gps/pos/rot1 0 1 0
/gps/pos/rot2 0 0 1
/gps/pos/centre 63.2 0 0 cm
/gps/ang/type iso"""
    elif (RegionName == "Cathode2"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 200 cm
/gps/pos/radius 200 cm
/gps/pos/confine /nEXO/TPCInternals/LXe/Cathode"""
    elif (RegionName == "TPC"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 150 cm
/gps/pos/radius 150 cm
/gps/pos/confine /nEXO/TPCVessel"""
    elif (RegionName == "HFE"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 500 cm
/gps/pos/radius 500 cm
/gps/pos/confine /nEXO/TPCExternals/InnerSphericalCryostat/HFE"""
    elif (RegionName == "InnerCryo"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 500 cm
/gps/pos/radius 500 cm
/gps/pos/confine /nEXO/TPCExternals/InnerSphericalCryostat"""
    elif (RegionName == "OuterCryo"):
        FullRegionCode = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 500 cm
/gps/pos/radius 500 cm
/gps/pos/confine /nEXO/TPCExternals/OuterSphericalCryostat"""

    return FullRegionCode
### End Region Builder Region

   

###### Define dictionaries for use in script formatting
AtomicNumberDict = {
    "Bi214" : 83,
    "Xe137" : 54,
    "Rn222" : 86,
    "Po218" : 84,
    "Pb214" : 82,
    "Bi214" : 83,
    "Po214" : 84,
    "Pb210" : 82,
    "Bi210" : 83,
    "Po210" : 84,
    "Th232" : 90,
    "U238"  : 92,
    "Co60"  : 27,
    "Ra228" : 88,
    "Ac228" : 89,
    "Th228" : 90,
    "Ra224" : 88,
    "Rn220" : 86,
    "Po216" : 84,
    "Pb212" : 82,
    "Bi212" : 83,
    "Po212" : 84,
    "Tl208" : 81,
}
AtomicMassDict = {
    "Bi214" : 214,
    "Xe137" : 137,
    "Rn222" : 222,
    "Po218" : 218,
    "Pb214" : 214,
    "Bi214" : 214,
    "Po214" : 214,
    "Pb210" : 210,
    "Bi210" : 210,
    "Po210" : 210,
    "Th232" : 232,
    "U238"  : 238,
    "Co60"  : 60,
    "Ra228" : 228,
    "Ac228" : 228,
    "Th228" : 228,
    "Ra224" : 224,
    "Rn220" : 220,
    "Po216" : 216,
    "Pb212" : 212,
    "Bi212" : 212,
    "Po212" : 212,
    "Tl208" : 208,

}
### End Dictionary Region



###### Preprocessing before entering loops
if not os.path.exists(HoldingDir):
    os.makedirs(HoldingDir)
IsotopeListName = ' '.join(IsotopeList)
FullRegionCode = RegionBuilder(RegionName)
### End Preprocessing Region

for JobCounter in range(0, NumJobs):
    ###### Set environment variables on the clusters
    SubmissionScriptContents = ""
    if (UseClusterFlag == 1):
        SubmissionScriptContents = SubmissionScriptContents + """
# Set environment variables for ROOT on cluster
cd /nfs/slac/g/exo/software/ROOT/rhel6/root/5.34.21/
source bin/thisroot.csh

# Set environment variables for Geant4 on cluster
cd /nfs/slac/g/exo/jbalbert/nEXOsim/trunk
source env-rhel6-64.csh

        """
#cd /data/data033/exo/software/geant4/geant4.10.00.p02-nothreaded-install/bin/


    ### End Environment Variables Region

    for IsotopeCounter in range(0, len(IsotopeList)):
        CurrentIsotope = IsotopeList[IsotopeCounter]
        CurrentAtomicNumber = AtomicNumberDict[CurrentIsotope]
        CurrentAtomicMass = AtomicMassDict[CurrentIsotope]

        # change number of decays for branched isotopes
        BranchingFactor = 1.0
        if CurrentIsotope == "Po212":
            BranchingFactor = 0.64
        elif CurrentIsotope == "Tl208":
            BranchingFactor = 0.36
        NumDecaysPerJobwBranch = int(math.floor(NumDecaysPerJob * BranchingFactor))

        ###### Build a string replacement dictionary so that the output execution file
             # can be formatted with variables in an easy-to-read way. This avoids having
             # to keep track of the many %s's in a long variable. There are much better
             # ways to do this in later versions of python
        StringRepDict = {
            "PWD" : PWD,
            "PathTonEXOBuild" : PathTonEXOBuild,
            "PathToClusteringAlgorithm" : PathToClusteringAlgorithm,
            "HoldingDir" : HoldingDir,
            "PreClustRootFileName" : PreClustRootFileName,
            "PostClustRootFileName" : PostClustRootFileName,
            "IsotopeList" : IsotopeList,
            "IsotopeListName" : IsotopeListName,
            "ClusterDiam" : ClusterDiam,
            "CurrentIsotope" : CurrentIsotope,
            "RegionName" : RegionName,
            "CurrentAtomicNumber" : CurrentAtomicNumber,
            "CurrentAtomicMass" : CurrentAtomicMass,
            "FullRegionCode" : FullRegionCode,
            "JobCounter" : JobCounter,
            "RandomSeed" : JobCounter + RandomSeedOffset,
            "NumDecaysPerJobwBranch" : NumDecaysPerJobwBranch,
        }
        ### End String Replacement Dictionary Region

        ###### Build the macro to be submitted to the specified nEXO build
        ExecutionScriptContents = """
/analysis/setOutputROOTFileName %(HoldingDir)s/%(PreClustRootFileName)s_%(CurrentIsotope)s_%(RegionName)s_%(JobCounter)d.root
/analysis/setOutputROOTFileOption recreate

#/nEXO/TPCInternals/internalsFlag 1

##########################
# Detector Construction
#
# Check overlaps during all G4PVPlacements
/nEXO/det/checkOverlap false
#
# Set the max step length in TPCRegion and inner cryostat
#/nEXO/det/stepMax 1 mm
#d
#/nEXO/TPCInternals/tpcLowerRadius 650 mm
/nEXO/TPCVessel/tpcLowerRadius 650 mm
/nEXO/TPCInternals/wwDistance 1259 mm
/nEXO/TPCInternals/fieldRingRef 0.0
/nEXO/TPCInternals/anodeRef 1.0
/nEXO/TPCInternals/cathodeRef 1.0
/nEXO/TPCInternals/bulgeRef 1.0
/nEXO/TPCInternals/activeSiPMPadFrac 1.0
/nEXO/TPCInternals/ringArrangeID 0
/nEXO/TPCInternals/fieldRingLength 10 mm
/nEXO/TPCInternals/fieldRingThick 1 mm
/nEXO/TPCInternals/ringSideDistance 30.85 mm
/nEXO/TPCInternals/radialRingDistance 6 mm

/nEXO/TPCExternals/setVersion 2 # spherical cryostat
###########################

/run/initialize

/run/verbose 0
/event/verbose 0
/tracking/verbose 0
/grdm/verbose 0

#########################
# for debugging
/analysis/printLogicVolumeNames

/generator/setGenerator gps

%(FullRegionCode)s

/gps/energy 0 keV
/gps/ion %(CurrentAtomicNumber)d %(CurrentAtomicMass)d
/grdm/nucleusLimits %(CurrentAtomicMass)d %(CurrentAtomicMass)d %(CurrentAtomicNumber)d %(CurrentAtomicNumber)d

/run/beamOn %(NumDecaysPerJobwBranch)d

        """ % StringRepDict
        # note that the -r option sets the random seed
        
        SubmissionScriptContents = SubmissionScriptContents + """
%(PathTonEXOBuild)s -m %(HoldingDir)s/ExecutionScript_%(CurrentIsotope)s_%(RegionName)s_%(JobCounter)d.mac -r %(RandomSeed)d""" % StringRepDict

        ExecutionFile = open("%(HoldingDir)s/ExecutionScript_%(CurrentIsotope)s_%(RegionName)s_%(JobCounter)d.mac" % StringRepDict,'w')
        ExecutionFile.write(ExecutionScriptContents)
        ExecutionFile.close()
        ### End Macro Building Region

    if (ClusterDepsFlag == 1):
        SubmissionScriptContents = SubmissionScriptContents + """
python -u %(PathToClusteringAlgorithm)s "%(PreClustRootFileName)s" "%(PostClustRootFileName)s" "%(IsotopeListName)s" %(ClusterDiam)d "%(HoldingDir)s" "%(RegionName)s" %(JobCounter)d """ % StringRepDict

    SubmissionFile = open("%(HoldingDir)s/SubmissionScript_%(RegionName)s_%(JobCounter)d.sh" % StringRepDict,'w')
    SubmissionFile.write(SubmissionScriptContents)
    SubmissionFile.close()

    ###### Make submission scripts executable ############
    st = os.stat("%(HoldingDir)s/SubmissionScript_%(RegionName)s_%(JobCounter)d.sh" % StringRepDict)
    os.chmod("%(HoldingDir)s/SubmissionScript_%(RegionName)s_%(JobCounter)d.sh" % StringRepDict, st.st_mode | stat.S_IEXEC)


    outinfo = "-o " + HoldingDir + " -e " + HoldingDir
    os.chdir(HoldingDir)
           
    ###### Submit the submission scripts to the cluster
    if (UseClusterFlag == 1):
        if (1):
#        if (JobCounter == 0):
            os.system("bsub -q xxl -R rhel60 -o $PWD -e $PWD %(HoldingDir)s/SubmissionScript_%(RegionName)s_%(JobCounter)d.sh" % StringRepDict)
        else:
            os.system("bsub -q xxl -R rhel60 -o /dev/null -e /dev/null %(HoldingDir)s/SubmissionScript_%(RegionName)s_%(JobCounter)d.sh" % StringRepDict)
    ### End Submission Region

