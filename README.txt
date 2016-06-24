To compile the nEXO code with version 10.0p0 of Geant4, source this file:
/afs/slac.stanford.edu/package/geant4/env-rhel5(6)-32(64).(c)sh

It turns out that the env-rhel6-64.csh file does not work as it is set up
there.
As an alternative, use the env-rhel6-64.csh file in this directory.

The build procedure is as follows:

1. Check out the code.  See
https://ntpc.ucllnl.org/nEXO/index.php/Detector_Simulations/Documentation/Download,_build_and_run_the_code
(these instructions should be duplicated there (hopefully))

2. Be sure you don't already have a "Makefile" or anything like
CMakeCache.txt.  These files will override what you are trying to do.

3. Source the environment script (described above) if you haven't already.

4. Make a build directory.  It should not be the same as this directory
with README.txt, src, include, and so on.

5. Change directories to this build directory.  Let's call the directory
with the README.txt, src, include, etc. <source_dir>

6. In the build directory, execute:
cmake <source_dir>

Now it should run and build the files cmake needs to compile things.

7. Now, still in the build directory, simply execute:
make

It should run and successfully build the nEXO executable.  Congratulations!
