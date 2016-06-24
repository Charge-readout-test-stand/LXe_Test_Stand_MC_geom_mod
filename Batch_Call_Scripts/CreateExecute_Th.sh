#!/bin/sh

############################################################################
# The purpose of this script is to send jobs to the batch to produce separate
# files for each daugher in the Th232 decay chain with correct relative 
# branching ratios in the specified components of nEXO.  The number of files 
# and number of decays produced in each file are defined in the 
# 'DecayValues.txt' script.  It creates macro scripts for each file and then 
# executes them.  A different seed is used for each file.



iso="Th232"
# Obviously, make sure the path to the decay values file is correct
. /nfs/slac/g/exo/jbalbert/nEXOsim/trunk/Batch_Call_Scripts/DecayValues.txt

### Directory to dump macro files into ###
MacroDirectory="./launchable_macros/"

#*****
### Name of the macro file ####
### Incrementation and .mac extention are added below ###
MacroFileName="Th-default"

#*****
### Define the path to the nEXO Executable (with executable name) ####
ExecutablePath="/nfs/slac/g/exo/jbalbert/nEXOsim/built/nEXO"

#*****
### Directory where output files should be written ###
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh/"
OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-D-022715/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-C-030715/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-B-031115/"
#OutputDirectory="/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-A-031415/"

#*****
### Define the output root file name
### Incrementation is already included
outputFileName="Th_thickness_output_169_223"

#*****
### List components to simulate the decay chains in
for type in TPC InnerCryo OuterCryo HFE
#TPC InnerCryo OuterCryo HFE ActiveLXe Water # other examples of components

do

echo "$type"
if [ $iso = Th232 ]; then

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
for piece in Tl208

# Some other isotopes you might consider:
#Th232 Ra228 Ac228 Th228 Ra224 Rn220 Po216 Pb212 Bi212 Po212 Tl208

# Pieces with consequential gammas
# Ac228 Po212 Bi212 Tl208

# Pieces with gammas in or above ROI
# Tl208





do
echo "$piece"

if [ $piece = Xe137 ]; then
gpsion="/gps/ion 54 137
/grdm/nucleusLimits 137 137 54 54"
fi

if [ $piece = Th232 ]; then
gpsion="/gps/ion 90 232
/grdm/nucleusLimits 232 232 90 90"
fi

if [ $piece = Ra228 ]; then
gpsion="/gps/ion 88 228
/grdm/nucleusLimits 228 228 88 88"
fi

if [ $piece = Ac228 ]; then
gpsion="/gps/ion 89 228
/grdm/nucleusLimits 228 228 89 89"
fi

if [ $piece = Th228 ]; then
gpsion="/gps/ion 90 228
/grdm/nucleusLimits 228 228 90 90"
fi

if [ $piece = Ra224 ]; then
gpsion="/gps/ion 88 224
/grdm/nucleusLimits 224 224 88 88"
fi

if [ $piece = Rn220 ]; then
gpsion="/gps/ion 86 220
/grdm/nucleusLimits 220 220 86 86"
fi

if [ $piece = Po216 ]; then
gpsion="/gps/ion 84 216
/grdm/nucleusLimits 216 216 84 84"
fi

if [ $piece = Pb212 ]; then
gpsion="/gps/ion 82 212
/grdm/nucleusLimits 212 212 82 82"
fi

if [ $piece = Bi212 ]; then
gpsion="/gps/ion 83 212
/grdm/nucleusLimits 212 212 83 83"
fi

if [ $piece = Po212 ]; then
numdecays=$(($numdecays*64/100))
gpsion="/gps/ion 84 212
/grdm/nucleusLimits 212 212 84 84"
fi

if [ $piece = Tl208 ]; then
numdecays=$(($numdecays*36/100))
gpsion="/gps/ion 81 208
/grdm/nucleusLimits 208 208 81 81"
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






