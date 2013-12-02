#ifndef NAME_SERVER_H__
#define NAME_SERVER_H__

#include <vector>

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
public:
    NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents);
    void VMregister(VendingMachine *vendingmachine);
    VendingMachine *getMachine(unsigned int id);
    VendingMachine **getMachineList();

    virtual ~NameServer();
private:
    Printer &prt;
    VendingMachine **machineList;

    std::vector<int> machineForStudent;
    unsigned numVendingMachines;
    unsigned numRegisteredMachines;

    void main();
};

#endif
