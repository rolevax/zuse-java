// Yaloe headers
#include "core/doc.h"
#include "ast/parser.h"
#include "mode/viewmode.h"
#include "mode/menumode.h"
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
    modes.emplace(new ViewMode(*this));
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
void Doc::keyboard(char key)
{
    assert(modes.size() > 0);
    modes.top()->keyboard(key);
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
    modes.emplace(mode);
    ob.observePush(modes.top()->name());
    modes.top()->onPushed();
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

    std::unique_ptr<Mode> popped = std::move(modes.top());
    modes.pop();
    popped->onPopped();

    if (nextPush != nullptr)
        push(nextPush);
    else
        modes.top()->onResume();
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
        assert(focus.isMap());
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

    // TODO list? var-size?
    outer->asList().insert(inner, a);
    if (BopListAst::UNUSED != bop)
        outer->asBopList().setOpAt(inner, bop);

    tokens.sync(root.get());
}

void Doc::append(Ast::Type type, int bop)
{
    ++inner;
    insert(type, bop);
}

void Doc::assart(Ast::Type type, int bop)
{
    outer = &outer->at(inner).asInternal();
    inner = 0;
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
    }
    // TODO: else if outer->isFixSize, type <- outer->placeholderAt(inner)...
    // TODO: else if outer->isLimitSize

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
    // using clone() to implement a movement.
    // this is to enhance the simplicity of the interface
    // with costing neglectable time.
    nester->change(0, outer->at(inner).clone());
    if (BopListAst::UNUSED != bop)
        nester->asBopList().setOpAt(1, bop);
    outer->change(inner, nester);
    tokens.sync(root.get());
}

void Doc::expose()
{
    assert(inner < outer->size());

    Ast *exposee = outer->at(inner).clone();
    digOut();
    outer->change(inner, exposee);
    tokens.sync(root.get());
}

void Doc::cast(Ast::Type type)
{
    switch (type) {
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

Ast *Doc::newTree(Ast::Type type)
{
    Ast *a = nullptr;

    switch (type) {
    case Ast::Type::DECL_CLASS: {
        Ast *id = new ScalarAst(Ast::Type::IDENT, "C0");
        Ast *ml = new ListAst(Ast::Type::MEMBER_LIST);
        a = new FixSizeAst<2>(Ast::Type::DECL_CLASS, id, ml);
        break;
    }
    case Ast::Type::DECL_VAR: {
        Ast *type = new ScalarAst(Ast::Type::IDENT, "Type");
        Ast *id = new ScalarAst(Ast::Type::IDENT, "ident");
        a = new FixSizeAst<2>(Ast::Type::DECL_VAR, type, id);
        break;
    }
    case Ast::Type::DECL_METHOD: {
        Ast *type = new ScalarAst(Ast::Type::IDENT, "T0");
        Ast *id = new ScalarAst(Ast::Type::IDENT, "m0");
        Ast *dpl = new ListAst(Ast::Type::DECL_PARAM_LIST);
        Ast *sl = new ListAst(Ast::Type::STMT_LIST);
        a = new FixSizeAst<4>(Ast::Type::DECL_METHOD, type, id, dpl, sl);
        break;
    }
    case Ast::Type::ASSIGN: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = new FixSizeAst<2>(Ast::Type::ASSIGN, lhs, rhs);
        break;
    }
    case Ast::Type::ADD_BOP_LIST: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = new BopListAst(Ast::Type::ADD_BOP_LIST, lhs, rhs, BopListAst::ADD);
        break;
    }
    case Ast::Type::MUL_BOP_LIST: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = new BopListAst(Ast::Type::MUL_BOP_LIST, lhs, rhs, BopListAst::MUL);
        break;
    }
    case Ast::Type::DOT_BOP_LIST: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = new BopListAst(Ast::Type::DOT_BOP_LIST, lhs, rhs, BopListAst::DOT);
        break;
    }
    case Ast::Type::DECL_PARAM: {
        Ast *type = new ScalarAst(Ast::Type::IDENT, "Type");
        Ast *id = new ScalarAst(Ast::Type::IDENT, "ident");
        a = new FixSizeAst<2>(Ast::Type::DECL_PARAM, type, id);
        break;
    }
    case Ast::Type::IDENT: {
        a = new ScalarAst(Ast::Type::IDENT, "ident");
        break;
    }
    case Ast::Type::STRING: {
        a = new ScalarAst(Ast::Type::STRING, "");
        break;
    }
    case Ast::Type::META: {
        a = new ScalarAst(Ast::Type::META, "");
        break;
    }
    default:
        throw "newTree: untreated type";
    }

    return a;
}


