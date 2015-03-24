#ifndef ITERATOR_H
#define	ITERATOR_H

template <class type>
class Iterator {
public:
    virtual type next() = 0;

    virtual type current() = 0;

    virtual bool has_next() = 0;

    virtual void reset() = 0;

    virtual void close() = 0;
};

#endif

