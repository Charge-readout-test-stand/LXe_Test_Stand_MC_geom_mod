#####################################################################
#  This is a shell script that writes and executes python scripts to 
#  take output from nEXO root files that have already been clustered with 
#  'SubmitClusterScript.sh' and separate them into SS and MS energy and 
#  standoff histograms, akin to what we create with EXO-200.  It assumes
#  that we have u and v wire charge collection.



#!/bin/sh

## Teflon Reflector Inner Radius = 62.80*cm
## APDPlatter Position along TPC axis = 63.12*cm

## Number of files if you need to run over multiple ##
numfiles="1"

#**** Give the name of the python file you would like to execute ****#
#**** Do not include '.py' extention
#**** Same message about incrementation as given below, add incrementation
#**** as " > ${PythonFileName}_${i}.py
PythonFileName=""


#**** Give the input file name ****************#
#**** If you want to loop over multiple files with increment values
#**** in the name, add the increment value below after 'filename'
#**** for example "filename = ${inputfile}_${i}
#**** Do not include the '.root' suffix, that is provided in the script
inputFileName=""

#**** Give the output file name (that will hold the histograms) *****#
#**** Same message about incrementation given above 
#**** Add as savefilename = ${outputFileName}_${i}.root
outputFileName=""

#**** List components to run over *******#
for Comp in TPC FieldSupport KCables_L KCables_R TReflector TeflonInsulator ActiveLXe InactiveLXe Cathode

#HFE InnerCryo OuterCryo
#OuterWater1 OuterWater2 HDPETank InnerWater1 InnerWater2 Shotcrete ShotDome FLOOR
do

#******** List parent isotopes to run over ************#
for Par in Th232 U238
do

#******** List V threshold in keV (250 for EXO-200 like) ************#
for vThreshkeV in 250.
do

#******** List U threshold in keV (75 for EXO-200 like) ************#
for uThreshkeV in 75.
do

#******** List cluster diameter, only for filename ***********#
for diam in 6
do

#*********** cut from the APD Platter **************#
# any event with a cluster greater than this in z(x) #
# (616 is 15 mm from APD Platter)#
for fidcutaxis in 616
do

#*********** Radial cut from the TReflector **************#
for fidcutrad in 628  
do

for(( fileNum = 1; fileNum <= ${numfiles}; fileNum++))
do

echo "

import ROOT, math, array
from ROOT import *
from array import array
from math import *

#~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~#
# Script to calculate the SS/MS fraction for a certain cluster Diamter
#~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##~*~##


Component = '${Comp}'
Diameter = '${diam}'
Isotope = '${Par}'
FileNum='${fileNum}'
#Half Axial Fiducial Cut in mm
HalfAxisFidCut = ${fidcutaxis}.
RadFidCut = ${fidcutrad}
uThresh = ${uThreshkeV}
vThresh = ${vThreshkeV}
date = '${date}'

ran = TRandom3()

SSEnergySaveName = 'Energy_%smmSSClusters_%s_10^7DecaysTPC_zL%i_rL%i_%suThresh_%ivThresh'% (Diameter, Isotope, HalfAxisFidCut, RadFidCut,uThresh, vThresh)
SSEnergyTitle = 'Energy of %s mm SS Clusters from %s in TPC, 10^7 initial decays, |Z| < %i, r < %i, 75uThresh, %fvThresh' % (Diameter,Isotope, HalfAxisFidCut, RadFidCut, vThresh)    
h_SSEnergy = TH1D('h_SSEnergy', SSEnergyTitle, 140, 700, 3500)
h_SSEnergy.GetXaxis().SetTitle('Energy (keV)')

MSEnergySaveName = 'Energy_%smmMSClusters_%s_10^7DecaysTPC_zL%i_rL%i_%suThresh_%ivThresh' % (Diameter, Isotope, HalfAxisFidCut, RadFidCut, uThresh, vThresh)
MSEnergyTitle = 'Energy of %s mm MS Clusters from %s in TPC, 10^7 initial decays, |Z| < %i, r < %i, 75uThresh, %fvThresh' % (Diameter,Isotope, HalfAxisFidCut, RadFidCut, vThresh)    
h_MSEnergy = TH1D('h_MSEnergy', MSEnergyTitle, 140, 700, 3500)
h_MSEnergy.GetXaxis().SetTitle('Energy (keV)')

