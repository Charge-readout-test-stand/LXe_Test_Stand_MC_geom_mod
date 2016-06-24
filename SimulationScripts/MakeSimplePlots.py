
import ROOT
import sys

# global variables
globalArgs = {}

def CatchGlobalArguments(args):

    global globalArgs

    globalArgs['InputRootFileName'] = str(sys.argv[1])
    globalArgs['OutputRootFileName'] = str(sys.argv[2])
    globalArgs['TreeName'] = str(sys.argv[3])
    globalArgs['TreeType'] = str(sys.argv[4])
    
def MakeSimpleG4Plots(tree):

    outFile = ROOT.TFile(globalArgs['OutputRootFileName'],'recreate')

    #print 'Getting entry vectors...'
    #tree.SetEstimate(tree.GetEntries()+1)
    #tree.Draw("GenX:GenY:GenZ:TMath::Sqrt(GenX*GenX+GenY*GenY):1","","goff")
    #print 'Done! Number of selected rows =', tree.GetSelectedRows()

    dir = ROOT.gDirectory
    print 'Making GenX vs GenY...'
    tree.Draw("GenY:GenX>>h1","","colz goff")
    print 'Making Radius vs GenZ...'
    tree.Draw("TMath::Sqrt(GenX*GenX+GenY*GenY):GenZ>>h2","","colz goff")
    print 'Making Xpos vs Ypos...'
    tree.Draw("Xpos:Ypos>>h3","EnergyDeposit > 0","colz goff")
    print 'Making Radius vs Zpos...'
    tree.Draw("TMath::Sqrt(Xpos*Xpos+Ypos*Ypos):Zpos>>h4","EnergyDeposit > 0","colz goff")
    print 'Making TotalEventEnergy...'
    tree.Draw("TotalEventEnergy>>h5(250,0,5)","TotalEventEnergy > 0","goff")
    print 'Making Number of Deposits vs Radius...'
    tree.Draw("NumDeposits:TMath::Sqrt(Xpos*Xpos+Ypos*Ypos)>>h6","TotalEventEnergy > 0","goff")
    print 'Making Number of Deposits vs Zpos...'
    tree.Draw("NumDeposits:Zpos>>h7","TotalEventEnergy > 0","goff")
    print 'Making Number of Deposits...'
    tree.Draw("NumDeposits>>h8","TotalEventEnergy > 0","goff")
    print 'Making Number of Deposits vs TotalEventEnergy...'
    tree.Draw("NumDeposits:TotalEventEnergy>>h9","TotalEventEnergy > 0","goff")

    outFile.cd()
    dir.Get('h1').Write('GenX_vs_GenY')
    dir.Get('h2').Write('Radius_vs_GenZ')
    dir.Get('h3').Write('Xpos_vs_Ypos')
    dir.Get('h4').Write('Radius_vs_Zpos')
    dir.Get('h5').Write('TotalEventEnergy')
    dir.Get('h6').Write('NumDeposits_vs_Radius')
    dir.Get('h7').Write('NumDeposits_vs_Zpos')
    dir.Get('h8').Write('NumDeposits')
    dir.Get('h9').Write('NumDeposits')   

    c = ROOT.TCanvas()
    c.Divide(2,1)
    c.cd(1)
    dir.Get('h1').Draw('colz')
    c.cd(2)
    dir.Get('h2').Draw('colz')  
    c.Write('GenX_vs_GenY')


    outFile.Close()

    return


def MakeSimpleClusterPlots(tree):

    outFile = ROOT.TFile(globalArgs['OutputRootFileName'],'recreate')

    dir = ROOT.gDirectory
    print 'Making NumClusters...'
    tree.Draw("NumClusters>>h0(50,0,50)","Weight*(NumClusters > 0)","goff")
    print 'Making NumClusters vs TotalEventEnergy...'
    tree.Draw("NumClusters:TotalEventEnergy>>h1(250,0,5,50,0,50)","Weight*(NumClusters > 0)","goff")
    print 'Making NuclideZ...'
    tree.Draw("NuclideZ>>h2(200,0,200)","Weight","goff")

    print 'Cut num clusters > 0 ...'
    tree.Draw(">>numc","NumClusters > 0","entrylist")
    tree.SetEntryList(dir.Get('numc'))

    print 'Making TotalEventEnergy...'
    tree.Draw("TotalEventEnergy>>h5(250,0,5)","Weight*(TotalEventEnergy > 0)","goff")

    print 'Cut num clusters > 0 && EnergyDeposit > 0...'
    tree.Draw(">>numcAed","NumClusters > 0 && EnergyDeposit > 0","entrylist")
    tree.SetEntryList(dir.Get('numcAed'))    

    print 'Making Xpos vs Ypos...'
    tree.Draw("Xpos:Ypos>>h3","Weight*(EnergyDeposit > 0)","colz goff")
    print 'Making Radius vs Zpos...'
    tree.Draw("TMath::Sqrt(Xpos*Xpos+Ypos*Ypos):Zpos>>h4","Weight*(EnergyDeposit > 0)","colz goff")


    print 'Making NumClusters vs Radius...'
    tree.Draw("NumClusters:TMath::Sqrt(Xpos*Xpos+Ypos*Ypos)>>h6(700,0,700,50,0,50)","Weight*(EnergyDeposit > 0)","colz goff")
    print 'Making NumClusters vs Zpos...'
    tree.Draw("NumClusters:Zpos>>h7(2000,-1000,1000,50,0,50)","Weight*(EnergyDeposit > 0)","colz goff")
    
    outFile.cd()
    dir.Get('h0').Write('NumClusters')
    dir.Get('h1').Write('NumClusters_vs_TotalEventEnergy')
    dir.Get('h2').Write('NuclideZ')
    dir.Get('h3').Write('Xpos_vs_Ypos')
    dir.Get('h4').Write('Radius_vs_Zpos')
    dir.Get('h5').Write('TotalEventEnergy')
    dir.Get('h6').Write('NumClusters_vs_Radius')
    dir.Get('h7').Write('NumClusters_vs_Zpos')


    outFile.Close()

    return


