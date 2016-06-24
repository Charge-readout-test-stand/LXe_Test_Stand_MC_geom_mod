#!/bin/sh

############################################################################
# The purpose of this script is to send jobs to the batch to produce separate
# files for each daugher in the U238 decay chain with correct relative 
# branching ratios in the specified components of nEXO.  The number of files 
# and number of decays produced in each file are defined in the 
# 'DecayValues.txt' script.  It creates macro scripts for each file and then 
# executes them.  A different seed is used for each file.


iso="U238"

# Obviously, make sure the path to the decay values file is correct
. /nfs/slac/g/exo/jbalbert/nEXOsim/trunk/Batch_Call_Scripts/DecayValues.txt

### Directory to dump macro files into ###
MacroDirectory="./launchable_macros/"

#*****
### Name of the macro file ####
### Incrementation and .mac extention are added below ###
MacroFileName="U-default-d"

#*****
### Define the path to the nEXO Executable (with executable name) ####
ExecutablePath="/nfs/slac/g/exo/jbalbert/nEXOsim/built/nEXO"

#*****
### Directory where output files should be written ###
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh/"
OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-D-030215/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-C-030715/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-B-031115/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-A-031415/"

#*****
### Define the output root file name
### Incrementation is already included
#outputFileName="U_thickness_output_131_186"
#outputFileName="U_thickness_output_151_206"
#outputFileName="U_thickness_output_181_236"
outputFileName="U_thickness_output_169_223"

#*****
### List components to simulate the decay chains in
#for type in TPC InnerCryo OuterCryo HFE
for type in HFE
#TPC InnerCryo OuterCryo HFE ActiveLXe Water # other examples of components
do

echo "$type"
if [ $iso = U238 ]; then

numdecays="numdecays_$type"
eval numdecays=\$$numdecays
factor="factor_$type"
eval factor=\$$factor
echo "$factor"

# Go through and convert short names into g4 sim logical volume names
# also, set confinement regions so the sim doesn't give up on finding
# an event in the confinement region
# If the geometry is significantly altered, we may need to change the
# confinement dimensions
# Changing the cryostat geometry setting will necessitate changing
# the names here... it is not optimal.
if [ $type = TPC ]; then
typename="/nEXO/TPCVessel"
confinedim="150 cm"
fi
if [ $type = InnerCryo ]; then
typename="/nEXO/TPCExternals/InnerSphericalCryostat"
confinedim="500 cm"
fi
if [ $type = OuterCryo ]; then
typename="/nEXO/TPCExternals/OuterSphericalCryostat"
confinedim="500 cm"
fi
if [ $type = HFE ]; then
typename="/nEXO/TPCExternals/InnerSphericalCryostat/HFE"
confinedim="500 cm"
fi
if [ $type = ActiveLXe ]; then
typename="/nEXO/TPCInternals/LXe/ActiveRegion"
confinedim="150 cm"
fi
if [ $type = Water ]; then
typename="/nEXO/TPCExternals/WaterShieldTank/Water"
confinedim="2000 cm"
fi
# This list can probably be extended, do so as you need
# I make no guarantees for the accuracy or up-to-date-ness 
# of this, carefully check that decays are being simulated where you expect!
# an improper decay name will not cause a crash!

#*****
## List the specific isotopes you want to simulate
for piece in Bi214

# Some other isotopes you might consider:
#U238 Th234 Pa234 U234 Th230 Ra226 Rn222 Po218 Pb214 Bi214 Po214 Pb210 Bi210 Po210


# Pieces with consequential emitted gammas
# Pa234 Pb214 Bi214 

# Pieces with gammas in or above the ROI
# Bi214

do
echo "$piece"

if [ $piece = U238 ]; then
gpsion="/gps/ion 92 238
/grdm/nucleusLimits 238 238 92 92"
fi

if [ $piece = Th234 ]; then
gpsion="/gps/ion 90 234 234
/grdm/nucleusLimits 234 234 90 90"
fi

