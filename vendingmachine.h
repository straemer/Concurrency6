#ifndef VENDING_MACHINES_H__
#define VENDING_MACHINES_H__

_Task VendingMachine {
public:
    enum Flavours { ... };                 // flavours of soda (YOU DEFINE)
    enum Status {
        BUY, // Successful Buy
        STOCK, // Out of stock
        FUNDS // insufficient funds
    };
    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                   unsigned int maxStockPerFlavour);
    Status buy(Flavours flavour, WATCard &card);
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
private:
    void main();
};

#endif
