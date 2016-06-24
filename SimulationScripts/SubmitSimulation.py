###############################################################################
#
#  SubmitSimulation.py
#  Submit simulations of the nEXO detector
# 
###############################################################################

import sys, os, stat, time
from Card import Card
import SimUtils, MacroBuildUtils
from optparse import OptionParser

def ReadCard(filename):
    return Card(filename)

def CreateJobs(card):

    jobList = []

    runSimulation = bool(int(card.Get('RunSimulation')))
    runClustering = bool(int(card.Get('RunClustering')))
    runReconstruction = bool(int(card.Get('RunReconstruction')))

    infoList = ['jobs']
    if runSimulation: infoList += ['g4']
    if runClustering: infoList += ['cluster','g4']
    if runReconstruction: infoList += ['recon','cluster']
    
    filesInfoList = SimUtils.GetFilesInfo(card,infoList)

    for fileInfo in filesInfoList:
        jobDir = '/'.join(fileInfo['JobFileName'].split('/')[:-1])
        if not os.path.isdir(jobDir):
            cmd = 'mkdir -p ' + jobDir
            print 'Making directory for job scripts:', cmd
            os.system(cmd)

        jobName = fileInfo['JobFileName']
        jobFile = open(jobName,'w')
        if card.Has('JobHeader'):
            jobFile.write(card.Get('JobHeader'))
        jobFile.write('source %s \n\n'%(card.Get('SetupFile')))
        jobFile.write('cd %s \n\n'%os.getcwd())
            
        seed = fileInfo['Seed']

        # Create output directories
        if runSimulation:
            macroDir = '/'.join(fileInfo['MacroFileName'].split('/')[:-1])
            if not os.path.isdir(macroDir):
                cmd = 'mkdir -p ' + macroDir
                print 'Making directory for G4 macros:', cmd
                os.system(cmd)
            jobFile.write('mkdir -p ' + '/'.join(fileInfo['G4FileName'].split('/')[:-1]) + '\n')
        if runClustering:
            jobFile.write('mkdir -p ' + '/'.join(fileInfo['ClusterFileName'].split('/')[:-1]) + '\n')
        if runReconstruction:
            jobFile.write('mkdir -p ' + '/'.join(fileInfo['ReconFileName'].split('/')[:-1]) + '\n')
        jobFile.write('\n')

        # If scratch directory is used:
        if 'ScratchDir' in fileInfo.keys():

            # Create scratch folder
            jobFile.write('mkdir -p ' + fileInfo['ScratchDir'] + '\n\n')

            hasBinDir = card.Has('BinTopDir')
            if runSimulation:
                MacroBuildUtils.CreateMacro(fileInfo, card)
    
                # Note: Macros are not saved in scratch
                macroName = fileInfo['MacroFileName']
                simName = fileInfo['ScratchDir'] + '/' + fileInfo['G4FileName'].split('/')[-1]
                
                screenOut = simName.replace('.root','.out')
                screenErr = simName.replace('.root','.err')
    
                nEXOsimPath = card.Get('FullnEXOSimPath')
                if hasBinDir: nEXOsimPath = nEXOsimPath.replace('[BinTopDir]',card.Get('BinTopDir'))               
                jobFile.write('%s -m %s -r %d > %s 2> %s \n\n' % (nEXOsimPath,macroName,seed,screenOut,screenErr))
            elif runClustering:
                jobFile.write('cp ' + fileInfo['G4FileName'] + ' ' + fileInfo['ScratchDir'] + '/' + ' \n\n')
                
                
            if runClustering:
                simName = fileInfo['ScratchDir'] + '/' + fileInfo['G4FileName'].split('/')[-1]
                clusterName = fileInfo['ScratchDir'] + '/' + fileInfo['ClusterFileName'].split('/')[-1]
                
                screenOut = clusterName.replace('.root','.out')
                screenErr = clusterName.replace('.root','.err')

                clusterPath = card.Get('FullClusteringPath')
                if hasBinDir: clusterPath = clusterPath.replace('[BinTopDir]',card.Get('BinTopDir'))    
                jobFile.write('python %s -i %s -o %s -c %s > %s 2> %s \n\n' % (clusterPath, simName, clusterName, card.GetFileName(),screenOut,screenErr))
            elif runReconstruction:
                jobFile.write('cp ' + fileInfo['ClusterFileName'] + ' ' + fileInfo['ScratchDir'] + '/' + ' \n\n')
                
            if runReconstruction:
                clusterName = fileInfo['ScratchDir'] + '/' + fileInfo['ClusterFileName'].split('/')[-1]
                reconName = fileInfo['ScratchDir'] + '/' + fileInfo['ReconFileName'].split('/')[-1]
    
                screenOut = reconName.replace('.root','.out')
                screenErr = reconName.replace('.root','.err')
    
                reconPath = card.Get('FullReconstructionPath')
                if hasBinDir: reconPath = reconPath.replace('[BinTopDir]',card.Get('BinTopDir'))
                jobFile.write('python %s -i %s -o %s -c %s -s %d > %s 2> %s \n\n' % (reconPath, clusterName, reconName, card.GetFileName(),seed,screenOut,screenErr))

            # Move files to final location and remove scratch
            if runSimulation:
                jobFile.write('cp ' + fileInfo['ScratchDir'] + '/' + fileInfo['G4FileName'].split('/')[-1].replace('.root','.*') + ' ' + '/'.join(fileInfo['G4FileName'].split('/')[:-1]) + '\n')
                jobFile.write('rm ' + fileInfo['ScratchDir'] + '/' + fileInfo['G4FileName'].split('/')[-1].replace('.root','.*') + '\n\n')
            elif runClustering:
                jobFile.write('rm ' + fileInfo['ScratchDir'] + '/' + fileInfo['G4FileName'].split('/')[-1].replace('.root','.*') + '\n\n')

            if runClustering:
                jobFile.write('cp ' + fileInfo['ScratchDir'] + '/' + fileInfo['ClusterFileName'].split('/')[-1].replace('.root','.*') + ' ' + '/'.join(fileInfo['ClusterFileName'].split('/')[:-1]) + '\n')
                jobFile.write('rm ' + fileInfo['ScratchDir'] + '/' + fileInfo['ClusterFileName'].split('/')[-1].replace('.root','.*') + '\n\n')
            elif runReconstruction:
                jobFile.write('rm ' + fileInfo['ScratchDir'] + '/' + fileInfo['ClusterFileName'].split('/')[-1].replace('.root','.*') + '\n\n')                

            if runReconstruction:
                jobFile.write('cp ' + fileInfo['ScratchDir'] + '/' + fileInfo['ReconFileName'].split('/')[-1].replace('.root','.*') + ' ' + '/'.join(fileInfo['ReconFileName'].split('/')[:-1]) + '\n')
                jobFile.write('rm ' + fileInfo['ScratchDir'] + '/' + fileInfo['ReconFileName'].split('/')[-1].replace('.root','.*') + '\n\n')
            # Remove scratch directory
            jobFile.write('rmdir ' + fileInfo['ScratchDir'] + '\n')
                
        # no scratch directory
        else: 
            hasBinDir = card.Has('BinTopDir')
            if runSimulation:
                MacroBuildUtils.CreateMacro(fileInfo, card)
    
                macroName = fileInfo['MacroFileName']
                simName = fileInfo['G4FileName']
                
                screenOut = simName.replace('.root','.out')
                screenErr = simName.replace('.root','.err')
    
                nEXOsimPath = card.Get('FullnEXOSimPath')
                if hasBinDir: nEXOsimPath = nEXOsimPath.replace('[BinTopDir]',card.Get('BinTopDir'))               
                jobFile.write('%s -m %s -r %d > %s 2> %s \n\n' % (nEXOsimPath,macroName,seed,screenOut,screenErr))
                
            if runClustering:
                simName = fileInfo['G4FileName']
                clusterName = fileInfo['ClusterFileName']
                
                screenOut = clusterName.replace('.root','.out')
                screenErr = clusterName.replace('.root','.err')
    
                clusterPath = card.Get('FullClusteringPath')
                if hasBinDir: clusterPath = clusterPath.replace('[BinTopDir]',card.Get('BinTopDir'))    
                jobFile.write('python %s -i %s -o %s -c %s > %s 2> %s \n\n' % (clusterPath, simName, clusterName, card.GetFileName(),screenOut,screenErr))
                
            if runReconstruction:
                clusterName = fileInfo['ClusterFileName']
                reconName = fileInfo['ReconFileName']
    
                screenOut = reconName.replace('.root','.out')
                screenErr = reconName.replace('.root','.err')
    
                reconPath = card.Get('FullReconstructionPath')
                if hasBinDir: reconPath = reconPath.replace('[BinTopDir]',card.Get('BinTopDir'))
                jobFile.write('python %s -i %s -o %s -c %s -s %d > %s 2> %s \n\n' % (reconPath, clusterName, reconName, card.GetFileName(),seed,screenOut,screenErr))

        if card.Has('JobFooter'):
            jobFile.write(card.Get('JobFooter'))

        jobFile.close()

        jobList.append(jobName)

    return jobList

