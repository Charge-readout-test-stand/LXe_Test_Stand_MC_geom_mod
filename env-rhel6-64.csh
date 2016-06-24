setenv CURRENT $PWD
setenv version1 geant4.10.00.p03
setenv version2 Geant4-10.0.3
setenv linux rhel6-64

setenv g4script /afs/slac.stanford.edu/package/geant4/vol51/$version1/$linux/share/$version2/geant4make
cd $g4script; source $g4script/geant4make.csh $g4script
cd $CURRENT
unset g4script

setenv Geant4_DIR /afs/slac.stanford.edu/package/geant4/vol51/$version1/$linux/lib64/$version2
unsetenv G4VIS_USE_OIX

if ( ! ${?LD_LIBRARY_PATH} ) then
  setenv LD_LIBRARY_PATH "/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib"
else
  setenv LD_LIBRARY_PATH "/afs/slac.stanford.edu/package/geant4/vol52/xercesc/xerces-c-3.1.1/$linux/lib:/afs/slac.stanford.edu/package/geant4/vol52/coin/Coin-3.1.3/$linux/lib":${LD_LIBRARY_PATH}
endif
