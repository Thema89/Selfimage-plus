SelfImage BUILDING NOTES
------------------------

Requirements
------------
To build SelfImage you will need:
 - SelfImage source tree
 - Borland C++ Builder 6 (BCB6)
 - UPX (to compress the executable)
 - Shalom Help (to build the help file)
 - Nullsoft Scriptable Install System (to make the installer)
 - TMultiReadExclusiveWriteSynchronizer (TMREWS) update for Delphi 6
   (optional)


TMultiReadExclusiveWriteSynchronizer
------------------------------------
Borlands's TMultiReadExclusiveWriteSynchronizer VCL class (or TMREWS) is used
by SelfImage.  This class is an alternative to using critical sections in
certain multi-threaded code.  Borland has issued an update to this class which
addresses deadlocking issues.  Although it has been, as of the time of this
writing, almost four years since this update was released, it is still
considered to be a "public beta" and "has not yet been certified".  Also, the
problem intended to be fixed by this update reportedly arises when "lots of
threads are using the same TMREWS object at the same time."  Despite the fact
that the update is a beta and that SelfImage doesn't have lots of threads
using the same TMREWS object, it is suggested that this update be used.
Licensing restrictions prevent the update from being distributed with
SelfImage, so it must be downloaded from Borland's CodeCentral.  See the
Borland Developer Network article at
http://bdn.borland.com/article/0,1410,28599,00.html for details.

This update is in the form of a replacement for the pascal SysUtils unit.  The
project file for SelfImage already references SysUtils.pas - all you need to
do is place the updated SysUtils.pas file in the main directory where the rest
of SelfImage is, and it will be built and the updated code included in the
resulting binaries.


Building
--------
Because Borland doesn't like to store relative paths in project files, you
will probably have to change several paths before the project will load/build.
When you unzip the source tree, make sure you leave the SelfImage and Shared
folders next to each other.  If you do this, you can simply load SelfImage.bpr
in a text editor and replace all instances of "w:\shared" with "..\shared".

Once this is done, simply load the SelfImage.bpg (not SelfImage.bpr) project
group.  There's no particular magic to building SelfImage at this point. The
project group includes the zlib and libbz2 projects, so you can build them all
at the same time.  If you decide not to build with the above-mentioned TMREWS
update, then you can remove SysUtils.pas from the SelfImage project. Otherwise,
once it is present and the JVCL components are installed, it should build
normally and without warnings.

A custom (batch file) build step has been added at the end to compress the
executable.  Compression is done mostly to support the use of SelfImage from
within a BartPE boot CD environment. This build batch file assumes that you
have UPX (Ultimate Packer for eXecutables) in your command path.  If it's not
in your path, then edit upx_compress.bat to reflect its location.  You can
remove this build step entirely if you don't want to compress your executable.
UPX is free software, available from http://upx.sourceforge.net.


Optimized memcpy/memset/memzero
-------------------------------
A small library of highly optimized memory routines has been used in SelfImage.
It adds the functions memcpy_optimized(), memset_optized(), and
memzero_optimized().  Each of these includes MMX, 3D-NOW, and SSE versions.
If there are any issues with these routines, you can cause SelfImage to fall
back to standard memcpy() and memset() by defining NO_ASM_MEMCPY and
recompiling.

The assembly routines are hand-aligned for each release to begin at a 16-byte
boundary.  While this isn't necesary for them to function, they operate at
optimum efficiency if they are aligned.  If you have questions on how to do
this alignment, contact me by email.


Logging
-------
A logging version can be built in one of two ways.  Either you can turn on
debugging (which causes the _DEBUG macro to be defined), or by defining
the ENABLE_LOGGING macro.  See the LOGGING comment in SelfImage.cpp for
details on setting the log level.

If logging is not enabled with either the _DEBUG or ENABLE_LOGGING #define,
then the log macros throughout the project will actually expand to nothing.
Thus, logging statements have zero overhead on a production build, so feel
free to add as many as you wish.


Shalom Help
-----------
The help file for SelfImage was created using the Shalom Help help authoring
program.  You can download it free of charge from:
  http://www.danish-shareware.dk/soft/shelpm/
The main project file for it is help\SelfImage.shp


NSIS Installer
--------------
The installer for SelfImage is created using the Nullsoft Scriptable Install
System, available at:
  http://nsis.sourceforge.net/  
The main install script is install\SelfImage.nsi


Problems
--------
If you have problems building SelfImage, feel free to contact me at:
kfitzner@excelcia.org.

Don't, however, bring me issues about porting SelfImage to MSVC, Delphi, or
earlier versions of C++ Builder.