#ifndef VENDING_MACHINES_H__
#define VENDING_MACHINES_H__

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
public:
    enum Flavours { };                 // flavours of soda (YOU DEFINE)
    enum Status {
        BUY, // Successful Buy
        STOCK, // Out of stock
        FUNDS // insufficient funds
    };
    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                   unsigned int maxStockPerFlavour);
    Status buy(Flavours flavour, WATCard &card);
    // inventory - return the inventory of the vending machine, and prevent buy() from being called
    // Pretty much the worst API ever.
    unsigned int *inventory();
    void restocked();
    inline _Nomutex unsigned int cost() { return m_cost; }
    inline _Nomutex unsigned int getId() { return m_id; }
private:
    void main();

    unsigned m_inventory[4];

    Printer &m_printer;
    NameServer &m_nameServer;
    const unsigned m_id;
    const unsigned m_cost;
    bool m_restocking;
};

#endif
