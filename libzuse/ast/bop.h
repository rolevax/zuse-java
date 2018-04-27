#ifndef ZUSE_BOP_H
#define ZUSE_BOP_H



namespace zuse
{



///
/// \brief Binary operator
///
enum class Bop
{
    UNUSED = -1, DEFAULT = 0,
    ADD = 0, SUB = 1,
    MUL = 0, DIV = 1, MOD = 2,
    DOT = 0, CALL = 1, ARR = 2
};



} // namespace zuse



#endif // ZUSE_BOP_H
