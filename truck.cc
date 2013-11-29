#include "truck.h"

#include "bottlingplant.h"
#include "nameserver.h"
#include "vendingmachine.h"

#include "MPRNG.h"

using namespace std;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
             unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
    m_printer(prt),
    m_nameServer(nameServer),
    m_plant(plant),
    m_numVendingMachines(numVendingMachines),
    m_maxStockPerFlavour(maxStockPerFlavour) {}

void Truck::main() {
    VendingMachine **vendingMachines = m_nameServer.getMachineList();
    unsigned cargo[4];
    for (;;) {
        yield(g_mprng(1,10));
        unsigned numEmptyInventory = 0;
        if (m_plant.getShipment(cargo)) {
            break;
        }
        if (cargo[0] == 0) {
            // empty delivery...
            continue;
        }
        for (unsigned i=0; i<m_numVendingMachines; ++i) {
            unsigned *inventory = vendingMachines[i]->inventory();
            for (unsigned j=0; j<4; ++j) {
                if (cargo[j] != 0) {
                    unsigned restockAmount = min(m_maxStockPerFlavour-inventory[j], cargo[j]);
                    inventory[j] += restockAmount;
                    cargo[j] -= restockAmount;
                    if (cargo[j] == 0) {
                        ++numEmptyInventory;
                        if (numEmptyInventory == 4) {
                            break;
                        }
                    }
                }
            }
            vendingMachines[i]->restocked();
        }
    }
}
