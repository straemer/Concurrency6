#ifndef BOTTLING_PLANT_H__
#define BOTTLING_PLANT_H__

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
public:
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments);
    virtual ~BottlingPlant();

    bool getShipment(unsigned int cargo[]);
private:
    void main();

    uCondition m_productionCondition;
    uCondition m_terminateCondition;

    Printer &m_printer;
    NameServer &m_nameServer;
    const unsigned m_numVendingMachines;
    const unsigned m_maxShippedPerFlavour;
    const unsigned m_maxStockPerFlavour;
    const unsigned m_timeBetweenShipments;
    unsigned m_production;

    enum TerminationStatus {
        NotTerminating = 0,
        DestructorCalled = 1 << 0,
        TruckNotified = 1 << 1,
        MainTerminated = 1 << 2
    };
    unsigned m_terminationStatus;
};

#endif
