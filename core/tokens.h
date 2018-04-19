#ifndef TOKENS_H
#define TOKENS_H

#include "hammer.h"
#include "token.h"
#include "tokensobserver.h"

#include <vector>



class Tokens
{
public:
    Tokens(TokensObserver &mOb);

    Tokens(const Tokens&) = delete;
    Tokens &operator=(const Tokens&) = delete;

    void setHotLight(ssize_t back);
    void light(const Ast *inner);
    void clear();
    void sync(const ListAst *root);
    void updateScalar(const InternalAst *outer, size_t inner);
    std::string pluck(size_t r);
    void jackKick(InternalAst *&outer, size_t &inner, bool down);
    void hackLead(InternalAst *&outer, size_t &inner, bool right);

    /// @name Hammer's Interface
    ///@{
    void put(size_t r, size_t c, const std::vector<Token*> &ts);
    void erase(const Region &r);
    ///@}

    friend std::ostream &operator<<(std::ostream &os, const Tokens &ts);

private:
    static bool isHjklTarget(const Ast *a);
    Region locate(const Ast *tar);
    void suckComma(Region &region);
    void newLine(size_t r, size_t c);
    void joinLine(size_t r);
    size_t anchor(size_t r, size_t c);
    Region anchor(const Region &r);

private:
    std::vector<std::vector<std::unique_ptr<Token>>> mRows;
    Hammer mHammer;
    TokensObserver &mOb;
};

std::ostream &operator<<(std::ostream &os, const Tokens &ts);

#endif // TOKENS_H
