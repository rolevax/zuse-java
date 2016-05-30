// Zuse headers
#include "core/doc.h"
#include "ast/parser.h"
#include "mode/normalmode.h"
#include "mode/menumode.h"
#include "mode/tilexmode.h"
#include "mode/identinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/listinputmode.h"
#include "mode/fixsizeinputmode.h"
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
    modes.emplace_back(new NormalMode(*this));
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

    for (int i = modes.size() - 1; i >= 0; i--) {
        assert(i < int(modes.size()));
        Mode::Result res = modes[i]->keyboard(key);
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
    if (mode == nullptr)
        return;

    modes.emplace_back(mode);
    ob.observePush(modes.back()->name());
    Mode::Result res = modes.back()->onPushed();
    handleModeResult(res);
    assert(res.handled());
}

/**
 * @brief Pop the top of the mode stack,
 * @param nextPush the next mode to push
 */
void Doc::pop()
{
    assert(modes.size() > 1); // bottom normal mode reserved

    ob.observePop();

    std::unique_ptr<Mode> popped = std::move(modes.back());
    modes.pop_back();
    popped->onPopped();
}

void Doc::handleModeResult(const Mode::Result &res)
{
    if (res.toPop()) {
        pop();
        push(res.nextPush);
        if (res.type == Mode::ResultType::DONE_POP) {
            Mode::Result r = modes.back()->onResume();
            handleModeResult(r);
        }
    } else { // stay on the stack
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

size_t Doc::getInnerIndex() const
{
    return inner;
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

void Doc::sibling(int step, bool skipHidden)
{
    size_t size = outer->size();
    if (skipHidden) {
        if (step == 0 || outer->size() < 2)
            return;

        // assume there is no internal node whose children are all hidden
        // such that this loop always terminate
        int dir = step > 0 ? 1 : -1;
        while (step != 0) {
            inner = (inner + dir) % size;
            if (getInner().getType() != Ast::Type::HIDDEN)
                step -= dir;
        }
    } else {
        inner = (ssize_t(inner + size) + step) % size;
    }
}

void Doc::jackKick(bool down)
{
    tokens.jackKick(outer, inner, down);
}

void Doc::hackLead(bool right)
{
    tokens.hackLead(outer, inner, right);
}

void Doc::focusInBig(bool match(const Ast*))
{
    std::queue<Ast*> queue;
    queue.push(&outer->at(inner));

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (match(test) && test != &outer->at(inner)) { // found
            outer = &test->getParent();
            inner = outer->indexOf(test);
            break;
        } else if (!test->isScalar()) { // keep searching
            InternalAst &inTest = test->asInternal();
            for (size_t i = 0; i < inTest.size(); i++)
                queue.push(&inTest.at(i));
        }
    }

    // do nothing if not found
}

void Doc::focusInBig(Ast::Type match)
{
    std::queue<Ast*> queue;
    queue.push(&outer->at(inner));

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (test->getType() == match && test != &outer->at(inner)) { // found
            outer = &test->getParent();
            inner = outer->indexOf(test);
            break;
        } else if (!test->isScalar()) { // keep searching
            InternalAst &inTest = test->asInternal();
            for (size_t i = 0; i < inTest.size(); i++)
                queue.push(&inTest.at(i));
        }
    }

    // do nothing if not found
}

void Doc::dollyOutBig(bool match(const Ast *))
{
    const Ast *a = outer;
    while (a->getType() != Ast::Type::CLASS_LIST) {
        if (match(a)) {
            outer = &a->getParent();
            inner = outer->indexOf(a);
            return;
        }
        a = &a->getParent();
    }
}

void Doc::dollyOutBig(Ast::Type match)
{
    const Ast *a = outer;

    while (a->getType() != Ast::Type::CLASS_LIST) {
        if (a->getType() == match) {
            outer = &a->getParent();
            inner = outer->indexOf(a);
            return;
        }
        a = &a->getParent();
    }
}

void Doc::siblingBig(bool match(const Ast *), bool right)
{
    if (inner == 0 && !right)
        return; // no elder sibling (ensure 'inner-1' not overflow)

    std::queue<Ast*> queue;
    if (right) {
        for (size_t i = inner + 1; i < outer->size(); i++)
            queue.push(&outer->at(i));
    } else {
        for (ssize_t i = inner - 1; i >= 0; i--)
            queue.push(&outer->at(i));
    }

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (match(test) && test != &outer->at(inner)) { // found
            outer = &test->getParent();
            inner = outer->indexOf(test);
            break;
        } else if (!test->isScalar()) { // keep searching
            InternalAst &inTest = test->asInternal();
            if (right) {
                for (size_t i = 0; i < inTest.size(); i++)
                    queue.push(&inTest.at(i));
            } else {
                for (ssize_t i = inTest.size() - 1; i >= 0; i--)
                    queue.push(&inTest.at(i));
            }
        }
    }

    // do nothing if not found
}

