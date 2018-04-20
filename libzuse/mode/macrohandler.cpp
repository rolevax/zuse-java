#include "macrohandler.h"

MacroHandler::MacroHandler(EditableDoc &doc)
    : mDoc(doc)
{

}

bool MacroHandler::macro(Key key, Mode *&nextPush)
{
    Ast::Type ot = mDoc.getOuter().getType();
    if (ot == Ast::Type::CLASS_LIST && mDoc.getOuter().size() == 0)
        return false;

    // meta comment:
    // "-->" means "go onto"
    // "==>" means "transform into"
    switch (key) {
    case Key::LEFT_PAREN:
        return macroLeftParen(nextPush);
    case Key::LEFT_BRACE:
        return macroLeftBrace(nextPush);
    case Key::COMMA:
        return macroComma(nextPush);
    case Key::ENTER:
    case Key::S_ENTER:
        return macroEnter(nextPush, key == Key::S_ENTER);
    default:
        return macroBop(key, nextPush);
    }
}

bool MacroHandler::macroLeftParen(Mode *&nextPush)
{
    Ast::Type ot = mDoc.getOuter().getType();
    Ast::Type it = mDoc.getInner().getType();

    if (ot == Ast::Type::MEMBER_LIST && it == Ast::Type::DECL_VAR) {
        // decl_var ==> decl_method
        mDoc.cast(Ast::Type::DECL_METHOD);
        // use offset 2 to skip return type and identifier
        nextPush = createMode(2);
        return true;
    } else if (ot == Ast::Type::DECL_VAR) {
        // dector --> decl_var ==> decl_method
        mDoc.digOut();
        mDoc.cast(Ast::Type::DECL_METHOD);
        // use offset 2 to skip return type and identifier
        nextPush = createMode(2);
        return true;
    } else if (ot == Ast::Type::IF_LIST) {
        if (it == Ast::Type::STMT_LIST) {
            // stmt_list ==> if_condbody
            // convert an 'else' into an 'else if'
            mDoc.nestAsRight(Ast::Type::IF_CONDBODY);
            mDoc.fallIn(); // inner becomes if-condition
        } else  {
            mDoc.append(Ast::Type::IF_CONDBODY);
        }
        nextPush = createMode();
        return true;
    } else if (it == Ast::Type::IF_CONDBODY) { // outer is not if-list
        // if_condbody (bare) ==> if_list
        // if_list is ill-one and if_list.typeAt() == if_condbody
        // so this will create a if_condbody child
        mDoc.nestAsLeft(Ast::Type::IF_LIST);
        mDoc.fallIn();
        mDoc.sibling(+1);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::IF_CONDBODY && it == Ast::Type::STMT_LIST) {
        // stmt_list --> if_condbody, and recursive call macro()
        // the additional 'it == stmt_list' is added
        // to guarentee method call can be inputted inside if-condition.
        // this is because the macroBop() call is after this
        // and I'm too lazy to change the order.
        // it's may actullay be a bug and I'll think about it if I had time.
        // ...a solution may be "if the condition is a name, then make it a
        // method call; else append another if_condbody".
        mDoc.digOut();
        return macroLeftParen(nextPush);
    } else if (ot == Ast::Type::TRY_LIST) {
        mDoc.append(Ast::Type::CATCH);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::CATCH) {
        mDoc.digOut();
        return macroLeftParen(nextPush);
    } else {
        return macroBop(Key::LEFT_PAREN, nextPush);
    }
}

