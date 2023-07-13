###################################################################################################################################################
#                                                            MAKEFILE OF SCF-FEM CODE                                                             #
###################################################################################################################################################
#--------------------------------------------------SET COMPILER FLAGS AND LIBRARIES TO BE LINKED--------------------------------------------------#
MAKE_MPI_RUN        = 0
MAKE_PRODUCTION_RUN = 0
PROD_OPTIONS        =
DEBUG_OPTIONS       = -DDEBUG_OUTPUTS #-DPRINT_AFULL
BOTH_OPTIONS        = # -DMUMPS_REPORT

CCOMP = /usr/bin/gcc
CPPCOMP = /usr/bin/g++

topdir = /home/cjrevelas/libraries/mumps_serial_cpp/

libdir = $(topdir)/lib

.SECONDEXPANSION:
include $(topdir)/Makefile.inc
LIBMUMPS_COMMON = $(libdir)/libmumps_common$(PLAT)$(LIBEXT)
LIBDMUMPS       = $(libdir)/libdmumps$(PLAT)$(LIBEXT) $(LIBMUMPS_COMMON)

CC_PROD  = -O3
CC_DEBUG = -std=c++14 -O0 -g -Wall -Wextra -pedantic-errors\

ifeq ($(MAKE_PRODUCTION_RUN),0)
CPPFLAGS = $(DEBUG_OPTIONS) $(BOTH_OPTIONS)
CCFLAGS  = $(CC_DEBUG) -cpp $(CPPFLAGS)
else
CPPFLAGS = $(PROD_OPTIONS) $(BOTH_OPTIONS)
CCFLAGS  = $(CC_PROD) -cpp $(CPPFLAGS)
endif

INCLDIR += -Isrc/include
INCLDIR += -I/home/cjrevelas/libraries/mumps_serial_cpp/include
INCLDIR += -I/home/cjrevelas/libraries/mumps_serial_cpp/libseq
INCLDIR += -I/usr/lib/gcc/x86_64-linux-gnu/9/include

LIBFS=-lstdc++ #-lm
LD = ld

LDFLAGS = $(CCFLAGS)
LIBOTHERS = -L/usr/lib/gcc/x86_64-linux-gnu/11/ -lgfortran
LIBOTHERS += -lpthread
#--------------------------------------------------------SET FILE PATHS AND EXECUTABLE NAME-------------------------------------------------------#
OBJDIR=obj
SRCDIR=src
RUNDIR=run


OBJECTS=$(OBJDIR)/Variable.o\
	$(OBJDIR)/Eos.o\
	$(OBJDIR)/EosHelfand.o\
	$(OBJDIR)/Memory.o\
        $(OBJDIR)/Mesh.o\
	$(OBJDIR)/Fem.o\
	$(OBJDIR)/Russel.o\
	$(OBJDIR)/StringOperations.o\
	$(OBJDIR)/EvalArg.o\
	$(OBJDIR)/Evaluator.o\
        $(OBJDIR)/Parser.o\
	$(OBJDIR)/IOHelper.o\
        $(OBJDIR)/Main.o

EXEC=$(RUNDIR)/russel
#-----------------------------------------------------------------COMPILE AND LINK----------------------------------------------------------------#
#compilation
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CPPCOMP) $(CCFLAGS) $(INCLDIR) $(LIBFS) -c -o $@ $?

#link object files to create the executable code
$(EXEC):$(LIBDMUMPS) $(OBJECTS)
	$(CPPCOMP) $(LDFLAGS) -o $(EXEC) $(OPTL) $(OBJECTS) $(LIBDMUMPS) $(LORDERINGS) $(LIBS) $(LIBBLAS) $(LIBOTHERS)

.SUFFIXES: (.SUFFIXES) .c .cpp .h .hpp
#-------------------------------------------------------------CLEAN WORKING DIRECTORY-------------------------------------------------------------#
clean:
	$(RM) $(OBJDIR)/*.o

cleaner:
	$(RM)  $(OBJDIR)/*.o $(RUNDIR)/RuSseL* $(RUNDIR)/o.* $(RUNDIR)/d.* $(RUNDIR)/e.* $(RUNDIR)/*out.bin

#-----------------------------------------------------------RUN TESTS TO VERIFY CHANGES-----------------------------------------------------------#
test:
	./test_integrity/test_integrity.sh

cleantest:
	$(RM)  $(OBJDIR)/*.o $(OBJDIR)/*.mod $(RUNDIR)/RuSseL* $(RUNDIR)/o.* $(RUNDIR)/d.* $(RUNDIR)/e.* $(RUNDIR)/*out.bin $(RUNDIR)/fort.* $(RUNDIR)/in.field.bin $(RUNDIR)/LOG*
###################################################################################################################################################
