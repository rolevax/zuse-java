// Zuse headers
#include "doc.h"

#include "../ast/parser.h"
#include "../mode/mode_normal.h"
#include "../mode/mode_menu.h"
#include "../mode/mode_tilex.h"
#include "../mode/mode_input_ident.h"
#include "../mode/mode_input_number.h"
#include "../mode/mode_input_string.h"
#include "../mode/mode_input_list.h"
#include "../mode/mode_input_fix_size.h"

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



namespace zuse
{



Doc::Doc(DocListener &listener)
    : mRoot(std::make_unique<AstRoot>())
    , mOuter(mRoot.get())
    , mTokens(listener)
    , mListener(listener)
{
    mModes.emplace_back(new ModeNormal(*this));
}

///
/// \brief Load and parse a source file
///
void Doc::load(const std::string &filename)
{
    if (mModes.size() > 1)
        throw std::runtime_error("Load failed: modifying mode not popped");

    mRoot = parse(filename);
    mOuter = mRoot.get();
    mInner = 0;

    mTokens.sync(mRoot.get());
    mTokens.light(&mOuter->at(mInner));
}

///
/// \brief Save the edit tree as a pretty-printed text file
///
void Doc::save(const std::string &filename)
{
    if (mModes.size() > 1)
        throw std::runtime_error("Save failed: modifying mode not popped");

    std::ofstream ofs(filename.c_str());
    ofs << mTokens;
    ofs.close();
}

///
/// \brief Handle a key stroke
///
void Doc::keyboard(Key key)
{
    assert(mModes.size() > 0);

    for (int i = mModes.size() - 1; i >= 0; i--) {
        assert(i < int(mModes.size()));
        Mode::Result res = mModes[i]->keyboard(key);
        bool done = res.isDone();
        handleModeResult(std::move(res));
        if (done)
            break;
    }

    if (mRoot->size() > 0)
        mTokens.light(&mOuter->at(mInner));
}

///
/// \brief Push a mode onto the mode stack,
///        Trigger the 'onPushed' callback of 'mode'
///
void Doc::push(std::unique_ptr<Mode> mode)
{
    if (mode == nullptr)
        return;

    mModes.emplace_back(std::move(mode));
    mListener.onModePushed(mModes.back()->name());
    Mode::Result res = mModes.back()->onPushed();
    assert(res.isDone());
    handleModeResult(std::move(res));
}

///
/// \brief Pop the top of the mode stack
///
void Doc::pop()
{
    assert(mModes.size() > 1); // bottom normal mode reserved

    mListener.onModePopped();

    std::unique_ptr<Mode> popped = std::move(mModes.back());
    mModes.pop_back();
    popped->onPopped();
}

void Doc::handleModeResult(Mode::Result res)
{
    if (res.toPop()) {
        pop();
        push(std::move(res.nextPush));
        if (res.type == Mode::ResultType::DONE_POP)
            handleModeResult(mModes.back()->onResume());
    } else { // stay on the stack
        push(std::move(res.nextPush));
    }
}

const AstInternal &Doc::getOuter() const
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

    AstInternal &focus = mOuter->at(mInner).asInternal();
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

