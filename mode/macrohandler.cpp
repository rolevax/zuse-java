#include "mode/macrohandler.h"

MacroHandler::MacroHandler(EditableDoc &doc)
    : doc(doc)
{

}

bool MacroHandler::macro(Key key, Mode *&nextPush)
{
    Ast::Type ot = doc.getOuter().getType();
    if (ot == Ast::Type::CLASS_LIST && doc.getOuter().size() == 0)
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
    Ast::Type ot = doc.getOuter().getType();
    Ast::Type it = doc.getInner().getType();

    if (ot == Ast::Type::MEMBER_LIST && it == Ast::Type::DECL_VAR) {
        // decl_var ==> decl_method
        doc.cast(Ast::Type::DECL_METHOD);
        // use offset 2 to skip return type and identifier
        nextPush = createMode(2);
        return true;
    } else if (ot == Ast::Type::DECL_VAR) {
        // dector --> decl_var ==> decl_method
        doc.digOut();
        doc.cast(Ast::Type::DECL_METHOD);
        // use offset 2 to skip return type and identifier
        nextPush = createMode(2);
        return true;
    } else if (ot == Ast::Type::IF_LIST) {
        if (it == Ast::Type::STMT_LIST) {
            // stmt_list ==> if_condbody
            // convert an 'else' into an 'else if'
            doc.nestAsRight(Ast::Type::IF_CONDBODY);
            doc.fallIn(); // inner becomes if-condition
        } else  {
            doc.append(Ast::Type::IF_CONDBODY);
        }
        nextPush = createMode();
        return true;
    } else if (it == Ast::Type::IF_CONDBODY) { // outer is not if-list
        // if_condbody (bare) ==> if_list
        // if_list is ill-one and if_list.typeAt() == if_condbody
        // so this will create a if_condbody child
        doc.nestAsLeft(Ast::Type::IF_LIST);
        doc.fallIn();
        doc.sibling(+1);
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
        doc.digOut();
        return macroLeftParen(nextPush);
    } else if (ot == Ast::Type::TRY_LIST) {
        doc.append(Ast::Type::CATCH);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::CATCH) {
        doc.digOut();
        return macroLeftParen(nextPush);
    } else {
        return macroBop(Key::LEFT_PAREN, nextPush);
    }
}

bool MacroHandler::macroLeftBrace(Mode *&nextPush)
{
    Ast::Type ot = doc.getOuter().getType();
    Ast::Type it = doc.getInner().getType();

    if (ot == Ast::Type::IF_LIST) {
        doc.append(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (it == Ast::Type::IF_CONDBODY) {
        doc.nestAsLeft(Ast::Type::IF_LIST);
        doc.fallIn(); // now outer is if_condbody (else if)
        doc.sibling(+1);
        doc.change(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::IF_CONDBODY && it == Ast::Type::STMT_LIST) {
        doc.digOut();
        return macroLeftBrace(nextPush);
    } else if (ot == Ast::Type::TRY_LIST) {
        doc.append(Ast::Type::STMT_LIST);
        nextPush = createMode();
        return true;
    } else if (ot == Ast::Type::CATCH) {
        doc.digOut();
        return macroLeftBrace(nextPush);
    } else {
        return false;
    }
}

bool MacroHandler::macroComma(Mode *&nextPush)
{
    const Ast *a = &doc.getOuter();
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
            doc.digOut();

        if (at == Ast::Type::DECL_VAR) {
            // if there is a declarator list, variable declaration
            // node won't be reached in the loop.
            // so here we must nest.
            doc.nestAsLeft(Ast::Type::DECTOR_LIST);
            // TODO check nest with dector_list
            doc.fallIn();
            doc.sibling(+1);
        } else {
            // already had list
            doc.append(doc.getOuter().typeAt(0));
        }
        nextPush = createMode();
        return true;
    }
}

bool MacroHandler::macroEnter(Mode *&nextPush, bool shift)
{
    const Ast *a = &doc.getOuter();
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
            doc.digOut();

        if (shift)
            doc.insert(doc.getOuter().typeAt(0));
        else
            doc.append(doc.getOuter().typeAt(0));

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
    while (Ast::precedence(type) < doc.getOuter().precedence()
           && doc.getInnerIndex() == doc.getOuter().size() - 1)
        doc.digOut();

    if (type == Ast::Type::CAST) {
        doc.nestAsRight(type);
        doc.fallIn();
    } else if (doc.getOuter().getType() == type && doc.getOuter().isList()) {
        doc.append(Ast::Type::META, op);
    } else {
        if (type == Ast::Type::ASSIGN
                && doc.getInner().getType() == Ast::Type::LOGIC_NOT) {
            // !x ==> x != ??
            doc.fallIn();
            doc.expose();
            type = Ast::Type::NEQ;
        }

        doc.nestAsLeft(type, op);

        // no need for modifying '!' and '~'
        if (!Ast::isFixSize(type, 1)) {
            doc.fallIn();
            doc.sibling(+1);
        }
    }

    if (!Ast::isFixSize(type, 1))
        nextPush = createMode();

    return true;
}

Mode *MacroHandler::createMode(int offset)
{
    return doc.createModifyMode(true, offset, true);
}

