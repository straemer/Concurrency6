#include "nameserver.h"
#include "vendingmachine.h"
#include "printer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) :
    prt(prt),
    machineList(new VendingMachine*[numVendingMachines]),
    machineForStudent(numStudents),
    numVendingMachines(numVendingMachines),
    numRegisteredMachines(0)
{
    prt.print(Printer::NameServer, 'S');
    for (unsigned int i = 0; i < numVendingMachines; i++) {
        machineList[i] = NULL;                                                  // all values intially point to NULL
    }
    for (unsigned int i = 0; i < numStudents; i++) {
        machineForStudent[i] = i % numVendingMachines;                          // loop through student and assign students to machines
    }
}

void NameServer::VMregister(VendingMachine *vendingMachine) {
    unsigned int machineId = vendingMachine->getId();                           // to reduce the number of times we call this function
    machineList[machineId] = vendingMachine;
    prt.print(Printer::NameServer, 'R', machineId);
}

VendingMachine *NameServer::getMachine(unsigned int id) {
    machineForStudent[id] = (machineForStudent[id] + 1) % numVendingMachines;   // go to the next machine
    prt.print(Printer::NameServer, 'N', id, machineForStudent[id]);
    return machineList[machineForStudent[id]];
}

VendingMachine **NameServer::getMachineList() {
    for (unsigned int i = 0; i < numVendingMachines; i++) {                     // make sure all machines are registered
    }
    return machineList;
}

void NameServer::main() {
    for (;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept(VMregister) {
            ++numRegisteredMachines;
            assert(numRegisteredMachines <= numVendingMachines);
        } or _When(numRegisteredMachines == numVendingMachines)
              _Accept(getMachine, getMachineList) {
        }
    }
    prt.print(Printer::NameServer, 'F');
}

NameServer::~NameServer() {
    delete [] machineList;
}
