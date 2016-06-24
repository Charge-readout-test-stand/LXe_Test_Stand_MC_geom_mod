#!/bin/bash

#-------------------------------------------------------------------------------
# options ----
#-------------------------------------------------------------------------------


# define these vars:
# NEXOANALYSIS -- defined in .cshrc


# base output directory 
#export MCBASE="/nfs/slac/g/exo_data4/users/alexis4/test-stand/mc"
export nEXODataDir="/nfs/slac/g/exo_data4/users/alexis4/test-stand/mc"

# nEXO MC build:
export NEXOMCBUILD="$HOME/software/nEXOSW/LXe_Test_Stand_MC_build"

# nEXO MC:
export NEXOMC="$HOME/software/nEXOSW/LXe_Test_Stand_MC"

# charge-readout-scripts
export CHARGEREADOUTSCRIPTS="$HOME/software/charge-readout-scripts/struck/"


TYPE=Bi207_Full_Ralph
#TYPE=e1MeV
#TYPE=g1MeV

# ongoing jobs, d coeff 70:
#DIFF=70 # diff coefficient, after 17 May 2016 discussion w Mike J
#j=100
#last_job=199 # last job

DIFF=51 # diff coefficient, for testing
#DIFF=1 # for testing
j=0
#last_job=$j # run one job
last_job=99 # last job

# FIXME -- fix Bi207_Full_Ralph.in beamon number, fix digi* template to remove
# files & run tier3!!

# ongoing jobs, d coeff 50:
#DIFF=50 # diff coefficient, after 22 Apr 2016 changes to analysis
#j=6500
#last_job=6999 # last job

# one test:
#j=4
#last_job=$j # run one job

#-------------------------------------------------------------------------------

export OUT_DIR=$nEXODataDir/${TYPE}_dcoeff${DIFF}
echo $OUT_DIR

mkdir -p $OUT_DIR/mc
mkdir -p $OUT_DIR/digitization_dcoeff$DIFF
mkdir -p $OUT_DIR/tier3
mkdir -p $OUT_DIR/log
mkdir -p $OUT_DIR/jobs_hold
mkdir -p $OUT_DIR/log

while test $j -le $last_job;
do
  echo $j

  sed -e "s/SEQNUM/$j/g" \
      -e "s/DIFF/$DIFF/g"  \
      -e "s/TYPE/$TYPE/g"  doall.script > $OUT_DIR/jobs_hold/doall${j}.script

  sed -e "s/SEQNUM/$j/g" \
      -e "s/TYPE/$TYPE/g"  $TYPE.in > $OUT_DIR/jobs_hold/run${j}.in

  sed -e "s/SEQNUM/$j/g"  \
      -e "s/TYPE/$TYPE/g"  \
      -e "s/SEED/10$j/g" mc.script > $OUT_DIR/jobs_hold/mc${j}.script
      #-e "s/NEXOMCBUILD/${NEXOMCBUILD}/g"  \
      #-e "s/NEXOMC/${NEXOMC}/g"  \
      #-e "s/OUT_DIR/${OUT_DIR}/g"  \

  sed -e "s/SEQNUM/$j/g"  \
      -e "s/TYPE/$TYPE/g"  \
      -e "s/DIFF/$DIFF/g"  \
      -e "s/SEED/10$j/g" digi.script > $OUT_DIR/jobs_hold/digi${j}.script

  chmod 755 $OUT_DIR/jobs_hold/mc${j}.script
  chmod 755 $OUT_DIR/jobs_hold/doall${j}.script
  chmod 755 $OUT_DIR/jobs_hold/digi${j}.script

  bsub -R rhel60 -J mc$j -oo $OUT_DIR/log/blog_$DIFF.job${j} -q long -W 12:00 $OUT_DIR/jobs_hold/doall${j}.script
  sleep 1.0
  #sleep 2.0
  #sleep 10

  j=`expr $j + 1 `
done


