// Yaloe headers
#include "core/doc.h"
#include "ast/parser.h"
#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/tilexmode.h"
#include "mode/identinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/listinputmode.h"
#include "gui/pdoc.h"

// headers for file I/O
#include <fstream>
#include <sstream>

// utilities
#include <stdexcept>
#include <queue>

// headers for debug uses
#include <QDebug>
#include <iostream>
#include <cstdlib>

Doc::Doc(PDoc &dob, TokensObserver &tob)
    : tokens(tob),
      ob(dob)
{
    modes.emplace_back(new ViewMode(*this));
}

void Doc::load(const std::string &filename)
{
    if (modes.size() > 1)
        throw std::runtime_error("Load failed: modifying mode not popped");

    root.reset(yaloe::parse(filename));
    outer = root.get();
    inner = 0;

    tokens.sync(root.get());
    tokens.light(&outer->at(inner));
}

void Doc::save(const std::string &filename)
{
    if (modes.size() > 1)
        throw std::runtime_error("Save failed: modifying mode not popped");

    std::ofstream ofs(filename.c_str());
    ofs << tokens;
    ofs.close();
}

/**
 * @brief Accept a key stroke
 * Handle it with the mode on the top of the mode stack.
 */
void Doc::keyboard(Key key)
{
    assert(modes.size() > 0);

    for (auto rit = modes.rbegin(); rit != modes.rend(); ++rit) {
        Mode::Result res = (*rit)->keyboard(key);
        handleModeResult(res);
        if (res.handled())
            break;
    }

    if (root->size() > 0)
        tokens.light(&outer->at(inner));
}

/**
 * @brief Push 'mode' onto the mode stack,
 * Trigger the 'onPushed' callback of 'mode'
 * This will take away the ownership of 'mode'.
 */
void Doc::push(Mode *mode)
{
    assert(mode != nullptr);

    modes.emplace_back(mode);
    ob.observePush(modes.back()->name());
    Mode::Result res = modes.back()->onPushed();
    handleModeResult(res);
    assert(res.handled());
}

/**
 * @brief Pop the top of the mode stack,
 * @param nextPush the next mode to push
 * Call the 'onPopped' of that mode.
 * Then if nextPush is not null, push that.
 * Else call 'onResume' of the new top mode.
 * Afterwards that mode object will be destructed.
 */
void Doc::pop(Mode *nextPush)
{
    assert(modes.size() > 1); // bottom view mode reserved

    ob.observePop();

    std::unique_ptr<Mode> popped = std::move(modes.back());
    modes.pop_back();
    popped->onPopped();

    if (nextPush != nullptr)
        push(nextPush);
    else
        modes.back()->onResume();
}

void Doc::handleModeResult(const Mode::Result &res)
{
    if (res.toPop()) {
        pop(res.nextPush);
    } else { // stay on the stack
        if (res.nextPush != nullptr)
            push(res.nextPush);
    }
}

const InternalAst &Doc::getOuter() const
{
    return *outer;
}

const Ast &Doc::getInner() const
{
    return outer->at(inner);
}

void Doc::fallIn()
{
    assert(inner < outer->size());

    InternalAst &focus = outer->at(inner).asInternal();
    assert(focus.size() > 0);

    if (focus.isList()) {
        outer = &focus;
        inner = 0;
    } else {
        assert(focus.isFixSize());
        outer = &focus;
        inner = 0;
    }
}

void Doc::digOut()
{
    if (outer == root.get())
        return;

    InternalAst *nextOuter = &outer->getParent();
    inner = nextOuter->indexOf(outer);
    outer = nextOuter;
}

void Doc::sibling(int step)
{
    size_t size = outer->size();
    inner = (ssize_t(inner + size) + step) % size;
}

void Doc::jackKick(bool down)
{
    tokens.jackKick(outer, inner, down);
}

void Doc::hackLead(bool right)
{
    tokens.hackLead(outer, inner, right);
}

void Doc::flyIn(std::function<bool(const Ast *)> &match)
{
    std::queue<Ast*> queue;
    queue.push(&outer->at(inner));

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (match(test)) {
            outer = &test->getParent();
            inner = outer->indexOf(test);
            break;
        } else if (!test->isScalar()) {
            InternalAst &inTest = test->asInternal();
            for (size_t i = 0; i < inTest.size(); i++)
                queue.push(&inTest.at(i));
        }
    }

    // do nothing if not found
}

/**
 * @brief Create and insert a new node at 'current' position
 * @param type The type of the new node
 */
void Doc::insert(Ast::Type type, int bop)
{
    assert(inner <= outer->size());

    Ast *a = newTree(type);

    outer->asList().insert(inner, a);
    if (BopListAst::UNUSED != bop) {
        BopListAst &bast = outer->asBopList();
        setBop(outer->asBopList(), inner, bast.opAt(inner + 1));
        setBop(outer->asBopList(), inner + 1, bop);
    }

    tokens.sync(root.get());
}

void Doc::append(Ast::Type type, int bop)
{
    ++inner;
    assert(inner <= outer->size());

    Ast *a = newTree(type);

    outer->asList().insert(inner, a);
    if (BopListAst::UNUSED != bop)
        setBop(outer->asBopList(), inner, bop);

    tokens.sync(root.get());
}

void Doc::assart(Ast::Type type, int bop)
{
    outer = &outer->at(inner).asInternal();
    inner = 0;

    assert(outer->size() == 0);

    insert(type, bop);
}

void Doc::remove()
{
    assert(inner < outer->size());

    if (outer->isList()) {
        ListAst *l = &outer->asList();
        delete l->remove(inner);

        bool toRemoveSelf = l->illZero();
        bool toExposeChild = l->illOne();

        if (outer->size() == 0)
            digOut();
        else if (inner >= outer->size())
            --inner;

        if (toRemoveSelf)
            remove();
        else if (toExposeChild)
            expose();
    } else {
        change(outer->typeAt(inner));
    }

    tokens.sync(root.get());
}