SSEnergy_SmearSaveName = 'Energy_%smmSSClusters_%s_10^7DecaysTPC_zL%i_rL%i_Smear_%suThresh_%ivThresh' % (Diameter, Isotope, HalfAxisFidCut, RadFidCut, uThresh, vThresh)
SSEnergyTitle_Smear = 'Smeared Energy of %s mm SS Clusters from %s in TPC, 10^7 initial decays, |Z| < %i, r < %i, 75uThresh, %fvThresh' % (Diameter,Isotope, HalfAxisFidCut, RadFidCut, vThresh)    
h_SSEnergy_Smear = TH1D('h_SSEnergy_Smear', SSEnergyTitle_Smear, 140, 700, 3500)
h_SSEnergy_Smear.GetXaxis().SetTitle('Energy (keV)')

MSEnergy_SmearSaveName = 'Energy_%smmMSClusters_%s_10^7DecaysTPC_zL%i_rL%i_Smear_%suThresh_%ivThresh' % (Diameter, Isotope, HalfAxisFidCut, RadFidCut, uThresh, vThresh)
MSEnergyTitle_Smear = 'Smeared Energy of %s mm MS Clusters from %s in TPC, 10^7 initial decays, |Z| < %i, r < %i, 75uThresh, %fvThresh' % (Diameter,Isotope, HalfAxisFidCut, RadFidCut, vThresh)    
h_MSEnergy_Smear = TH1D('h_MSEnergy_Smear', MSEnergyTitle_Smear, 140, 700, 3500)
h_MSEnergy_Smear.GetXaxis().SetTitle('Energy (keV)')

h_StandoffDistSS = TH1D('h_StandoffDistSS', 'Standoff Distance of SS Clusters in TPC', 65, 0, 650)
h_StandoffDistSS.GetXaxis().SetTitle('Standoff Distance (mm)')

h_StandoffDistMS = TH1D('h_StandoffDistMS', 'Standoff Distance of MS Clusters in TPC', 65, 0, 650)
h_StandoffDistMS.GetXaxis().SetTitle('Standoff Distance (mm)')

h_StandoffVsEnergySS = TH2D('h_StandoffVsEnergySS', 'Standoff Distance in TPC vs. Energy of Event, SS Clusters', 140, 700, 3500, 65, 0, 650)
h_StandoffVsEnergySS.GetXaxis().SetTitle('Energy (keV)')
h_StandoffVsEnergySS.GetYaxis().SetTitle('Standoff Distance (mm)')

h_StandoffVsEnergyMS = TH2D('h_StandoffVsEnergyMS', 'Standoff Distance in TPC vs. Energy of Event, MS Clusters', 140, 700, 3500, 65, 0, 650)
h_StandoffVsEnergyMS.GetXaxis().SetTitle('Energy (keV)')
h_StandoffVsEnergyMS.GetYaxis().SetTitle('Standoff Distance (mm)')

h_StandoffVsEnergySS_Smear = TH2D('h_StandoffVsEnergySS_Smear', 'Standoff Distance in TPC vs. Smeared Energy of Event, SS Clusters', 140, 700, 3500, 65, 0, 650)
h_StandoffVsEnergySS_Smear.GetXaxis().SetTitle('Energy (keV)')
h_StandoffVsEnergySS_Smear.GetYaxis().SetTitle('Standoff Distance (mm)')

h_StandoffVsEnergyMS_Smear = TH2D('h_StandoffVsEnergyMS_Smear', 'Standoff Distance in TPC vs. Smeared Energy of Event, MS Clusters', 140, 700, 3500, 65, 0, 650)
h_StandoffVsEnergyMS_Smear.GetXaxis().SetTitle('Energy (keV)')
h_StandoffVsEnergyMS_Smear.GetYaxis().SetTitle('Standoff Distance (mm)')

#************ Scale Factor for reduced resolution ************#
scalefactor = 1.
#scalefactor = 1.5/1.67

p0SS = -2.244e-6
p1SS = 36.9
p2SS = 8.792e-3

p0MS = -3.676e-6
p1MS = 41.08
p2MS = 9.611e-3

def IsFiducialSS():
    x = t.Xpos[0]
    y = t.Ypos[0]
    z = t.Zpos[0]
    rad = math.sqrt(y*y+z*z)
    ExitStatus = 0  # Variable for keeping track of how the event was trashed
    energy = t.EnergyDeposit[0]*1000.

    if energy > uThresh:
        if abs(x) > HalfAxisFidCut or rad > RadFidCut:
            ExitStatus = 1
            return ExitStatus
    if energy < vThresh:
        ExitStatus = 2
        return ExitStatus
    standoffz = 631. - abs(x)
    standoffr = 628. - rad
    if standoffz < standoffr:
        standoff = standoffz
    else:
        standoff = standoffr
    #print 'standoffz = ', standoffz
    #print 'standoffr = ', standoffr
    #print 'standoff = ', standoff
    
    h_StandoffDistSS.Fill(standoff)

    h_SSEnergy.Fill(energy)
    sigma = sqrt(p0SS*p0SS*energy + p1SS*p1SS + p2SS*p2SS*energy*energy)*scalefactor
    ESmeared = energy + ran.Gaus(0.0, sigma)
    h_StandoffVsEnergySS.Fill(energy, standoff)
    h_StandoffVsEnergySS_Smear.Fill(ESmeared, standoff)

    return ExitStatus

