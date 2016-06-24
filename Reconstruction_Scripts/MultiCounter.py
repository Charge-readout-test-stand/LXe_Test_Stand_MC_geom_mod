# Count ROI events using extremely simple methods.
# Basically, loop over the events, take the total energy deposit in
# the xenon, apply energy resolution, and see how many events
# remain in the ROI.  The SS/MS discrimination, FV, and other
# such things are not considered here.

import ROOT
import code

# path to the simulation output to be processed

#inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-D-112414/"
inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-C-030715/"
#inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-B-031115/"
#inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-D-022715/"
#inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-D-030215/"
#inpath = "/nfs/slac/g/exo_data6/exo_data/data/nEXO_simulation_output/josh-hfe-thickness-A-031415/"

# List of filename forms to be looped over (one form for each component)
# This is the part I change most often.

informlist = [
"Th_thickness_output_169_223_*_Tl208_TPC.root",
"Th_thickness_output_169_223_*_Tl208_HFE.root",
"Th_thickness_output_169_223_*_Tl208_InnerCryo.root",
"Th_thickness_output_169_223_*_Tl208_OuterCryo.root"
]
#informlist = [
#"U_thickness_output_131_186_*_Bi214_TPC.root",
#"U_thickness_output_131_186_*_Bi214_HFE.root"
#"U_thickness_output_169_223_*_Bi214_HFE.root"
#"U_thickness_output_151_206_*_Bi214_HFE.root"
#"U_thickness_output_181_236_*_Bi214_HFE.root"
#"U_thickness_output_131_186_*_Bi214_InnerCryo.root",
#"U_thickness_output_131_186_*_Bi214_OuterCryo.root"
#]

# read in files of a particular form, and do the energy resolution smearing and print results
def GetEfficiencies(inform):
   tc = ROOT.TChain("nEXOevents")
   tc.Add(inpath + inform)
   
   nbinsunsmeared = 5000
   nbinssmeared = 5000
   emin = 0.0  # note energy is in MeV
   emax = 5.0
   h_unsmeared = ROOT.TH1D("h_unsmeared", "unsmeared total energy; E (MeV);", nbinsunsmeared, emin, emax)
   h_smeared = ROOT.TH1D("h_smeared", "smeared total energy; E (MeV);", nbinssmeared, emin, emax)

   print "inform is: ", inform

   
   n_entries = tc.GetEntries()
   
   print "Total entries: ", str(n_entries)
   
   c1 = ROOT.TCanvas()
   tc.Draw("TotalEventEnergy>>h_unsmeared", "")
   
   # Now, do energy smearing
   nsmear = 2000   # number of random throws per bin for smearing
   eresolution = 0.01   # 1% energy resolution, change this if you want!

   rando = ROOT.TRandom3(4096)
   
   for i in range(nbinsunsmeared):
      if i%100 == 0:
          print "Now doing bin ", i
      ibin = i + 1
      n_in_bin = h_unsmeared.GetBinContent(ibin)
      bin_center = h_unsmeared.GetBinCenter(ibin)
      if i%100 == 0:
          print "bin center ", bin_center
      for j in range(nsmear):
          h_smeared.Fill(rando.Gaus(bin_center, bin_center*eresolution), n_in_bin/float(nsmear))
   
   
   c2 = ROOT.TCanvas()
   
   # Now, let's count what is in the ROI
   # 1 keV bins -> 
   ROI_low_bin = 2434
   ROI_high_bin = 2482
   counts_in_ROI = h_smeared.Integral(ROI_low_bin, ROI_high_bin)
   print "counts in ROI: ", str(counts_in_ROI)
   print "final rate is: ", str(counts_in_ROI/float(n_entries))
   
   h_smeared.Rebin(5)
   h_smeared.Draw()

   #h_unsmeared.Clear()
   #h_smeared.Clear()
   #tc.Clear()

# Here's actually the main loop
for inform in informlist:
   GetEfficiencies(inform)


code.interact("entering interactive session",local=globals())
