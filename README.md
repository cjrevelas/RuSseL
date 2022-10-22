# RuSseL
This is the repository of the C++ version of RuSseL3D.

The code is named after the British philosopher and mathematician, Bertrand Russell.

It is an open-source code, distributed under the terms of the accompanying LICENSE.

The authors of the code are:\
Constantinos J. Revelas (cjrevelas@gmail.com)\
Aristotelis P. Sgouros (arissgouros@gmail.com)\
Apostolos T. Lakkas (tolis1981@gmail.com)

RuSseL is a code that was initially written in Fortran and applies the Finite Element Method to run three-
dimensional calculations on heterogeneous polymer systems, based on Self-Consistent Field Theory (SCFT).
At the moment, the code can address homopolymer melts in contact with solid surfaces and provide with
useful results regarding the thermodynamics and the structural properties of the system. The solid
surfaces can be either bare or grafted with polymer chains of the same chemical identity as the matrix
chains. We decided to re-write the code in C++ and adopt an object-oriented design, since the code
started growing fast and the need for new features has grown as well. We are aware that modern Fortran
supports object oriented development, but we find the syntax to be rather clunky. In addition, it is
cumbersome for us to use some higher-level data structures, like hashes in Fortran, since we have to rely
on external subroutines/libraries and incorporate them in the main code. 

The RuSseL3D repository includes the following files and directories:
 - README                -> current file
 - LICENCE               -> the GNU General Public License of the code (GPL)
 - Makefile              -> file controlling the compilation process (will be replaced by CMakeLists.txt in the future)
 - compile_commands.txt -> file for configuring the neovim C++ environment
 - .gitignore            -> contains all files and directories to be ignored by the git version control system
 - .git/                 -> directory containing the settings of the git version control system
 - obj/                  -> directory where all object files are dropped during the compilation process
 - run/                  -> directory where the compiled executable file is redirected after compilation
 - src/                  -> directory containing all the source files of the code
 - tools/                -> directory containing all the pre- and post- processing files
 - .husky/
 - ./node_modules/

To configure and compile the code, type the following commands inside the root directory of the code:\
    make

Some ideas are provided below for code extension:
 - block copolymer systems and polymer blends
 - incompressible models
 - systems in absence of matrix chains
 - non monodisperse matrix and grafted chains
 - find faster solver for linear system of equations (maybe iterative)
 - add an open source mesher
 - dynamic density functional theory calculations (ddft)
