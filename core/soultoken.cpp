#include "core/soultoken.h"

#include <cassert>

SoulToken::SoulToken(const Ast *ast, Role role) :
    Token(ast, role)
{
    assert(role == Role::BEGIN || role == Role::END || role == Role::META);
}

const std::string &SoulToken::getText() const
{
    static const std::string tab[4] = {
        "", "    ", "        ", "            "
    };

    if (getRole() != Role::BEGIN)
        return tab[0];

    return tab[getAst()->indentLevel()];
}