void Doc::change(Ast::Type type)
{
    assert(inner < outer->size());

    outer->change(inner, newTree(type));
    tokens.sync(root.get());
}

void Doc::nestAsLeft(Ast::Type type, int bop)
{
    assert(inner < outer->size());

    InternalAst *nester = &newTree(type)->asInternal();
    outer->nestAsLeft(inner, nester);

    if (BopListAst::UNUSED != bop)
        setBop(outer->at(inner).asBopList(), 1, bop); // set inner, not outer!

    tokens.sync(root.get());
}

void Doc::expose()
{
    assert(inner < outer->size());

    size_t exposee = inner;
    digOut();
    outer->expose(inner, exposee);

    tokens.sync(root.get());
}

void Doc::cast(Ast::Type type)
{
    switch (type) {
    case Ast::Type::ARG_LIST:
        if (getInner().getType() != Ast::Type::ARG_LIST)
            nestAsLeft(Ast::Type::ARG_LIST, BopListAst::UNUSED);
        return; // nest or do nothing
    case Ast::Type::DECL_METHOD:
        if (getInner().getType() == Ast::Type::DECL_VAR
                && getInner().asInternal().at(1).isScalar()) {
            // from variable declaration to method declaration
            InternalAst *a = &newTree(Ast::Type::DECL_METHOD)->asInternal();
            // copy return type and name
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            outer->change(inner, a);
        }
        break;
    default:
        break;
    }

    tokens.sync(root.get());
}

Mode *Doc::createModifyMode(bool clear)
{
    if (getInner().isList() && !getInner().isBopList())
        return new ListInputMode(*this);

    switch (getInner().getType()) {
    case Ast::Type::META:
        return new TilexMode(*this);
    case Ast::Type::IDENT:
        return new IdentInputMode(*this, clear);
    case Ast::Type::STRING:
        return new StringInputMode(*this, clear);
    case Ast::Type::NUMBER:
        return new NumberInputMode(*this, clear);
    default:
        return nullptr;
    }
}

void Doc::scalarAppend(const char *str)
{
    ScalarAst &scalar = outer->at(inner).asScalar();
    while ('\0' != *str)
        scalar.append(*str++);
    tokens.updateScalar(outer, inner);
}

void Doc::scalarAppend(char c)
{
    outer->at(inner).asScalar().append(c);
    tokens.updateScalar(outer, inner);
}

void Doc::scalarClear()
{
    outer->at(inner).asScalar().clear();
    tokens.updateScalar(outer, inner);
}

void Doc::listClear()
{
    outer->at(inner).asList().clear();
}

void Doc::setHotLight(HotLightLevel level)
{
    switch (level) {
    case HotLightLevel::OFF:
        tokens.setHotLight(-1); // TODO: de-magic
        break;
    case HotLightLevel::POINT: {
        ssize_t back = getInner().getType() == Ast::Type::STRING ? 1 : 0;
        tokens.setHotLight(back);
        break;
    }
    case HotLightLevel::AREA:
        tokens.setHotLight(-2); // TODO: de-magic
        break;
    }
}

void Doc::toggleTension(bool b)
{
    ob.observeTension(b);
}

void Doc::setBop(BopListAst &blist, size_t pos, int bop)
{
    blist.setOpAt(pos, bop);
    if (blist.getType() == Ast::Type::DOT_BOP_LIST
            && bop == BopListAst::CALL) {
        if (blist.at(pos).getType() != Ast::Type::ARG_LIST)
            blist.nestAsLeft(pos, &newTree(Ast::Type::ARG_LIST)->asList());
    }
}

/**
 * Make a minimal non-ill tree
 */
Ast *Doc::newTree(Ast::Type type)
{
    Ast *a = nullptr;

    if (Ast::isBopList(type)) {
        Ast *lhs = newTree(InternalAst::typeAt(type, 0));
        Ast *rhs = newTree(InternalAst::typeAt(type, 1));
        a = new BopListAst(type, lhs, rhs, BopListAst::DEFAULT);
    } else if (Ast::isList(type)) {
        ListAst *la = new ListAst(type);
        a = la;
        if (la->illZero())
            la->append(newTree(la->typeAt(0)));
        if (la->illOne())
            la->append(newTree(la->typeAt(1)));
    } else if (Ast::isFixSize(type)) {
        if (Ast::isFixSize(type, 1)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            a = new FixSizeAst<1>(type, t0);
        } else if (Ast::isFixSize(type, 2)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            Ast *t1 = newTree(InternalAst::typeAt(type, 1));
            a = new FixSizeAst<2>(type, t0, t1);
        } else if (Ast::isFixSize(type, 4)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            Ast *t1 = newTree(InternalAst::typeAt(type, 1));
            Ast *t2 = newTree(InternalAst::typeAt(type, 2));
            Ast *t3 = newTree(InternalAst::typeAt(type, 3));
            a = new FixSizeAst<4>(type, t0, t1, t2, t3);
        } else {
            throw "TODO";
        }
    } else {
        assert(Ast::isScalar(type));

        switch (type) {
        case Ast::Type::IDENT:
            a = new ScalarAst(Ast::Type::IDENT, "id");
            break;
        case Ast::Type::STRING:
            a = new ScalarAst(Ast::Type::STRING, "");
            break;
        case Ast::Type::NUMBER:
            a = new ScalarAst(Ast::Type::NUMBER, "0");
            break;
        case Ast::Type::META:
            a = new ScalarAst(Ast::Type::META, "");
            break;
        default:
            throw "WTF";
        }
    }

    return a;
}


