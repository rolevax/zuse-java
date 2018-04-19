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
    : mTokens(tob),
      mOb(dob)
{
    mModes.emplace_back(new NormalMode(*this));
}

void Doc::load(const std::string &filename)
{
    if (mModes.size() > 1)
        throw std::runtime_error("Load failed: modifying mode not popped");

    mRoot.reset(yaloe::parse(filename));
    mOuter = mRoot.get();
    mInner = 0;

    mTokens.sync(mRoot.get());
    mTokens.light(&mOuter->at(mInner));
}

void Doc::save(const std::string &filename)
{
    if (mModes.size() > 1)
        throw std::runtime_error("Save failed: modifying mode not popped");

    std::ofstream ofs(filename.c_str());
    ofs << mTokens;
    ofs.close();
}

/**
 * @brief Accept a key stroke
 * Handle it with the mode on the top of the mode stack.
 */
void Doc::keyboard(Key key)
{
    assert(mModes.size() > 0);

    for (int i = mModes.size() - 1; i >= 0; i--) {
        assert(i < int(mModes.size()));
        Mode::Result res = mModes[i]->keyboard(key);
        handleModeResult(res);
        if (res.handled())
            break;
    }

    if (mRoot->size() > 0)
        mTokens.light(&mOuter->at(mInner));
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

    mModes.emplace_back(mode);
    mOb.observePush(mModes.back()->name());
    Mode::Result res = mModes.back()->onPushed();
    handleModeResult(res);
    assert(res.handled());
}

/**
 * @brief Pop the top of the mode stack,
 * @param nextPush the next mode to push
 */
void Doc::pop()
{
    assert(mModes.size() > 1); // bottom normal mode reserved

    mOb.observePop();

    std::unique_ptr<Mode> popped = std::move(mModes.back());
    mModes.pop_back();
    popped->onPopped();
}

void Doc::handleModeResult(const Mode::Result &res)
{
    if (res.toPop()) {
        pop();
        push(res.nextPush);
        if (res.type == Mode::ResultType::DONE_POP) {
            Mode::Result r = mModes.back()->onResume();
            handleModeResult(r);
        }
    } else { // stay on the stack
        push(res.nextPush);
    }
}

const InternalAst &Doc::getOuter() const
{
    return *mOuter;
}

const Ast &Doc::getInner() const
{
    return mOuter->at(mInner);
}

size_t Doc::getInnerIndex() const
{
    return mInner;
}

void Doc::fallIn()
{
    assert(mInner < mOuter->size());

    InternalAst &focus = mOuter->at(mInner).asInternal();
    assert(focus.size() > 0);

    if (focus.isList()) {
        mOuter = &focus;
        mInner = 0;
    } else {
        assert(focus.isFixSize());
        mOuter = &focus;
        mInner = 0;
    }
}

void Doc::digOut()
{
    if (mOuter == mRoot.get())
        return;

    InternalAst *nextOuter = &mOuter->getParent();
    mInner = nextOuter->indexOf(mOuter);
    mOuter = nextOuter;
}

void Doc::sibling(int step, bool skipHidden)
{
    size_t size = mOuter->size();
    if (skipHidden) {
        if (step == 0 || mOuter->size() < 2)
            return;

        // assume there is no internal node whose children are all hidden
        // such that this loop always terminate
        int dir = step > 0 ? 1 : -1;
        while (step != 0) {
            mInner = (mInner + dir) % size;
            if (getInner().getType() != Ast::Type::HIDDEN)
                step -= dir;
        }
    } else {
        mInner = (ssize_t(mInner + size) + step) % size;
    }
}

void Doc::jackKick(bool down)
{
    mTokens.jackKick(mOuter, mInner, down);
}

void Doc::hackLead(bool right)
{
    mTokens.hackLead(mOuter, mInner, right);
}

