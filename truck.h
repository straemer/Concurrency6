#ifndef TRUCK_H__
#define TRUCK_H__

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
public:
    Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
private:
    void main();
};

#endif
