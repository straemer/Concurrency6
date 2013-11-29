#include "bottlingplant.h"

#include "truck.h"

#include "MPRNG.h"

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments) :
    m_printer(prt),
    m_nameServer(nameServer),
    m_numVendingMachines(numVendingMachines),
    m_maxShippedPerFlavour(maxShippedPerFlavour),
    m_maxStockPerFlavour(maxStockPerFlavour),
    m_timeBetweenShipments(timeBetweenShipments),
    m_production(0),
    m_terminationStatus(NotTerminating)
{}

BottlingPlant::~BottlingPlant() {
    assert(m_terminationStatus == NotTerminating);
    m_terminationStatus = DestructorCalled;
    m_terminateCondition.wait();
}

bool BottlingPlant::getShipment(unsigned int cargo[]) {
    if (m_terminationStatus == NotTerminating) {
        for (unsigned i=0; i<4; ++i) {
            cargo[i] = m_production;
        }
        m_productionCondition.signal();
        return false;
    } else {
        m_terminationStatus |= TruckNotified;
        m_productionCondition.signal();
        if (m_terminationStatus == (DestructorCalled | TruckNotified | MainTerminated)) {
            m_terminateCondition.signal();
        }
        return true;
    }
}

void BottlingPlant::main() {
    Truck truck(m_printer, m_nameServer, *this, m_numVendingMachines, m_maxStockPerFlavour);
    for (;;) {
        yield(m_timeBetweenShipments);
        m_production = g_mprng(m_maxShippedPerFlavour);
        m_productionCondition.wait();

        if (m_terminationStatus != NotTerminating) {
            break;
        }
    }
    m_terminationStatus |= MainTerminated;
    if (m_terminationStatus != (DestructorCalled | TruckNotified | MainTerminated)) {
        m_productionCondition.wait();
    }
    m_terminateCondition.signal();
}
