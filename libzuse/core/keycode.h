#ifndef ZUSE_KEYCODE_H
#define ZUSE_KEYCODE_H

enum class Key
{
    BACKTICK, NUM1, NUM2, NUM3, NUM4, NUM5,
    NUM6, NUM7, NUM8, NUM9, NUM0, MINUS, EQUAL, BACKSPACE,
    TAB, LEFT_SQUARE, RIGHT_SQUARE, BACKSLASH,
    SEMICOLON, SINGLE_QUOTE, ENTER,
    LEFT_SHIFT, COMMA, DOT, SLASH, RIGHT_SHIFT, SPACE,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N,
    O, P, Q, R, S, T, U, V, W, X, Y, Z,

    TIDE, EXCLAM, AT, HASH, DOLLAR, PERCENT,
    CARET, AND, ASTERISK, LEFT_PAREN, RIGHT_PAREN, UNDERSCORE, PLUS, S_BACKSPACE,
    S_TAB, LEFT_BRACE, RIGHT_BRACE, PIPE,
    COLON, DOUBLE_QUOTE, S_ENTER,
    S_SHIFT, LESS, GREATER, QUESTION, S_SHIFT2, S_SPACE,
    S_A, S_B, S_C, S_D, S_E, S_F, S_G, S_H, S_I, S_J, S_K, S_L, S_M, S_N,
    S_O, S_P, S_Q, S_R, S_S, S_T, S_U, S_V, S_W, S_X, S_Y, S_Z,

    OTHER
};

namespace KeyCode
{

Key makeShifted(Key k);
bool isAlpha(Key k);
bool isDigit(Key k);
char toChar(Key k);
Key fromChar(char c);

}

#endif // ZUSE_KEYCODE_H