bool MacroHandler::macroLeftBrace(Mode *&nextPush)
{
    Ast::Type ot = mDoc.getOuter().getType();
    Ast::Type it = mDoc.getInner().getType();

    if (ot == Ast::Type::IF_LIST) {
        mDoc.append(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (it == Ast::Type::IF_CONDBODY) {
        mDoc.nestAsLeft(Ast::Type::IF_LIST);
        mDoc.fallIn(); // now outer is if_condbody (else if)
        mDoc.sibling(+1);
        mDoc.change(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::IF_CONDBODY && it == Ast::Type::STMT_LIST) {
        mDoc.digOut();
        return macroLeftBrace(nextPush);
    } else if (ot == Ast::Type::TRY_LIST) {
        mDoc.append(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::CATCH) {
        mDoc.digOut();
        return macroLeftBrace(nextPush);
    } else {
        return false;
    }
}

bool MacroHandler::macroComma(Mode *&nextPush)
{
    const Ast *a = &mDoc.getOuter();
    Ast::Type at;
    int digCount = 0;

    while (at = a->getType(), at != Ast::Type::CLASS_LIST
           && at != Ast::Type::ARG_LIST
           && at != Ast::Type::DECL_PARAM_LIST
           && at != Ast::Type::DECTOR_LIST
           && at != Ast::Type::DECL_VAR) {
        a = &a->getParent();
        digCount++;
    }

    if (at == Ast::Type::CLASS_LIST) {
        return false; // comma-able node not found
    } else {
        while (digCount --> 0)
            mDoc.digOut();

        if (at == Ast::Type::DECL_VAR) {
            // if there is a declarator list, variable declaration
            // node won't be reached in the loop.
            // so here we must nest.
            mDoc.nestAsLeft(Ast::Type::DECTOR_LIST);
            // TODO check nest with dector_list
            mDoc.fallIn();
            mDoc.sibling(+1);
        } else {
            // already had list
            mDoc.append(mDoc.getOuter().typeAt(0));
        }
        nextPush = createMode();
        return true;
    }
}

bool MacroHandler::macroEnter(Mode *&nextPush, bool shift)
{
    const Ast *a = &mDoc.getOuter();
    Ast::Type at;
    int digCount = 0;

    while (at = a->getType(), at != Ast::Type::CLASS_LIST
           && at != Ast::Type::STMT_LIST) {
        a = &a->getParent();
        digCount++;
    }

    if (at == Ast::Type::CLASS_LIST) {
        return false; // statement list not found
    } else {
        while (digCount --> 0)
            mDoc.digOut();

        if (shift)
            mDoc.insert(mDoc.getOuter().typeAt(0));
        else
            mDoc.append(mDoc.getOuter().typeAt(0));

        nextPush = createMode();
        return true;
    }
}

bool MacroHandler::macroBop(Key key, Mode *&nextPush)
{
    Ast::Type type;
    int op = BopListAst::UNUSED;

    switch (key) {
    case Key::LEFT_PAREN:
        type = Ast::Type::DOT_BOP_LIST;
        op = BopListAst::CALL;
        break;
    case Key::LEFT_SQUARE:
        type = Ast::Type::DOT_BOP_LIST;
        op = BopListAst::ARR;
        break;
    case Key::RIGHT_PAREN:
        type = Ast::Type::CAST;
        break;
    case Key::DOT:
        type = Ast::Type::DOT_BOP_LIST;
        op = BopListAst::DOT;
        break;
    case Key::ASTERISK:
        type = Ast::Type::MUL_BOP_LIST;
        op = BopListAst::MUL;
        break;
    case Key::SLASH:
        type = Ast::Type::MUL_BOP_LIST;
        op = BopListAst::DIV;
        break;
    case Key::PERCENT:
        type = Ast::Type::MUL_BOP_LIST;
        op = BopListAst::MOD;
        break;
    case Key::PLUS:
        type = Ast::Type::ADD_BOP_LIST;
        op = BopListAst::ADD;
        break;
    case Key::MINUS:
        type = Ast::Type::ADD_BOP_LIST;
        op = BopListAst::SUB;
        break;
    case Key::EQUAL:
        type = Ast::Type::ASSIGN;
        break;
    case Key::LESS:
        type = Ast::Type::LT;
        break;
    case Key::GREATER:
        type = Ast::Type::GT;
        break;
    case Key::EXCLAM:
        type = Ast::Type::LOGIC_NOT;
        break;
    case Key::TIDE:
        type = Ast::Type::BIT_NOT;
        break;
    case Key::AND:
        type = Ast::Type::BIT_AND;
        break;
    case Key::PIPE:
        type = Ast::Type::BIT_OR;
        break;
    case Key::CARET:
        type = Ast::Type::BIT_XOR;
        break;
    default:
        return false;
    }

    // precondition from this line: inner is an expression

    // simulate plain-text input by comparing precedence
    // and the inner should be the last child
    while (Ast::precedence(type) < mDoc.getOuter().precedence()
           && mDoc.getInnerIndex() == mDoc.getOuter().size() - 1)
        mDoc.digOut();

    if (type == Ast::Type::CAST) {
        mDoc.nestAsRight(type);
        mDoc.fallIn();
    } else if (mDoc.getOuter().getType() == type && mDoc.getOuter().isList()) {
        mDoc.append(Ast::Type::META, op);
    } else {
        if (type == Ast::Type::ASSIGN
                && mDoc.getInner().getType() == Ast::Type::LOGIC_NOT) {
            // !x ==> x != ??
            mDoc.fallIn();
            mDoc.expose();
            type = Ast::Type::NEQ;
        }

        mDoc.nestAsLeft(type, op);

        // no need for modifying '!' and '~'
        if (!Ast::isFixSize(type, 1)) {
            mDoc.fallIn();
            mDoc.sibling(+1);
        }
    }

    if (!Ast::isFixSize(type, 1))
        nextPush = createMode();

    return true;
}

Mode *MacroHandler::createMode(int offset)
{
    return mDoc.createModifyMode(true, offset, true);
}

