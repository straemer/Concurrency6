#ifndef STUDENT_H__
#define STUDENT_H__

_Task Student {
public:
    Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
            unsigned int maxPurchases);
private:
    void main();
};

#endif
