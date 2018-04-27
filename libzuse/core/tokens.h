#ifndef ZUSE_TOKENS_H
#define ZUSE_TOKENS_H

#include "hammer.h"
#include "token.h"
#include "doc_listener.h"

#include <vector>



namespace zuse
{



///
/// \brief All tokens of a source file
///
class Tokens
{
public:
    Tokens(DocListener &listener);

    Tokens(const Tokens&) = delete;
    Tokens &operator=(const Tokens&) = delete;

    void setHotLight(ssize_t back);
    void light(const Ast *inner);
    void clear();
    void sync(const AstList *root);
    void updateScalar(const AstInternal *outer, size_t inner);
    std::string pluck(size_t r);
    void jackKick(AstInternal *&outer, size_t &inner, bool down);
    void hackLead(AstInternal *&outer, size_t &inner, bool right);

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
    DocListener &mListener;
};

std::ostream &operator<<(std::ostream &os, const Tokens &ts);



} // namespace zuse



#endif // ZUSE_TOKENS_H
