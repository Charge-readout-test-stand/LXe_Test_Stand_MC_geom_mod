These scripts are ported from old EXO sim, originally written by Tessa.
They are designed for making decay MC for isotopes in secular equilibrium.
The Carleton group has a different framework for running the simulation,
but these scripts should work for now.  They have been used for the
HFE thickness studies.

To use the CreateExecute_U.sh and CreateExecute_Th.sh (the cobalt one is
not updated or ready) in the meantime, you can still do that.

Open up one of the files, and edit it for running, I have marked the 
important things that I expect you to want to change with a:
#*****

Special consideration should be given to the list of TPC components
to confine decays to, and the isotopes to generate.

You will also need to edit DecayValues.txt.  That is the actual
input file which specifies how many decays to do for each 
component.  The numdecays values give how many decays to do per file,
and the factor values give how many files to generate.

Please don't hesitate to contact me (joalbert@indiana.edu) if you have
questions or problems!
(jba 5/8/15)