if [ $piece = Pa234 ]; then
gpsion="/gps/ion 91 234 0 73.92
/grdm/nucleusLimits 234 234 91 91"
fi

if [ $piece = U234 ]; then
gpsion="/gps/ion 92 234
/grdm/nucleusLimits 234 234 92 92"
fi

if [ $piece = Th230 ]; then
gpsion="/gps/ion 90 230
/grdm/nucleusLimits 230 230 90 90"
fi

if [ $piece = Ra226 ]; then
gpsion="/gps/ion 88 226
/grdm/nucleusLimits 226 226 88 88"
fi

if [ $piece = Rn222 ]; then
gpsion="/gps/ion 86 222
/grdm/nucleusLimits 222 222 86 86"
fi

if [ $piece = Po218 ]; then
gpsion="/gps/ion 84 218
/grdm/nucleusLimits 218 218 84 84"
fi

if [ $piece = Pb214 ]; then
gpsion="/gps/ion 82 214
/grdm/nucleusLimits 214 214 82 82"
fi

if [ $piece = Bi214 ]; then
gpsion="/gps/ion 83 214
/grdm/nucleusLimits 214 214 83 83"
fi

if [ $piece = Po214 ]; then
gpsion="/gps/ion 84 214
/grdm/nucleusLimits 214 214 84 84"
fi

if [ $piece = Pb210 ]; then
gpsion="/gps/ion 82 210
/grdm/nucleusLimits 210 210 82 82"
fi

if [ $piece = Bi210 ]; then
gpsion="/gps/ion 83 210
/grdm/nucleusLimits 210 210 83 83"
fi

if [ $piece = Po210 ]; then
gpsion="/gps/ion 84 210
/grdm/nucleusLimits 210 210 84 84"
fi


for((i = 1; i <= $factor; i++))
do
namesuffix="${i}_${piece}_${type}"

echo "
##############################################################################
#           Event Generator for $piece in the $type
#
##############################################################################

#/rootfile/fileName ${OutputDirectory}/${outputFileName}_${namesuffix}.root

#/run/initialize

/control/verbose 0
/run/verbose 0
/event/verbose 0
/tracking/verbose 0
#/grdm/verbose 0

##########################
# Detector Construction
#
# Check overlaps during all G4PVPlacements
/nEXO/det/checkOverlap false
#
# Set the max step length in TPCRegion and inner cryostat
#/nEXO/det/stepMax 1 mm
#
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

# Here is where one could change the cryostat details
/nEXO/TPCExternals/setVersion 2 # spherical cryostat
/nEXO/TPCExternals/OuterSphericalCryostat/setRadius 2230.0 mm
/nEXO/TPCExternals/InnerSphericalCryostat/setRadius 1690.0 mm
###########################

##########################
# Choose PrimaryGenerator
#
#/generator/setGenerator gun
/generator/setGenerator gps

##########################
# Initialisation procedure
#
/run/initialize

##########################
# Set root output file options
/analysis/setOutputROOTFileName ${OutputDirectory}/${outputFileName}_${namesuffix}.root
/analysis/setOutputROOTFileOption recreate
/analysis/setOutputROOTTreeName nEXOevents

#########################
# for debugging
/analysis/printLogicVolumeNames

/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz $confinedim
/gps/pos/radius $confinedim
/gps/pos/confine $typename
/gps/ang/type iso
/gps/energy 0 keV
/gps/particle ion
$gpsion

/run/beamOn $numdecays

"  > ${MacroDirectory}/${MacroFileName}_${namesuffix}.mac

# Note that the -r on the end sets the random seed
# Here we submit to queue.  Modify if submitting somewhere other than SLAC
bsub -q xlong -R rhel60 ${ExecutablePath} -m ${MacroDirectory}/${MacroFileName}_${namesuffix}.mac -r $i
done


done

else
echo "Not the right isotope."
fi

done






