#ifndef BOTTLING_PLANT_H__
#define BOTTLING_PLANT_H__

_Task BottlingPlant {
 public:
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments);
    bool getShipment(unsigned int cargo[]);
private:
    void main();
};

#endif
