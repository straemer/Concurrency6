#ifndef PARENT_H__
#define PARENT_H__

_Monitor Printer;
_Monitor Bank;

_Task Parent {
public:
    Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay);
private:
    void main();
};

#endif
