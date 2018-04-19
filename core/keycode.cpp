#include "keycode.h"

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
    case Key::BACKTICK:
        return '`';
    case Key::MINUS:
        return '-';
    case Key::EQUAL:
        return '=';
    case Key::BACKSPACE:
        return '\b';
    case Key::TAB:
        return '\t';
    case Key::LEFT_SQUARE:
        return '[';
    case Key::RIGHT_SQUARE:
        return ']';
    case Key::BACKSLASH:
        return '\\';
    case Key::SEMICOLON:
        return ';';
    case Key::SINGLE_QUOTE:
        return '\'';
    case Key::COMMA:
        return ',';
    case Key::DOT:
        return '.';
    case Key::SLASH:
        return '/';
    case Key::SPACE:
        return ' ';
    case Key::TIDE:
        return '~';
    case Key::EXCLAM:
        return '!';
    case Key::AT:
        return '@';
    case Key::HASH:
        return '#';
    case Key::DOLLAR:
        return '$';
    case Key::PERCENT:
        return '%';
    case Key::CARET:
        return '^';
    case Key::AND:
        return '&';
    case Key::ASTERISK:
        return '*';
    case Key::LEFT_PAREN:
        return '(';
    case Key::RIGHT_PAREN:
        return ')';
    case Key::UNDERSCORE:
        return '_';
    case Key::PLUS:
        return '+';
    case Key::LEFT_BRACE:
        return '{';
    case Key::RIGHT_BRACE:
        return '}';
    case Key::PIPE:
        return '|';
    case Key::COLON:
        return ':';
    case Key::DOUBLE_QUOTE:
        return '"';
    case Key::LESS:
        return '<';
    case Key::GREATER:
        return '>';
    case Key::QUESTION:
        return '?';


    case Key::ENTER:
    case Key::LEFT_SHIFT:
    case Key::RIGHT_SHIFT:
    case Key::S_BACKSPACE:
    case Key::S_TAB:
    case Key::S_ENTER:
    case Key::S_SHIFT:
    case Key::S_SHIFT2:
    case Key::S_SPACE:
        return '\a';
    default:
        throw "WTF";
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
    case '`':
        return Key::BACKTICK;
    case '-':
        return Key::MINUS;
    case '=':
        return Key::EQUAL;
    case '\b':
        return Key::BACKSPACE;
    case '\t':
        return Key::TAB;
    case '[':
        return Key::LEFT_SQUARE;
    case ']':
        return Key::RIGHT_SQUARE;
    case '\\':
        return Key::BACKSLASH;
    case ';':
        return Key::SEMICOLON;
    case '\'':
        return Key::SINGLE_QUOTE;
    case ',':
        return Key::COMMA;
    case '.':
        return Key::DOT;
    case '/':
        return Key::SLASH;
    case ' ':
        return Key::SPACE;
    case '~':
        return Key::TIDE;
    case '!':
        return Key::EXCLAM;
    case '@':
        return Key::AT;
    case '#':
        return Key::HASH;
    case '$':
        return Key::DOLLAR;
    case '%':
        return Key::PERCENT;
    case '^':
        return Key::CARET;
    case '&':
        return Key::AND;
    case '*':
        return Key::ASTERISK;
    case '(':
        return Key::LEFT_PAREN;
    case ')':
        return Key::RIGHT_PAREN;
    case '_':
        return Key::UNDERSCORE;
    case '+':
        return Key::PLUS;
    case '{':
        return Key::LEFT_BRACE;
    case '}':
        return Key::RIGHT_BRACE;
    case '|':
        return Key::PIPE;
    case ':':
        return Key::COLON;
    case '"':
        return Key::DOUBLE_QUOTE;
    case '<':
        return Key::LESS;
    case '>':
        return Key::GREATER;
    case '?':
        return Key::QUESTION;
    case '\n':
    case '\r':
        return Key::ENTER;

    default:
        return Key::OTHER;
    }
}

}
