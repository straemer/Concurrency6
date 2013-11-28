CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o bank.o printer.o vendingmachine.o bottlingplant.o nameserver.o student.o watcard.o config.o parent.o truck.o watcardoffice.o

EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

%.o : %.cc					# change compiler 1st executable
	${CXX} ${CXXFLAGS} -c $< -o $@

${EXEC} : ${OBJECTS}
	${CXX} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}