    AstInternal *nextOuter = &mOuter->getParent();
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
            mInner = (mInner + size + dir) % size;
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
            AstInternal &inTest = test->asInternal();
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
            AstInternal &inTest = test->asInternal();
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
            AstInternal &inTest = test->asInternal();
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
            AstInternal &inTest = test->asInternal();
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
void Doc::insert(Ast::Type type, Bop bop)
{
    assert(mInner <= mOuter->size());

    mOuter->asList().insert(mInner, newTree(type));
    if (Bop::UNUSED != bop) {
        AstListBop &bast = mOuter->asBopList();
        setBop(mOuter->asBopList(), mInner, bast.opAt(mInner + 1));
        setBop(mOuter->asBopList(), mInner + 1, bop);
    }

    mTokens.sync(mRoot.get());
}

void Doc::append(Ast::Type type, Bop bop)
{
    ++mInner;
    assert(mInner <= mOuter->size());

    mOuter->asList().insert(mInner, newTree(type));
    if (Bop::UNUSED != bop)
        setBop(mOuter->asBopList(), mInner, bop);

    mTokens.sync(mRoot.get());
}

void Doc::assart(Ast::Type type, Bop bop)
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
        AstList *l = &mOuter->asList();
        l->remove(mInner);

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

void Doc::change(std::unique_ptr<Ast> a)
{
    assert(mInner < mOuter->size());

    mOuter->change(mInner, std::move(a));
    mTokens.sync(mRoot.get());
}

void Doc::change(Ast::Type type)
{
    change(newTree(type));
}

void Doc::nestAsLeft(Ast::Type type, Bop bop)
{
    assert(mInner < mOuter->size());

    mOuter->nestAsLeft(mInner, newInternalTree(type));

    if (Bop::UNUSED != bop)
        setBop(mOuter->at(mInner).asBopList(), 1, bop); // set inner, not outer!

    mTokens.sync(mRoot.get());
}

void Doc::nestAsRight(Ast::Type type, Bop bop)
{
    assert(mInner < mOuter->size());

    mOuter->nestAsRight(mInner, newInternalTree(type));

    if (Bop::UNUSED != bop)
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
        nestAsLeft(Type::ARG_LIST, Bop::UNUSED);
    } else if (from == Type::DECL_VAR && to == Type::DECL_METHOD) {
        if (getInner().asInternal().at(1).isScalar()) {
            // from variable declaration to method declaration
            auto a = newInternalTree(Type::DECL_METHOD);
            // copy return type and name
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            mOuter->change(mInner, std::move(a));
        }
    } else if (Ast::isFixSize(from, 2)
                || (Ast::isBopList(from) && getInner().asBopList().size() == 2)) {
        // size 2 ==> size 2
        if (Ast::isFixSize(to, 2) || Ast::isBopList(to)) {
            // simply copy children and change
            auto a = newInternalTree(to);
            a->change(0, getInner().asInternal().at(0).clone());
            a->change(1, getInner().asInternal().at(1).clone());
            mOuter->change(mInner, std::move(a));
        }
    }

    mTokens.sync(mRoot.get());
}

std::unique_ptr<Mode> Doc::createModifyMode(bool clear, size_t offset, bool macroContext)
{
    if (getInner().isList() && !getInner().isBopList())
        return std::make_unique<ModeInputList>(*this);
    else if (getInner().isFixSize())
        return std::make_unique<ModeInputFixSize>(*this, getInner().asInternal(), offset);

    switch (getInner().getType()) {
    case Ast::Type::META:
        return std::make_unique<ModeTilex>(*this, macroContext);
    case Ast::Type::IDENT:
        return std::make_unique<ModeInputIdent>(*this, clear);
    case Ast::Type::STRING:
        return std::make_unique<ModeInputString>(*this, clear);
    case Ast::Type::NUMBER:
        return std::make_unique<ModeInputNumber>(*this, clear);
    default:
        return nullptr;
    }
}

void Doc::scalarAppend(const char *str)
{
    AstScalar &scalar = mOuter->at(mInner).asScalar();
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
    mListener.onClipSwitched(c);
}

void Doc::yank(const Ast &a)
{
    mClipslots[mClipIndex] = a.clone();
}

void Doc::paste()
{
    if (mClipslots[mClipIndex] == nullptr)
        return;

    mOuter->change(mInner, mClipslots[mClipIndex]->clone());
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
    mListener.onCursorTension(b);
}

void Doc::setBop(AstListBop &blist, size_t pos, Bop bop)
{
    blist.setOpAt(pos, bop);
    if (blist.getType() == Ast::Type::DOT_BOP_LIST
            && bop == Bop::CALL) {
        if (blist.at(pos).getType() != Ast::Type::ARG_LIST)
            blist.nestAsLeft(pos, newInternalTree(Ast::Type::ARG_LIST));
    }
}

