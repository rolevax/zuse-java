#include "core/keycode.h"

namespace KeyCode
{

Key makeShifted(Key k)
{
    return k < Key::TIDE ? Key(int(Key::TIDE) + (int(k) - int(Key::BACKTICK))) : k;
}

bool isAlpha(Key k)
{
    return (Key::A <= k && k <= Key::Z) || (Key::S_A <= k && k <= Key::S_Z);
}

bool isDigit(Key k)
{
    return Key::NUM1 <= k && k <= Key::NUM0;
}

char toChar(Key k)
{
    if (Key::A <= k && k <= Key::Z)
        return 'a' + (int(k) - int(Key::A));
    else if (Key::S_A <= k && k <= Key::S_Z)
        return 'A' + (int(k) - int(Key::S_A));
    else if (k == Key::NUM0)
        return '0';
    else if (Key::NUM1 <= k && k <= Key::NUM9)
        return '1' + (int(k) - int(Key::NUM1));

    switch (k) {
    // TODO
    default:
        return 'X';
    }
}

Key fromChar(char c)
{
    if ('a' <= c && c <= 'z')
        return Key(int(Key::A) + (c - 'a'));
    else if ('A' <= c && c <= 'Z')
        return Key(int(Key::S_A) + (c - 'A'));
    else if ('0' == c)
        return Key::NUM0;
    else if ('1' <= c && c <= '9')
        return Key(int(Key::NUM1) + (c - '1'));

    switch (c) {
    // TODO
    default:
        return Key::OTHER;
    }
}

}
