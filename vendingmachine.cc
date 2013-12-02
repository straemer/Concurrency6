#include "vendingmachine.h"

#include "watcard.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id,
                               unsigned int sodaCost, unsigned int maxStockPerFlavour) :
    m_printer(prt),
    m_nameServer(nameServer),
    m_id(id),
    m_cost(sodaCost),
    m_restocking(false)
{
    for (unsigned i=0; i<4; ++i) {
        m_inventory[i] = 0;
    }
}

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card) {
    assert((unsigned)flavour < 4);
    if (m_inventory[flavour] == 0) {
        return STOCK;
    }
    if (card.getBalance() < m_cost) {
        return FUNDS;
    }
    card.withdraw(m_cost);
    --m_inventory[flavour];
    return BUY;
}

unsigned int *VendingMachine::inventory() {
    m_restocking = true;
    return m_inventory;
}

void VendingMachine::restocked() {
    m_restocking = false;
}

void VendingMachine::main() {
    for (;;) {
        _Accept(~VendingMachine) {
            break;
        } or _When (m_restocking) _Accept(restocked) {
        } or _When (!m_restocking) _Accept(buy, inventory) {
        }
    }
}