void Doc::focusInBig(bool match(const Ast*))
{
    std::queue<Ast*> queue;
    queue.push(&mOuter->at(mInner));

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (match(test) && test != &mOuter->at(mInner)) { // found
            mOuter = &test->getParent();
            mInner = mOuter->indexOf(test);
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
    queue.push(&mOuter->at(mInner));

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (test->getType() == match && test != &mOuter->at(mInner)) { // found
            mOuter = &test->getParent();
            mInner = mOuter->indexOf(test);
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
    const Ast *a = mOuter;
    while (a->getType() != Ast::Type::CLASS_LIST) {
        if (match(a)) {
            mOuter = &a->getParent();
            mInner = mOuter->indexOf(a);
            return;
        }
        a = &a->getParent();
    }
}

void Doc::dollyOutBig(Ast::Type match)
{
    const Ast *a = mOuter;

    while (a->getType() != Ast::Type::CLASS_LIST) {
        if (a->getType() == match) {
            mOuter = &a->getParent();
            mInner = mOuter->indexOf(a);
            return;
        }
        a = &a->getParent();
    }
}

void Doc::siblingBig(bool match(const Ast *), bool right)
{
    if (mInner == 0 && !right)
        return; // no elder sibling (ensure 'inner-1' not overflow)

    std::queue<Ast*> queue;
    if (right) {
        for (size_t i = mInner + 1; i < mOuter->size(); i++)
            queue.push(&mOuter->at(i));
    } else {
        for (ssize_t i = mInner - 1; i >= 0; i--)
            queue.push(&mOuter->at(i));
    }

    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (match(test) && test != &mOuter->at(mInner)) { // found
            mOuter = &test->getParent();
            mInner = mOuter->indexOf(test);
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
    if (mInner == 0 && !right)
        return; // no elder sibling (ensure 'inner-1' not overflow)

    std::queue<Ast*> queue;
    if (right) {
        for (size_t i = mInner + 1; i < mOuter->size(); i++)
            queue.push(&mOuter->at(i));
    } else {
        for (ssize_t i = mInner - 1; i >= 0; i--)
            queue.push(&mOuter->at(i));
    }


    // breadth-first search
    while (!queue.empty()) {
        Ast *test = queue.front();
        queue.pop();
        if (test->getType() == match && test != &mOuter->at(mInner)) { // found
            mOuter = &test->getParent();
            mInner = mOuter->indexOf(test);
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
    assert(mInner <= mOuter->size());

    Ast *a = newTree(type);

    mOuter->asList().insert(mInner, a);
    if (BopListAst::UNUSED != bop) {
        BopListAst &bast = mOuter->asBopList();
        setBop(mOuter->asBopList(), mInner, bast.opAt(mInner + 1));
        setBop(mOuter->asBopList(), mInner + 1, bop);
    }

    mTokens.sync(mRoot.get());
}

void Doc::append(Ast::Type type, int bop)
{
    ++mInner;
    assert(mInner <= mOuter->size());

    Ast *a = newTree(type);

    mOuter->asList().insert(mInner, a);
    if (BopListAst::UNUSED != bop)
        setBop(mOuter->asBopList(), mInner, bop);

    mTokens.sync(mRoot.get());
}

void Doc::assart(Ast::Type type, int bop)
{
    mOuter = &mOuter->at(mInner).asInternal();
    mInner = 0;

    assert(mOuter->size() == 0);

    insert(type, bop);
}

void Doc::remove()
{
    assert(mInner < mOuter->size());

    // only handle list ill-one and ill-zero
    // does not care about hidden node issues.
    // hidden node issues are done in normal mode
    if (mOuter->isList()) {
        ListAst *l = &mOuter->asList();
        l->erase(mInner);

        bool toRemoveSelf = l->illZero();
        bool toExposeChild = l->illOne();

        if (mOuter->size() == 0)
            digOut();
        else if (mInner >= mOuter->size())
            --mInner;

        if (toRemoveSelf)
            remove();
        else if (toExposeChild)
            expose();
    } else {
        change(mOuter->typeAt(mInner));
    }

    mTokens.sync(mRoot.get());
}

void Doc::change(Ast *a)
{
    assert(mInner < mOuter->size());

    mOuter->change(mInner, a);
    mTokens.sync(mRoot.get());
}

void Doc::change(Ast::Type type)
{
    change(newTree(type));
}

void Doc::nestAsLeft(Ast::Type type, int bop)
{
    assert(mInner < mOuter->size());

    InternalAst *nester = &newTree(type)->asInternal();
    mOuter->nestAsLeft(mInner, nester);

    if (BopListAst::UNUSED != bop)
        setBop(mOuter->at(mInner).asBopList(), 1, bop); // set inner, not outer!

    mTokens.sync(mRoot.get());
}

void Doc::nestAsRight(Ast::Type type, int bop)
{
    assert(mInner < mOuter->size());

    InternalAst *nester = &newTree(type)->asInternal();
    mOuter->nestAsRight(mInner, nester);

    if (BopListAst::UNUSED != bop)
        setBop(mOuter->at(mInner).asBopList(), 1, bop); // set inner, not outer!

    mTokens.sync(mRoot.get());
}

void Doc::expose()
{
    assert(mInner < mOuter->size());

    size_t exposee = mInner;
    digOut();
    mOuter->expose(mInner, exposee);

    mTokens.sync(mRoot.get());
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
            mOuter->change(mInner, a);
        }
    } else if (Ast::isFixSize(from, 2)
                || (Ast::isBopList(from) && getInner().asBopList().size() == 2)) {
        // size 2 ==> size 2
        if (Ast::isFixSize(to, 2) || Ast::isBopList(to)) {
            // simply copy children and change
            InternalAst *a = &newTree(to)->asInternal();
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            mOuter->change(mInner, a);
        }
    }

    mTokens.sync(mRoot.get());
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
    ScalarAst &scalar = mOuter->at(mInner).asScalar();
    while ('\0' != *str)
        scalar.append(*str++);
    mTokens.updateScalar(mOuter, mInner);
}

void Doc::scalarAppend(char c)
{
    mOuter->at(mInner).asScalar().append(c);
    mTokens.updateScalar(mOuter, mInner);
}

void Doc::scalarClear()
{
    mOuter->at(mInner).asScalar().clear();
    mTokens.updateScalar(mOuter, mInner);
}

void Doc::listClear()
{
    mOuter->at(mInner).asList().clear();
}

void Doc::toggleAbstract()
{
    FixSizes::getModifiers(mOuter->at(mInner)).abstract ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleFinal()
{
    FixSizes::getModifiers(mOuter->at(mInner)).final ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleAccess(bool up)
{
    // circulate by guarenteed 2-bit overflow
    if (up)
        FixSizes::getModifiers(mOuter->at(mInner)).access++;
    else
        FixSizes::getModifiers(mOuter->at(mInner)).access--;
    mTokens.sync(mRoot.get());
}

void Doc::toggleStatic()
{
    FixSizes::getModifiers(mOuter->at(mInner)).statik ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleTransient()
{
    FixSizes::getModifiers(mOuter->at(mInner)).transient ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleVolatile()
{
    FixSizes::getModifiers(mOuter->at(mInner)).voladile ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleNative()
{
    FixSizes::getModifiers(mOuter->at(mInner)).native ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::toggleSynchronized()
{
    FixSizes::getModifiers(mOuter->at(mInner)).synchronized ^= 1;
    mTokens.sync(mRoot.get());
}

void Doc::switchClip(char c)
{
    assert('a' <= c && c <= 'z');
    mClipIndex = c - 'a';
    mOb.observeSwitchClip(c);
}

void Doc::yank(const Ast &a)
{
    mClipslots[mClipIndex].reset(a.clone());
}

void Doc::paste()
{
    if (mClipslots[mClipIndex] == nullptr)
        return;

    Ast *a = mClipslots[mClipIndex]->clone();
    mOuter->change(mInner, a);
    mTokens.sync(mRoot.get());
}

void Doc::setHotLight(HotLightLevel level)
{
    switch (level) {
    case HotLightLevel::OFF:
        mTokens.setHotLight(-1); // TODO: de-magic
        break;
    case HotLightLevel::POINT: {
        ssize_t back = getInner().getType() == Ast::Type::STRING ? 1 : 0;
        mTokens.setHotLight(back);
        break;
    }
    case HotLightLevel::AREA:
        mTokens.setHotLight(-2); // TODO: de-magic
        break;
    }
}

void Doc::toggleTension(bool b)
{
    mOb.observeTension(b);
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


