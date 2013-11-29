#include "nameserver.h"
#include "vendingmachine.h"
#include "printer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) {
    prt.print(Printer::NameServer, 'S');
    machineList = new VendingMachine*[numVendingMachines];
    for (unsigned int i = 0; i < numVendingMachines; i++) {
        machineList[i] = NULL;                                                  // all values intially point to NULL
    }
    machineForStudent = new int[numStudents];                                   // array mapping student to machine
    for (unsigned int i = 0; i < numStudents; i++) {
        machineForStudent[i] = i % numVendingMachines;                          // loop through student and assign students to machines
    }
    waiting = new uCondition[numVendingMachines];                               // condition for each machine
}

void NameServer::VMregister(VendingMachine *vendingmachine) {
    unsigned int machineId = vendingMachine->getId();                           // to reduce the number of times we call this function
    machineList[machineId] = vendingMachine;
    prt.print(Printer::NameServer, 'R', machineId);
    waiting[machineId].signal();
}

VendingMachine *NameServer::getMachine(unsigned int id) {
    machineForStudent[id] = (machineForStudent[id] + 1) % machineList.size();   // go to the next machine
    if (!machineList[machineForStudent[id]]) {                                  // could've used if, but while is easier
        waiting.wait();                                                         // block if machine is not registered yet
    }
    prt.print(Printer::NameServer, 'N', id, machineForStudent[id]);
    return machineForStudent[i];
}

VendingMachine **NameServer::getMachineList() {
    for (unsigned int i = 0; i < machineList.size(); i++) {                     // make sure all machines are registered
        if (!machineList[i]) {
            waiting[i].wait();                                                  // block if NULL
        }
    }
    return machineList;
}

void NameServer::main() {
    // There has to be something here...
}

void NameServer::~NameServer() {
    delete waiting[];
    delete machineList[];
    delete machineForStudent[];
    prt.print(Printer::NameServer, 'F');
}
