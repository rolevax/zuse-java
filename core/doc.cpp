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
    if (root->size() > 0)
        modes.top()->keyboard(key);
    else
        modes.top()->emptyKeyboard(key); // TODO might be useless
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

const Ast &Doc::getOuter() const
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

    if (outer->at(inner).isScalar())
        return; // cannot fall in scalar

    InternalAst &focus = outer->at(inner).asInternal();

    if (focus.isList()) {
        if (focus.size() == 0) {
            push(new MenuMode(*this, MenuMode::Context::ASSART));
        } else {
            outer = &focus;
            inner = 0;
        }
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
 * @brief Create an empty subnode
 * @param type the type of the new node
 * Create a new subnode inside 'outer' at 'inner',
 * with specified type 'type'.
 * The value of the new node is "as empty as possible".
 */
void Doc::insert(Ast::Type type)
{
    assert(inner <= outer->size());

    Ast *a = newTree(type);

    // TODO list? var-size?
    outer->asList().insert(inner, a);

    tokens.sync(root.get());
}

void Doc::append(Ast::Type type)
{
    ++inner;
    insert(type);
}

void Doc::assart(Ast::Type type)
{
    outer = &outer->at(inner).asInternal();
    inner = 0;
    insert(type);
}

void Doc::remove()
{
    /*
    assert(inner < outer->size());

    tokens.remove(outer, inner);
    clipboard = outer->remove(inner);
    if (outer->getType() == Ast::Type::ROOT) {
        // nothing to do yet, just leave it here
    } else if (Ast::isList(*outer)) {
        if (inner == outer->size()) {
            if (inner > 0) {
                --inner;
            } else { // outer became empty
                digOut();
                // re-insert to generate special empty look
                tokens.remove(outer, inner);
                tokens.insert(outer, inner);
            }
        }
    }
    */
}

void Doc::change(Ast::Type type)
{
    assert(inner < outer->size());

    if (!outer->at(inner).isChangeable())
        return;

    tokens.remove(outer, inner);
    outer->change(inner, newTree(type));
    tokens.insert(&outer->asList(), inner);
}

void Doc::nestAsLeft(Ast::Type type)
{
    assert(inner < outer->size());

    InternalAst *nester = &newTree(type)->asInternal();
    // allowing removing inside a non-list ast breaks
    // the principle of consistency through ast's interface.
    // thus clone() is used here to enhance the integrity of
    // the interface design in a cost of effeciency.
    nester->change(0, outer->at(inner).clone());
    outer->change(inner, nester);
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

void Doc::setHotLight(bool b)
{
    if (b) {
        ssize_t back = 0; // TODO: consider quote, etc.
        tokens.setHotLight(back);
    } else {
        tokens.setHotLight(-1);
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
    case Ast::Type::CLASS: {
        Ast *id = new ScalarAst(Ast::Type::IDENT, "C0");
        Ast *ml = new ListAst(Ast::Type::MEMBER_LIST);
        a = new FixSizeAst<2>(Ast::Type::CLASS, id, ml);
        break;
    }
    case Ast::Type::DECL_STMT: {
        Ast *type = new ScalarAst(Ast::Type::IDENT, "Type");
        Ast *id = new ScalarAst(Ast::Type::IDENT, "ident");
        ListAst *dbl = new ListAst(Ast::Type::DECL_BEAN_LIST);
        Ast *db = new DeclBeanAst(id) ;
        dbl->append(db);
        a = new FixSizeAst<2>(Ast::Type::DECL_STMT, type, dbl);
        break;
    }
    case Ast::Type::METHOD: {
        Ast *id = new ScalarAst(Ast::Type::IDENT, "m0");
        Ast *dpl = new ListAst(Ast::Type::DECL_PARAM_LIST);
        Ast *sl = new ListAst(Ast::Type::STMT_LIST);
        a = new FixSizeAst<3>(Ast::Type::METHOD, id, dpl, sl);
        break;
    }
    case Ast::Type::ASSIGN: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = new FixSizeAst<2>(Ast::Type::ASSIGN, lhs, rhs);
        break;
    }
    case Ast::Type::ADDSUB_LIST: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = TermListAst::makeBop(lhs, rhs, TermListAst::Op::ADD);
        break;
    }
    case Ast::Type::MULDIV_LIST: {
        Ast *lhs = new ScalarAst(Ast::Type::IDENT, "lhs");
        Ast *rhs = new ScalarAst(Ast::Type::IDENT, "rhs");
        a = TermListAst::makeBop(lhs, rhs, TermListAst::Op::MUL);
        break;
    }
    case Ast::Type::CALL: {
        Ast *id = new ScalarAst(Ast::Type::IDENT, "meth");
        Ast *args = new ListAst(Ast::Type::ARG_LIST);
        a = new FixSizeAst<2>(Ast::Type::CALL, id, args);
        break;
    }
    case Ast::Type::IDENT: {
        a = new ScalarAst(Ast::Type::IDENT, "");
        break;
    }
    default:
        throw "newTree: untreated type";
    }

    return a;
}


