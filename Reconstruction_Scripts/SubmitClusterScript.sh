################################################################
# The purpose of this script is to take output from nEXO Monte
# Carlo and cluster the raw energy according to some specified 
# Diameter.  The output can be fed into SubmitHistoCreation.sh
# to create SS/MS Histograms.  It creates python files that are 
# then executed.


#!/bin/sh


#### List the number of files to run this script over #######
numfiles="1"

##### List the diameter you would like to cluster at ########
diam="6"



#############################################################
# This next block of code is only to define the names of the
# input and output files.
#############################################################

#### List the parent isotope ####
for Par in Th232 U238 Co60
do

#### Components you would like to run over ######
for Comp in bb0n TPC FieldSupport KCables_L KCables_R TReflector TeflonInsulator APDPlatter ActiveLXe InactiveLXe Cathode
#InnerCryo OuterCryo HFE
#ShotDome Shotcrete FLOOR
#ActiveLXe InactiveLXe
do

##### Name of output root file (with clustered energy deposits) #####
outputFileName = ""

#### Input file name - change to your own input file names around lines
#### 158 -> 179 of this shell script

#### Name of python file that will be created and executed
PythonFileName=""



for((fileNum = 1; fileNum <= ${numfiles}; fileNum++))

do

echo "
import ROOT, math, array
from ROOT import *
from array import array

print 'in Python Script'

fileNum = ${fileNum}
ClusterDiam = ${diam}
Component = '${Comp}'
Parent = '${Par}'


#**** Lists the separate isotopes in a decay chain ******#
if Parent == 'Th232':
    Pieces = ['Ac228', 'Ra224','Bi212', 'Tl208']
if Parent == 'U238':
    Pieces = ['Th234', 'Pa234','Ra226','Pb214', 'Bi214','Pb210']
if Parent == 'Co60':
    Pieces = ['None']
if Parent == 'Xe137':
    Pieces = ['Xe137']

date = '${date}'

#***** Output ROOT file *****#

rootFileName = ${outputFileName}.root

rootFile = TFile(rootFileName, 'RECREATE')

clusterTree = TTree('clusterTree', 'Tree of Clustered Energy Deposits in LXe')


#********* Define the array for clustered deposits ************#
EventNumber = array('i',[0])
TotalEventEnergy = array('d',[0])
NumClusters = array('i',[0])
EnergyDeposit = array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
Xpos = array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
Ypos = array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
Zpos = array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])

clusterTree.Branch('EventNumber', EventNumber, 'EventNumber/I')
clusterTree.Branch('TotalEventEnergy', TotalEventEnergy, 'TotalEventEnergy/D')
clusterTree.Branch('NumClusters', NumClusters, 'NumClusters/I')
clusterTree.Branch('EnergyDeposit', EnergyDeposit, 'EnergyDeposit[40]/D')
clusterTree.Branch('Xpos', Xpos, 'Xpos[40]/D')
clusterTree.Branch('Ypos', Ypos, 'Ypos[40]/D')
clusterTree.Branch('Zpos', Zpos, 'Zpos[40]/D') 


def PerformClusteringOnList(Deps, numClust):

    if len(Deps) < 2:
        PopArray = Deps.pop(0)
        Xpos[numClust] = PopArray[0]
        Ypos[numClust] = PopArray[1]
        Zpos[numClust] = PopArray[2]
        EnergyDeposit[numClust] = PopArray[3]
        numClust += 1

        NumClusters[0] = numClust
        clusterTree.Fill()

        return
    
    x1 = Deps[0][0]
    y1 = Deps[0][1]
    z1 = Deps[0][2]
    for element in range(1,len(Deps)):
        x2 = Deps[element][0]
        y2 = Deps[element][1]
        z2 = Deps[element][2]
        
        d = math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))
        if d < ClusterDiam:
            e1 = Deps[0][3]
            e2 = Deps[element][3]
            eNew = e1 + e2
            xNew = (e1*x1 + e2*x2)/eNew
            yNew = (e1*y1 + e2*y2)/eNew
            zNew = (e1*z1 + e2*z2)/eNew
            NewDep = [xNew, yNew, zNew, eNew]
            
            Deps.pop(element)
            Deps.pop(0)
            Deps.insert(0,NewDep)

            PerformClusteringOnList(Deps, numClust)
            return

    PopArray = Deps.pop(0)
    
    Xpos[numClust] = PopArray[0]
    Ypos[numClust] = PopArray[1]
    Zpos[numClust] = PopArray[2]
    EnergyDeposit[numClust] = PopArray[3]
    numClust += 1
    
    PerformClusteringOnList(Deps, numClust)
    


#***Add together all isotopes in decay chain and create a TChain***#
t = TChain('tree')

#*** Use this is the source was a decay chain *****#
#*** Change file names to paths of your own root files
#'''
for piece in Pieces:
#    print 'num = ', num
    if Parent == 'Th232' or 'U238':
        filename = '/nfs/slac/g/exo/user-data/tesjohns/TPC_Components/%s-%s/%s-%s-%s-%s-%s.root' % (Component, date, Component, Parent, piece, date, str(fileNum))
    if Parent == 'Co60':
        filename = '/nfs/slac/g/exo/user-data/tesjohns/TPC_Components/%s-%s/%s-%s-%s-%s.root' % (Component, date, Component, Parent, date, str(fileNum))

    if Parent == 'Xe137':
        filename = '/nfs/slac/g/exo/user-data/tesjohns/TPC_Components/%s-%s/%s--%s-%s-%s.root' % (Component, date, Component, Parent, date, str(fileNum))
    t.Add(filename)
#'''

#****** If the source was not a decay chain ******#
#****** Change to your own input file name *******#
'''
filename = '/nfs/slac/g/exo/user-data/tesjohns/bb0n/bb0n-${date}-%s.root' % str(fileNum)
print 'Adding file ', filename
t.Add(filename)
''''

n = t.GetEntries()
print 'nfinal = ', n


for i in range(0, n):
    if i%10000 == 0:
        print 'i = ', i
    t.GetEntry(i)
    arrayNum = t.NumDeposits

    EventNumber[0] = 0
    TotalEventEnergy[0] = 0.
    NumClusters[0] = 0
    for j in range(0,40):
        EnergyDeposit[j] = 0.
        Xpos[j] = 0.
        Ypos[j] = 0.
        Zpos[j] = 0.


    EventNumber[0] = t.EventNumber
    TotalEventEnergy[0] = t.TotalEventEnergy
    numClust = 0

    Deps = []

    for j in range(0,arrayNum):
        Dep = [t.Xpos[j], t.Ypos[j], t.Zpos[j], t.EnergyDeposit[j]]
        Deps.append(Dep)

    PerformClusteringOnList(Deps, 0)

print 'right before writing'    
clusterTree.Write()

rootFile.Close()


"  > ${PythonFileName}.py


bsub -q xxl wrap_python.sh ${PythonFileName}.py


done
done
done
