CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = bank.o printer.o vendingmachine.o bottlingplant.o nameserver.o student.o watcard.o config.o parent.o truck.o watcardoffice.o MPRNG.o

ALL_OBJECTS = ${OBJECTS} main.o
TEST_OBJECTS = ${OBJECTS} test.o

EXEC = soda
TEST_EXEC = test
ALL_EXECS = ${EXEC} ${TEST_EXEC}

DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean

all : ${ALL_EXECS}					# build all executables

%.o : %.cc					# change compiler 1st executable
	${CXX} ${CXXFLAGS} -c $< -o $@

${EXEC} : ${ALL_OBJECTS}
	${CXX} $^ -o $@

#############################################################
# Test Compilation
#############################################################
test : ${TEST_EXEC}

${TEST_EXEC} : ${TEST_OBJECTS}
	${CXX} $^ -o $@

${ALL_OBJECTS} : ${MAKEFILE_NAME}               # OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${ALL_EXECS}
