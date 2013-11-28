#ifndef NONCOPYABLE_H__
#define NONCOPYABLE_H__

class Noncopyable {
private:
    // Not implemented to prevent copying.
    Noncopyable(const Noncopyable&);
    void operator=(const Noncopyable&);
};

#endif