///
/// \brief Make a minimal non-ill tree
/// \param type Type of the tree
///
std::unique_ptr<Ast> Doc::newTree(Ast::Type type)
{
    if (!Ast::isScalar(type))
        return newInternalTree(type);

    switch (type) {
    case Ast::Type::IDENT:
        return std::make_unique<AstScalar>(Ast::Type::IDENT, "id");
    case Ast::Type::STRING:
        return std::make_unique<AstScalar>(Ast::Type::STRING, "");
    case Ast::Type::NUMBER:
        return std::make_unique<AstScalar>(Ast::Type::NUMBER, "0");
    case Ast::Type::META:
        return std::make_unique<AstScalar>(Ast::Type::META, "");
    case Ast::Type::HIDDEN:
        return std::make_unique<AstScalar>(Ast::Type::HIDDEN, "");
    default:
        throw "WTF";
    }
}

std::unique_ptr<AstInternal> Doc::newInternalTree(Ast::Type type)
{
    std::unique_ptr<AstInternal> a;

    if (Ast::isBopList(type)) {
        auto lhs = newTree(AstInternal::typeAt(type, 0));
        auto rhs = newTree(AstInternal::typeAt(type, 1));
        a = std::make_unique<AstListBop>(type, std::move(lhs), std::move(rhs), Bop::DEFAULT);
    } else if (Ast::isList(type)) {
        a = std::make_unique<AstList>(type);
        AstList &list = a->asList();
        if (list.illZero())
            list.append(newTree(list.typeAt(0)));
        if (list.illOne())
            list.append(newTree(list.typeAt(1)));
    } else if (Ast::isFixSize(type)) {
        if (Ast::isFixSize(type, 1)) {
            auto t0 = newTree(AstInternal::typeAt(type, 0));
            a = std::make_unique<AstFixSize<1>>(type, std::move(t0));
        } else if (Ast::isFixSize(type, 2)) {
            auto t0 = newTree(AstInternal::typeAt(type, 0));
            auto t1 = newTree(AstInternal::typeAt(type, 1));
            a = std::make_unique<AstFixSize<2>>(type, std::move(t0), std::move(t1));
        } else if (Ast::isFixSize(type, 3)) {
            auto t0 = newTree(AstInternal::typeAt(type, 0));
            auto t1 = newTree(AstInternal::typeAt(type, 1));
            auto t2 = newTree(AstInternal::typeAt(type, 2));
            a = std::make_unique<AstFixSize<3>>(type, std::move(t0), std::move(t1), std::move(t2));
        } else if (Ast::isFixSize(type, 4)) {
            auto t0 = newTree(AstInternal::typeAt(type, 0));
            auto t1 = newTree(AstInternal::typeAt(type, 1));
            auto t2 = newTree(AstInternal::typeAt(type, 2));
            auto t3 = newTree(AstInternal::typeAt(type, 3));
            a = std::make_unique<AstFixSize<4>>(type,
                                                std::move(t0), std::move(t1),
                                                std::move(t2), std::move(t3));
        } else if (Ast::isFixSize(type, 5)) {
            auto t0 = newTree(AstInternal::typeAt(type, 0));
            auto t1 = newTree(AstInternal::typeAt(type, 1));
            auto t2 = newTree(AstInternal::typeAt(type, 2));
            auto t3 = newTree(AstInternal::typeAt(type, 3));
            auto t4 = newTree(AstInternal::typeAt(type, 4));
            a = std::make_unique<AstFixSize<5>>(type,
                                                std::move(t0), std::move(t1),
                                                std::move(t2), std::move(t3), std::move(t4));
        } else {
            throw "TODO";
        }
    } else {
        throw "WTF";
    }

    return a;
}



} // namespace zuse
