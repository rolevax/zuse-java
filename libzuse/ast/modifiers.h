#ifndef ZUSE_MODIFIERS_H
#define ZUSE_MODIFIERS_H

#include <ostream>

namespace zuse
{



struct Modifiers
{
    unsigned access : 2;
    bool abstract : 1;
    bool final : 1;
    bool statik : 1;
    bool transient : 1;
    bool voladile : 1;
    bool native : 1;
    bool synchronized : 1;

    static const unsigned DEFAULT = 0, PUBLIC = 1, PROTECTED = 2, PRIVATE = 3;

    Modifiers()
        : access(DEFAULT)
        , abstract(false)
        , final(false)
        , statik(false)
        , transient(false)
        , voladile(false)
        , native(false)
        , synchronized(false)
    {
    }

    Modifiers &operator=(const Modifiers &rhs) = default;

    Modifiers &operator|=(Modifiers rhs)
    {
        access |= rhs.access;
        abstract |= rhs.abstract;
        final |= rhs.final;
        statik |= rhs.statik;
        transient |= rhs.transient;
        voladile |= rhs.voladile;
        native |= rhs.native;
        synchronized |= rhs.synchronized;

        return *this;
    }

    explicit operator unsigned()
    {
        unsigned res = access;
        res <<= 2;
        res |= abstract;
        res <<= 1;
        res |= final;
        res <<= 1;
        res |= statik;
        res <<= 1;
        res |= transient;
        res <<= 1;
        res |= voladile;
        res <<= 1;
        res |= native;
        res <<= 1;
        res |= synchronized;
        return res;
    }

    // it's useless, just to feed bison/flex
    friend std::ostream &operator<<(std::ostream &os, Modifiers m);
};

inline std::ostream &operator<<(std::ostream &os, Modifiers m)
{
    return os << unsigned(m);
}



} // namespace zuse



#endif // ZUSE_MODIFIERS_H
