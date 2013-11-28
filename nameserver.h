#ifndef NAME_SERVER_H__
#define NAME_SERVER_H__

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
public:
    NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents);
    void VMregister(VendingMachine *vendingmachine);
    VendingMachine *getMachine(unsigned int id);
    VendingMachine **getMachineList();
private:
    void main();
};

#endif