void Doc::siblingBig(Ast::Type match, bool right)
{
    if (inner == 0 && !right)
        return; // no elder sibling (ensure 'inner-1' not overflow)

    std::queue<Ast*> queue;
    if (right) {
        for (size_t i = inner + 1; i < outer->size(); i++)
            queue.push(&outer->at(i));
    } else {
        for (ssize_t i = inner - 1; i >= 0; i--)
            queue.push(&outer->at(i));
    }


    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (test->getType() == match && test != &outer->at(inner)) { // found
            outer = &test->getParent();
            inner = outer->indexOf(test);
            break;
        } else if (!test->isScalar()) { // keep searching
            InternalAst &inTest = test->asInternal();
            if (right) {
                for (size_t i = 0; i < inTest.size(); i++)
                    queue.push(&inTest.at(i));
            } else {
                for (ssize_t i = inTest.size() - 1; i >= 0; i--)
                    queue.push(&inTest.at(i));
            }
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

    // only handle list ill-one and ill-zero
    // does not care about hidden node issues.
    // hidden node issues are done in normal mode
    if (outer->isList()) {
        ListAst *l = &outer->asList();
        l->erase(inner);

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

void Doc::change(Ast *a)
{
    assert(inner < outer->size());

    outer->change(inner, a);
    tokens.sync(root.get());
}

void Doc::change(Ast::Type type)
{
    change(newTree(type));
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

void Doc::nestAsRight(Ast::Type type, int bop)
{
    assert(inner < outer->size());

    InternalAst *nester = &newTree(type)->asInternal();
    outer->nestAsRight(inner, nester);

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

void Doc::cast(Ast::Type to)
{
    using Type = Ast::Type;
    Type from = getInner().getType();

    if (from == to)
        return;

    if (to == Type::ARG_LIST) {
        nestAsLeft(Type::ARG_LIST, BopListAst::UNUSED);
    } else if (from == Type::DECL_VAR && to == Type::DECL_METHOD) {
        if (getInner().asInternal().at(1).isScalar()) {
            // from variable declaration to method declaration
            InternalAst *a = &newTree(Type::DECL_METHOD)->asInternal();
            // copy return type and name
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            outer->change(inner, a);
        }
    } else if (Ast::isFixSize(from, 2)
                || (Ast::isBopList(from) && getInner().asBopList().size() == 2)) {
        // size 2 ==> size 2
        if (Ast::isFixSize(to, 2) || Ast::isBopList(to)) {
            // simply copy children and change
            InternalAst *a = &newTree(to)->asInternal();
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            outer->change(inner, a);
        }
    }

    tokens.sync(root.get());
}

Mode *Doc::createModifyMode(bool clear, size_t offset, bool macroContext)
{
    if (getInner().isList() && !getInner().isBopList())
        return new ListInputMode(*this);
    else if (getInner().isFixSize())
        return new FixSizeInputMode(*this, getInner().asInternal(), offset);

    switch (getInner().getType()) {
    case Ast::Type::META:
        return new TilexMode(*this, macroContext);
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

void Doc::toggleAbstract()
{
    FixSizes::getModifiers(outer->at(inner)).abstract ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleFinal()
{
    FixSizes::getModifiers(outer->at(inner)).final ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleAccess(bool up)
{
    // circulate by guarenteed 2-bit overflow
    if (up)
        FixSizes::getModifiers(outer->at(inner)).access++;
    else
        FixSizes::getModifiers(outer->at(inner)).access--;
    tokens.sync(root.get());
}

void Doc::toggleStatic()
{
    FixSizes::getModifiers(outer->at(inner)).statik ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleTransient()
{
    FixSizes::getModifiers(outer->at(inner)).transient ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleVolatile()
{
    FixSizes::getModifiers(outer->at(inner)).voladile ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleNative()
{
    FixSizes::getModifiers(outer->at(inner)).native ^= 1;
    tokens.sync(root.get());
}

void Doc::toggleSynchronized()
{
    FixSizes::getModifiers(outer->at(inner)).synchronized ^= 1;
    tokens.sync(root.get());
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
        } else if (Ast::isFixSize(type, 3)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            Ast *t1 = newTree(InternalAst::typeAt(type, 1));
            Ast *t2 = newTree(InternalAst::typeAt(type, 2));
            a = new FixSizeAst<3>(type, t0, t1, t2);
        } else if (Ast::isFixSize(type, 4)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            Ast *t1 = newTree(InternalAst::typeAt(type, 1));
            Ast *t2 = newTree(InternalAst::typeAt(type, 2));
            Ast *t3 = newTree(InternalAst::typeAt(type, 3));
            a = new FixSizeAst<4>(type, t0, t1, t2, t3);
        } else if (Ast::isFixSize(type, 5)) {
            Ast *t0 = newTree(InternalAst::typeAt(type, 0));
            Ast *t1 = newTree(InternalAst::typeAt(type, 1));
            Ast *t2 = newTree(InternalAst::typeAt(type, 2));
            Ast *t3 = newTree(InternalAst::typeAt(type, 3));
            Ast *t4 = newTree(InternalAst::typeAt(type, 4));
            a = new FixSizeAst<5>(type, t0, t1, t2, t3, t4);
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
        case Ast::Type::HIDDEN:
            a = new ScalarAst(Ast::Type::HIDDEN, "");
            break;
        default:
            throw "WTF";
        }
    }

    return a;
}


