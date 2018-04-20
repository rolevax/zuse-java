#include "tokens.h"
#include "bonetoken.h"
#include "soultoken.h"

#include <cassert>
#include <cstdlib>

#include <QDebug>
#include <iostream>
#include <sstream>

Tokens::Tokens(DocListener &listener) :
    mHammer(*this),
    mListener(listener)
{
    clear();
}

void Tokens::setHotLight(ssize_t back)
{
    mListener.onHotLight(back);
}

/**
 * @brief Make the displayed cursor point to a node.
 * @param inner the node to point
 */
void Tokens::light(const Ast *inner)
{
    Region in = anchor(locate(inner));

    Ast *outer = &inner->getParent();
    Region out = anchor(locate(outer));

    mListener.onLight(out.br, out.bc, out.er, out.ec,
                    in.br, in.bc, in.er, in.ec);
}

void Tokens::clear()
{
    if (mRows.size() > 1)
        mListener.onLineRemoved(1, mRows.size() - 1);
    mRows.clear();
    mRows.emplace_back();
    mListener.onLineUpdated(0, pluck(0));
}

void Tokens::sync(const ListAst *root)
{
    assert(root->getType() == Ast::Type::CLASS_LIST);
    clear();
    mHammer.hit(*root, 0, 0);
}

/**
 * @brief Apply a scalar change. Assuming it is a one-line change.
 */
void Tokens::updateScalar(const InternalAst *outer, size_t inner)
{
    assert(outer->at(inner).isScalar());
    Region r = locate(&outer->at(inner));

    mListener.onLineUpdated(r.br, pluck(r.br));
}

/**
 * @brief print a row to the returned string
 */
std::string Tokens::pluck(size_t r)
{
    assert(r < mRows.size());

    std::string ret;
    for (const std::unique_ptr<Token> &t : mRows[r])
        ret += t->getText();

    return ret;
}

/**
 * @brief Vertical concrete cursor moving
 */
void Tokens::jackKick(InternalAst *&outer, size_t &inner, bool down)
{
    Region r = locate(&outer->at(inner));

    // preferred target column index
    size_t sugg;
    if (r.br == r.er) {
        Region anc = anchor(r);
        sugg = (anc.bc + anc.ec) / 2;
    } else {
        sugg = 0;
    }

    for (ssize_t i = down ? r.er + 1 : r.br - 1;
         down ? i < ssize_t(mRows.size()) : i >= 0;
         down ? i++ : i--) {
        std::vector<size_t> fleshes;
        for (size_t j = 0; j < mRows[i].size(); j++)
            if (isHjklTarget(mRows[i][j]->getAst()))
                fleshes.push_back(j);

        if (!fleshes.empty()) {
            auto evalDiff = [this, i, sugg](size_t j)
            {
                size_t left = anchor(i, j);
                size_t right = left + mRows[i][j]->getText().size();
                size_t mid = (left + right) / 2;
                return std::abs(ssize_t(mid) - ssize_t(sugg));
            };

            size_t tar = fleshes[0];
            size_t diff = evalDiff(tar);

            for (auto it = fleshes.begin() + 1; it != fleshes.end(); ++it) {
                size_t j = *it;
                size_t nextDiff = evalDiff(j);
                if (nextDiff < diff) {
                    diff = nextDiff;
                    tar = j;
                }
            }

            const Ast *a = mRows[i][tar]->getAst();
            outer = &a->getParent();
            inner = outer->indexOf(a);

            break; // big loop
        } // end if
    } // end for
}

/**
 * @brief Horizontal concrete cursor moving
 */
void Tokens::hackLead(InternalAst *&outer, size_t &inner, bool right)
{
    Region r = locate(&outer->at(inner));
    // flesh token cannot have column index 0 since it's
    // surrounded by soul tokens.
    // this condition is also for preventing 'i' in the loop
    // from overflow.
    if (r.br != r.er || r.bc == 0)
        return;

    for (size_t i = right ? r.ec + 1 : r.bc - 1;
         right ? i < mRows[r.br].size() : i > 0; right ? i++ : i--) {
        if (isHjklTarget(mRows[r.br][i]->getAst())) {
            const Ast *a = mRows[r.br][i]->getAst();
            outer = &a->getParent();
            inner = outer->indexOf(a);
            break;
        }
    }

    // do nothing if not found
}

void Tokens::put(size_t r, size_t c, const std::vector<Token *> &ts)
{
    size_t origR = r;

    for (Token *t : ts) {
        if (t == nullptr) {
            newLine(r, c);
            ++r;
            c = 0;
        } else {
            auto &row = mRows[r];
            row.emplace(row.begin() + c, t);
            ++c;
        }
    }

    if (r > origR)
        mListener.onLineInserted(origR, r - origR);

    while (origR <= r) {
        mListener.onLineUpdated(origR, pluck(origR));
        ++origR;
    }
}

/**
 * @brief Tokens::erase
 * @param r
 * Remove all tokens from the region 'r'
 */