def IsFiducialMS(nClus):
    clusCount = 0
    TotalEnergy = 0
    ExitStatus = 0
    standoff = 0
    standoffclus = 0

    for clus in range(0,nClus):
        x = t.Xpos[clus]
        y = t.Ypos[clus]
        z = t.Zpos[clus]
        rad = math.sqrt(y*y+z*z)
        energy = t.EnergyDeposit[clus]*1000.
        
        if energy > uThresh:
            #if cluster is above threshold and non-fiducial, throw
            #out entire event
            if abs(x) > HalfAxisFidCut or rad > RadFidCut:
                ExitStatus = 1
                return ExitStatus
            if energy < vThresh:
                ExitStatus = 2
                return ExitStatus
            else:
                standoffz = 631. - abs(x)
                standoffr = 628. - rad
                if standoffz < standoffr:
                    standoffclus = standoffz
                else:
                    standoffclus = standoffr
                #print 'cluster ', clus
                #print 'standoffz = ', standoffz
                #print 'standoffr = ', standoffr
                if standoffclus < standoff or standoff == 0:
                    standoff = standoffclus
                TotalEnergy += energy
                clusCount += 1
        else:
            continue
    #print 'finalStandoff = ', standoff
    if clusCount == 0:
	return 3
    if clusCount == 1:
        h_StandoffDistSS.Fill(standoff)

        sigma = sqrt(p0SS*p0SS*TotalEnergy + p1SS*p1SS + p2SS*p2SS*TotalEnergy*TotalEnergy)*scalefactor
        ESmeared = TotalEnergy + ran.Gaus(0.0, sigma)
        h_StandoffVsEnergySS.Fill(TotalEnergy, standoff)
        h_StandoffVsEnergySS_Smear.Fill(ESmeared, standoff)
        h_SSEnergy.Fill(TotalEnergy)
    if clusCount > 1:
        h_StandoffDistMS.Fill(standoff)

        sigma = sqrt(p0MS*p0MS*TotalEnergy + p1MS*p1MS + p2MS*p2MS*TotalEnergy*TotalEnergy)*scalefactor
        ESmeared = TotalEnergy + ran.Gaus(0.0, sigma)
        h_StandoffVsEnergyMS.Fill(TotalEnergy, standoff)
        h_StandoffVsEnergyMS_Smear.Fill(ESmeared, standoff)
        h_MSEnergy.Fill(TotalEnergy)
    return




filename = '${inputFileName}.root'


f = TFile(filename)

t = f.Get('clusterTree')

n = t.GetEntries()
print 'Number entries = ', n

CountLowU = 0
TrashcountSSVwire = 0
TrashcountSSFidcut = 0
TrashcountMSVwire = 0
TrashcountMSFidcut = 0

##### Actual code starts here, run through clusters
for i in range(0,n):
    if i % 10000 == 0:
        print 'i = ', i
    t.GetEntry(i)

    nClus = t.NumClusters
    if nClus == 1:
        #print 'SS'
        ExitStatus =  IsFiducialSS()
        #print '\n'
        if ExitStatus == 1:
            TrashcountSSFidcut += 1
        if ExitStatus == 2:
            TrashcountSSVwire += 1        
	if ExitStatus == 3:
	    CountLowU += 1

    if nClus > 1:
        #print 'MS'
        ExitStatus = IsFiducialMS(nClus)
        #print '\n'
        if ExitStatus == 1:
            TrashcountMSFidcut += 1
        if ExitStatus == 2:
            TrashcountMSVwire += 1     
    	if ExitStatus == 3:
	    CountLowU += 1


# Now smear graphs with energy resolution
histLenSS = h_SSEnergy.GetNbinsX()
p0 = -2.244e-6
p1 = 36.9
p2 = 8.792e-3
for k in range(0,histLenSS):
    val = int(h_SSEnergy.GetBinContent(k+1))
    for l in range(0,val):
        E_center = h_SSEnergy.GetXaxis().GetBinCenter(k+1)
        E_centerkeV = E_center
        sigma = sqrt(p0*p0*E_centerkeV + p1*p1 + p2*p2*E_centerkeV*E_centerkeV)*scalefactor
        h_SSEnergy_Smear.Fill(E_center + ran.Gaus(0.0, sigma))


