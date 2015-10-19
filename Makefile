# System architecture
SYSTEM     = x86-64_linux

# Static library format for Cplex
LIBFORMAT  = static_pic

# Source code folder
SRC	= src
INCLUDE = include

# Machine hostname
MACHINE = $(shell hostname)

# Library type(STATIC or DYNAMIC)
MERGE = DYNAMIC

##### Folders
# Temp folders
TMP_GIRP = ./tmp/LR
TMP_STATIC = ./tmp/lib/static
# Perm folders
DAT_DOXYFILE = ./dat/doxyfile
DAT_INSTANCES = ./dat/instances
DAT_LP_MODELS = ./dat/lp_models
DAT_RESULTS = ./dat/results


# Cplex directory
CPLEXDIR	  = /opt/ibm/ILOG/CPLEX_Studio1261/cplex

# Concert directory
CONCERTDIR	  = /opt/ibm/ILOG/CPLEX_Studio1261/concert

# Compiler
CCC = g++-4.8

# Compilation parameters (Add afterward: --coverage -pg -ftree-vectorize -mfpmath=sse -march=native)
CCOPT = -std=gnu++0x -O3 -ftree-vectorize -mfpmath=sse -march=native -march=native -flto -g -m64 -fPIC -fexceptions -DNDEBUG -DIL_STD

# Cplex static libraries directory
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Concert static libraries directory
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Include libraries identifiers
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread

# Cplex header's directory
CPLEXINCDIR   = $(CPLEXDIR)/include

# Concert header's directory
CONCERTINCDIR = $(CONCERTDIR)/include

# Header's include path
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

# Executable name
CPP_EX = LR-UrApHMP

# Compiling
all:
	mkdir -p $(TMP_GIRP)
	mkdir -p $(TMP_STATIC)
	mkdir -p $(DAT_DOXYFILE)
	mkdir -p $(DAT_INSTANCES)
	mkdir -p $(DAT_LP_MODELS)
	mkdir -p $(DAT_RESULTS)
	make -j8 $(CPP_EX);

# Executing
execute: $(CPP_EX)
	./$(CPP_EX)

# Cleaning
clean:
	# /bin/rm -rf $(CPP_EX)
	/bin/rm -rf ./tmp
	/bin/rm -rf ./dat


########################## FRAMEWORK OBJECT's ######################################################
# CONFIG
# $(TMP_GIRP)/FWConfig.o: $(SRC)/framework/config/FWConfig.cpp $(SRC)/framework/config/FWConfig.h
	# $(CCC) -c $(CCFLAGS) $(SRC)/framework/config/FWConfig.cpp -o $(TMP_GIRP)/FWConfig.o

# $(TMP_GIRP)/FWManager.o: $(SRC)/framework/config/FWManager.cpp $(SRC)/framework/config/FWManager.h
	# $(CCC) -c $(CCFLAGS) $(SRC)/framework/config/FWManager.cpp -o $(TMP_GIRP)/FWManager.o

########################## GENERATING OBJECT's ######################################################

# CONFIGURATION - INSTANCES
$(TMP_GIRP)/UrApHMP.o: $(SRC)/UrApHMP.cpp $(INCLUDE)/UrApHMP.h
	$(CCC) -c $(CCFLAGS) $(SRC)/UrApHMP.cpp -o $(TMP_GIRP)/UrApHMP.o

# STRUCTURE - SOLUTION
$(TMP_GIRP)/solution.o: $(SRC)/solution.cpp $(INCLUDE)/solution.h
	$(CCC) -c $(CCFLAGS) $(SRC)/solution.cpp -o $(TMP_GIRP)/solution.o

# EXACT
$(TMP_GIRP)/model.o: $(SRC)/model.cpp $(INCLUDE)/model.h
	$(CCC) -c $(CCFLAGS) $(SRC)/model.cpp -o $(TMP_GIRP)/model.o
$(TMP_GIRP)/solver.o: $(SRC)/solver.cpp $(INCLUDE)/solver.h
	$(CCC) -c $(CCFLAGS) $(SRC)/solver.cpp -o $(TMP_GIRP)/solver.o

# LAGRANGIAN RELAXATION
$(TMP_GIRP)/subgradient.o: $(SRC)/subgradient.cpp $(INCLUDE)/subgradient.h
	$(CCC) -c $(CCFLAGS) $(SRC)/subgradient.cpp -o $(TMP_GIRP)/subgradient.o

# MAIN
$(TMP_GIRP)/main.o: $(SRC)/main.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/main.cpp -o $(TMP_GIRP)/main.o

########################## OBJECT's LIBRARIES #######################################################
# CONFIGURATION
$(TMP_GIRP)/Configuration.o:  $(TMP_GIRP)/UrApHMP.o
	gcc -Wl,-r  $(TMP_GIRP)/UrApHMP.o -o $(TMP_GIRP)/Configuration.o -nostdlib

# STRUCTURE
$(TMP_GIRP)/Structure.o: $(TMP_GIRP)/solution.o
	gcc -Wl,-r $(TMP_GIRP)/solution.o -o $(TMP_GIRP)/Structure.o -nostdlib

# EXACT
$(TMP_GIRP)/Exact.o: $(TMP_GIRP)/model.o $(TMP_GIRP)/solver.o
	gcc -Wl,-r $(TMP_GIRP)/model.o $(TMP_GIRP)/solver.o -o $(TMP_GIRP)/Exact.o -nostdlib

# LAGRANGIAN
$(TMP_GIRP)/Lagrangian.o: $(TMP_GIRP)/subgradient.o
	gcc -Wl,-r $(TMP_GIRP)/subgradient.o -o $(TMP_GIRP)/Lagrangian.o -nostdlib

########################## LINKANDO TUDO ########################################################
$(CPP_EX): $(TMP_GIRP)/Exact.o $(TMP_GIRP)/Configuration.o $(TMP_GIRP)/Structure.o $(TMP_GIRP)/Lagrangian.o $(TMP_GIRP)/main.o
	$(CCC)  $(CCFLAGS) $(TMP_GIRP)/Exact.o $(TMP_GIRP)/Configuration.o $(TMP_GIRP)/Structure.o $(TMP_GIRP)/Lagrangian.o $(TMP_GIRP)/main.o -L$(TMP_STATIC) -o $(CPP_EX) $(CCLNFLAGS)
#endif