def MakeSimpleReconPlots(tree):

    outFile = ROOT.TFile(globalArgs['OutputRootFileName'],'recreate')

    dir = ROOT.gDirectory
    print 'Making ReconFlag...'
    tree.Draw("ReconFlag>>h1(7,-3,3)","Weight","goff")
    print 'Cut recon flag == 2...'
    tree.Draw(">>rec2","ReconFlag == 2","entrylist")
    tree.SetEntryList(dir.Get('rec2'))

    print 'Making TotalReconEnergy...'
    tree.Draw("TotalReconEnergy>>h2(250,0,5)","Weight","goff")
    print 'Making SmearedEnergy...'
    tree.Draw("SmearedEnergy>>h3(250,0,5)","Weight","goff")
    print 'Making Number of Recon Clusters'
    tree.Draw("NumReconClusters>>h4(20,0,20)","Weight","goff")
    print 'Making Number of Recon Clusters vs True Energy'
    tree.Draw("NumReconClusters:TotalEventEnergy>>h5(250,0,5,20,0,20)","Weight","goff")
    print 'Making SS/MS separation...'
    tree.Draw("TotalReconEnergy>>hss(250,0,5)","Weight*(NumReconClusters == 1)","goff")
    tree.Draw("TotalReconEnergy>>hms(250,0,5)","Weight*(NumReconClusters > 1)","goff")
    tree.Draw("TotalReconEnergy>>hall(250,0,5)","Weight*(NumReconClusters >= 1)","goff")
    print 'Making SS/MS smeared separation...'
    tree.Draw("SmearedEnergy>>hsss(250,0,5)","Weight*(NumReconClusters == 1)","goff")
    tree.Draw("SmearedEnergy>>hsms(250,0,5)","Weight*(NumReconClusters > 1)","goff")
    tree.Draw("SmearedEnergy>>hsall(250,0,5)","Weight*(NumReconClusters >= 1)","goff")
    
    outFile.cd()
    hss = dir.Get('hss')
    hss.Write('TotalReconEnergy_ss')
    hss.Sumw2()
    hms = dir.Get('hms')
    hms.Write('TotalReconEnergy_ms')
    hms.Sumw2()
    hall = dir.Get('hall')
    hall.Write('TotalReconEnergy_all')
    hall.Sumw2()    
    hss.Divide(hall)
    hss.Write('ssms_separation')

    hsss = dir.Get('hsss')
    hsss.Write('SmearedEnergy_ss')
    hsss.Sumw2()
    hsms = dir.Get('hsms')
    hsms.Write('SmearedEnergy_ms')
    hsms.Sumw2()
    hsall = dir.Get('hsall')
    hsall.Write('SmearedEnergy_all')
    hsall.Sumw2()    
    hsss.Divide(hall)
    hsss.Write('ssms_smeared_separation')

    dir.Get('h1').Write('ReconFlag')
    dir.Get('h2').Write('TotalReconEnergy')
    dir.Get('h3').Write('SmearedEnergy')
    dir.Get('h4').Write('NumReconClusters')
    dir.Get('h5').Write('NumReconClusters_vs_TotalEventEnergy')

    outFile.Close()

    return
    

if __name__ == "__main__":

    CatchGlobalArguments(sys.argv)

    inFile = ROOT.TFile.Open(globalArgs['InputRootFileName'])
    if not inFile:
        print 'File named', globalArgs['InputRootFileName'], 'not found...'
        sys.exit()

    tree = inFile.Get(globalArgs['TreeName'])
    if not tree:
        print 'Tree named', globalArgs['TreeName'], 'not found...'
        sys.exit()

    if globalArgs['TreeType'].lower() == 'g4':
        MakeSimpleG4Plots(tree)
    elif globalArgs['TreeType'].lower() == 'cluster':
        MakeSimpleClusterPlots(tree)
    elif globalArgs['TreeType'].lower() == 'recon':
        MakeSimpleReconPlots(tree)

    inFile.Close()
