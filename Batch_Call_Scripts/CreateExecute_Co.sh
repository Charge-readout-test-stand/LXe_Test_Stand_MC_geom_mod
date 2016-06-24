#!/bin/sh

############################################################################
# The purpose of this script is to send jobs to the batch to produce Co60
# Decays in the specified components of nEXO.  The number of files and number
# of decays produced in each file are defined in the 'DecayValues.txt' script.
# It creates python scripts for each file and then executes them.  A different 
# seed is used for each file.



### Name of the python file ####
### Incrementation and .py extention are added below ###
PythonFileName=""

### Define the path to the nEXO Executable ####
ExecutablePath=""


iso="Co60"
. Batch_Call_Scripts/DecayValues.txt



### List components to simulate Co60 decay in 
for type in InnerCryo OuterCryo
#TPC FieldRing TReflector APDPlatter KCables TeflonInsulator FieldSupport

do 


if [ $iso = Co60 ]; then
gpsion="/gps/ion 27 60 0 0"
numdecays="numdecays_$type"

factor="factor_$type"
eval factor=\$$factor
eval numdecays=\$$numdecays
echo "$factor"

echo "numdecays = $numdecays"

fi




for(( i = 1; i<=$factor; i++))

do
echo "
##############################################################################
#           Event Generator for $iso in the $type
#
##############################################################################

/rootfile/fileName ${outputFileName}_$i

/run/initialize
/run/verbose 0
/event/verbose 0
/tracking/verbose 0
/grdm/verbose 0

/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 200 cm
/gps/pos/radius 200 cm
/gps/pos/confine $type
/gps/energy 0 keV
/gps/particle ion
$gpsion

/run/beamOn $numdecays

"  > ${PythonFileName}_${i}.py

bsub -q xxl ${ExecutablePath} -m ${PythonFileName}_${i}.py -r $i
done



done