void Tokens::erase(const Region &r)
{
    assert(r.br < mRows.size() && r.bc < mRows[r.br].size());
    assert(r.er < mRows.size() && r.ec < mRows[r.er].size());
    assert(r.br <= r.er);

    int needJoin = 0;

    if (r.br == r.er) { // one-line erase
        /*
        needJoin += rows[r.er][r.ec]->needEnter();
        auto it = rows[r.br].begin();
        rows[r.br].erase(it + r.bc, it + r.ec + 1);
        */
    } else { // multi-line erase
        /*
        needJoin += rows[r.br].back()->needEnter(); // first line
        */

        /* bottom-up remove from the last row
         * since it won't mess up the row indices
         */

        // remove inside the last line
        /*
        needJoin += rows[r.er][r.ec]->needEnter();
        auto endIt = rows[r.er].begin();
        rows[r.er].erase(endIt, endIt + r.ec + 1);

        if (r.br + 1 < r.er) { // remove internal lines [br+1,er-1]
            rows.erase(rows.begin() + r.br + 1, rows.begin() + r.er);
            ob.observeRemoveLine(r.br + 1, r.er - (r.br + 1));
        }

        // remove inside the first line
        rows[r.br].erase(rows[r.br].begin() + r.bc, rows[r.br].end());
        */
    }


    while (needJoin --> 0)
        joinLine(r.br + 1);

    mListener.onLineUpdated(r.br, pluck(r.br));
}

bool Tokens::isHjklTarget(const Ast *a)
{
    return a->isScalar() && a->getType() != Ast::Type::HIDDEN;
}

/**
 * @brief Output in syntatically correct plain text
 */
std::ostream &operator<<(std::ostream &os, const Tokens &ts)
{
    for (const auto &row : ts.mRows) {
        for (const auto &t : row)
            os << t->getText();
        os << std::endl;
    }

    return os;
}

/**
 * @brief Find the region of tokens enclosing the specified Ast
 * @param tar The ast node to locate
 * @return The region
 */
Region Tokens::locate(const Ast *tar)
{
    Region res;
    bool found = false; // debug use

    for (auto rit = mRows.begin(); rit != mRows.end(); ++rit) {
        for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
            const Ast *a = (*cit)->getAst();
            if (a == tar) {
                Token::Role role = (*cit)->getRole();
                if (role == Token::Role::BEGIN) {
                    res.br = rit - mRows.begin();
                    res.bc = cit - rit->begin();
                } else if (role == Token::Role::END) {
                    res.er = rit - mRows.begin();
                    res.ec = cit - rit->begin();
                    found = true;
                    break;
                }
            }
        }
    }

    assert(found);
    return res;
}

/**
 * @brief Include a comman that should be removed together
 * 		  with the region.
 * @param r region to remove
 * Assume that comma is adjacent to the "group",
 * including prefix tab indent.
 * @deprecated
 */
void Tokens::suckComma(Region &r)
{
    const Ast *in = mRows[r.br][r.bc]->getAst();
    const InternalAst &par = in->getParent();
    if (par.size() > 1) {
        if (par.indexOf(in) == par.size() - 1) {
            /* very end of a non-single-element list
               then include the previous comma,
               which is assumed to be the end of the previous row. */
            if (mRows[r.br - 1].back()->getAst() == &par) {
                --r.br;
                r.bc = mRows[r.br].size() - 1;
            }
        } else { // first or internel list element
            if (mRows[r.er][r.ec + 1]->getAst() == &par)
                ++r.ec; // include the following comma
        }
    }
}

void Tokens::newLine(size_t r, size_t c)
{
    assert(r < mRows.size() && c <= mRows[r].size());
    mRows.emplace(mRows.begin() + r + 1);
    auto &newRow = mRows[r + 1];
    newRow.insert(newRow.end(),
                  std::make_move_iterator(mRows[r].begin() + c),
                  std::make_move_iterator(mRows[r].end()));
    mRows[r].erase(mRows[r].begin() + c, mRows[r].end());
}

/**
 * @brief Join the r-th line into the (r-1)-th line
 * @param r
 */
void Tokens::joinLine(size_t r)
{
    assert(r > 0 && r < mRows.size());
    auto &prevRow = mRows[r - 1];
    prevRow.insert(prevRow.end(),
                   std::make_move_iterator(mRows[r].begin()),
                   std::make_move_iterator(mRows[r].end()));
    mRows.erase(mRows.begin() + r);
}

/**
 * @brief Convert token level index into character level index
 * @param r row of token
 * @param c column of token
 * @return column index in terms of character
 * "How many characters are there before the token?"
 */
size_t Tokens::anchor(size_t r, size_t c)
{
    assert(r < mRows.size() && c <= mRows[r].size());
    size_t offset = 0;
    for (size_t i = 0; i < c; i++)
        offset += mRows[r][i]->getText().size();
    return offset;
}

Region Tokens::anchor(const Region &r)
{
   Region c(r);
   c.bc = anchor(c.br, c.bc);
   if (r.br != r.er && r.ec == 0) {
       c.er = r.er - 1;
       c.ec = anchor(r.er - 1, mRows[r.er - 1].size());
   } else {
       c.ec = anchor(c.er, c.ec + 1);
   }
   return c;
}