histLenMS = h_MSEnergy.GetNbinsX()
p0 = -3.676e-6
p1 = 41.08
p2 = 9.611e-3
for k in range(0,histLenMS):
    val = int(h_MSEnergy.GetBinContent(k+1))
    for l in range(0,val):
        E_center = h_MSEnergy.GetXaxis().GetBinCenter(k+1)
        E_centerkeV = E_center
        sigma = sqrt(p0*p0*E_centerkeV + p1*p1 + p2*p2*E_centerkeV*E_centerkeV)*scalefactor
        h_MSEnergy_Smear.Fill(E_center + ran.Gaus(0.0, sigma))

savefilename = '${outputFileName}.root'
savefile = TFile(savefilename, 'RECREATE')


h_SSEnergy.Write()
h_MSEnergy.Write()
h_SSEnergy_Smear.Write()
h_MSEnergy_Smear.Write()

h_StandoffDistSS.Write()
h_StandoffDistMS.Write()

h_StandoffVsEnergySS.Write()
h_StandoffVsEnergyMS.Write()

h_StandoffVsEnergySS_Smear.Write()
h_StandoffVsEnergyMS_Smear.Write()


savefile.Close()



'''
writefile = open('PythonScripts_6_30_13/nEXO_Histograms_%s_%s_%smm_%svThresh_%iby%iFidCut.txt' %(Component,Isotope, Diameter,vThresh,HalfAxisFidCut, RadFidCut), 'w')


c1 = TCanvas('c1', 'c1')
h_SSEnergy.Draw()
c1.SaveAs('Histos_TPC_Threshold_FidCut/' + SSEnergySaveName + '.png')
c1.SetLogy()
h_SSEnergy.Draw()
c1.SaveAs('Histos_TPC_Threshold_FidCut/' + SSEnergySaveName + '_Log.png')

c2 = TCanvas('c2', 'c2')
h_MSEnergy.Draw()
c2.SaveAs('Histos_TPC_Threshold_FidCut/' + MSEnergySaveName + '.png')
c2.SetLogy()
h_MSEnergy.Draw()
c2.SaveAs('Histos_TPC_Threshold_FidCut/' + MSEnergySaveName + '_Log.png')

c3 = TCanvas('c3', 'c3')
h_SSEnergy_Smear.Draw()
c3.SaveAs('Histos_TPC_Threshold_FidCut/' + SSEnergy_SmearSaveName + '.png')
c3.SetLogy()
h_SSEnergy_Smear.Draw()
c3.SaveAs('Histos_TPC_Threshold_FidCut/' + SSEnergy_SmearSaveName + '_Log.png')

c4 = TCanvas('c4', 'c4')
h_MSEnergy_Smear.Draw()
c4.SaveAs('Histos_TPC_Threshold_FidCut/' + MSEnergy_SmearSaveName + '.png')
c4.SetLogy()
h_MSEnergy_Smear.Draw()
c4.SaveAs('Histos_TPC_Threshold_FidCut/' + MSEnergy_SmearSaveName + '_Log.png')
'''

print 'CountLowU = %i' % CountLowU 
#s1 = 
print 'TrashcountSSVwire = %i' % TrashcountSSVwire
#s2 = 
print 'TrashcountMSVwire = %i' % TrashcountMSVwire

#s3 =  
print 'TrashcountSSFidcut = %i' % TrashcountSSFidcut
#s4 =  
print 'TrashcountMSFidcut = %i' % TrashcountMSFidcut 

TCSSV = TrashcountSSVwire/n
TCMSV = TrashcountMSVwire/n
TCSSF = TrashcountSSFidcut/n
TCMSF = TrashcountMSFidcut/n 

#s5 =  
print 'TrashcountSSVwire Efficiency = %s' % str(TCSSV) 
#s6 =  
print 'TrashcountMSVwire Efficiency = %s' % str(TCMSV)

#s7 = 
print 'TrashcountSSFidcut Efficiency = %s' % str(TCSSF)
#s8 = 
print 'TrashcountMSFidcut Efficiency = %s' % str(TCMSF)



'''
writefile.write(s1)
writefile.write(s2)
writefile.write(s3)
writefile.write(s4)
writefile.write(s5)
writefile.write(s6)
writefile.write(s7)
writefile.write(s8)

writefile.close()
'''
" > ${PythonFileName}.py

bsub -q long wrap_python.sh ${PythonFileName}.py

done
done
done
done
done
done
done
done