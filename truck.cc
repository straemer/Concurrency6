#include "truck.h"

#include "bottlingplant.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "printer.h"

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
    m_printer.print(Printer::Truck, 'S');
    VendingMachine **vendingMachines = m_nameServer.getMachineList();
    unsigned cargo[4];
    for (;;) {
        yield(g_mprng(1,10));
        if (m_plant.getShipment(cargo)) {
            _Accept(~Truck) {
                break;
            }
        }
        unsigned deliverySize = 0;
        for (unsigned i=0; i<4; ++i) {
            deliverySize += cargo[i];
        }
        m_printer.print(Printer::Truck, 'P', deliverySize);

        for (unsigned i=0; i<m_numVendingMachines && deliverySize > 0; ++i) {
            m_printer.print(Printer::Truck, 'd', i, deliverySize);
            unsigned *inventory = vendingMachines[i]->inventory();
            unsigned undeliveredAmount = 0;
            for (unsigned j=0; j<4 && deliverySize > 0; ++j) {
                unsigned restockAmount;
                if ((m_maxStockPerFlavour - inventory[j]) <= cargo[j]) {
                    restockAmount = m_maxStockPerFlavour - inventory[j];
                } else {
                    restockAmount = cargo[j];
                    undeliveredAmount += m_maxStockPerFlavour - inventory[j] - cargo[j];
                }
                inventory[j] += restockAmount;
                cargo[j] -= restockAmount;
                deliverySize -= restockAmount;
            }
            if (undeliveredAmount > 0) {
                m_printer.print(Printer::Truck, 'U', i, undeliveredAmount);
            }
            m_printer.print(Printer::Truck, 'D', i, deliverySize);
            vendingMachines[i]->restocked();
        }
    }
    m_printer.print(Printer::Truck, 'F');
}