def SubmitJobs(card,jobs,submit, timeBetween):

    for i, job in enumerate(jobs):
        st = os.stat(job)
        os.chmod(job, st.st_mode | stat.S_IEXEC)

        #screenOut = "-o %s -e %s" % (card.Get('FullOutputPath'),card.Get('FullOutputPath'))

        # For the LLNL system, use a different command that redirects std-err and std-out to files
        if (card.Get('BatchFarmCall')=='msub'):
            folder, fName = os.path.split(job)
            jobName = os.path.splitext(fName)[0]
            systemOut = "-o %s/%s.sout -e %s/%s.serr" % (folder, jobName, folder, jobName)
            cmd = "%s -N %s_%d %s %s" % (card.Get('BatchFarmCall'), jobName, i, systemOut, job)
        else:
            cmd = "%s %s" % (card.Get('BatchFarmCall'),job)
        print cmd
        if submit:
            os.system(cmd)
            time.sleep(timeBetween)
        
if __name__ == "__main__":
    
    usage = "usage: python SubmitSimulation.py --card=card --submit-jobs"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-s","--submit-jobs",dest="submitJobs",action="store_true",default=False)
    parser.add_option("-t","--time-between", dest="timeBetween", nargs=1, default=0)

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not options.cardName:
        print 'You must specify the card name using the option "--card=cardName"'
        sys.exit()

    card = ReadCard(options.cardName)
    jobs = CreateJobs(card)
    SubmitJobs(card, jobs, options.submitJobs, float(options.timeBetween))

