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

    Ast::Type it = doc.getInner().getType();

    // meta comment:
    // "-->" means "go onto"
    // "==>" means "transform into"
    switch (key) {
    case Key::LEFT_PAREN:
        return macroLeftParen(nextPush);
    case Key::LEFT_BRACE:
        if (ot == Ast::Type::IF_LIST) {
            doc.append(Ast::Type::STMT_LIST);
            nextPush = doc.createModifyMode(true);
            return true;
        } else if (ot == Ast::Type::IF_CONDBODY && it == Ast::Type::STMT_LIST) {
            doc.digOut();
            ot = doc.getOuter().getType();
            if (ot == Ast::Type::IF_LIST) {
                doc.append(Ast::Type::STMT_LIST);
            } else {
                doc.nestAsLeft(Ast::Type::IF_LIST);
                doc.fallIn();
                doc.sibling(+1); // now inner is if_condbody (else if)
                doc.change(Ast::Type::STMT_LIST);
            }
            nextPush = doc.createModifyMode(true);
            return true;
        } else {
            return false;
        }
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
        nextPush = doc.createModifyMode(true, 2);
        return true;
    } else if (ot == Ast::Type::DECL_VAR) {
        // dector --> decl_var ==> decl_method
        doc.digOut();
        doc.cast(Ast::Type::DECL_METHOD);
        // use offset 2 to skip return type and identifier
        nextPush = doc.createModifyMode(true, 2);
        return true;
    } else if (ot == Ast::Type::IF_LIST) {
        if (it == Ast::Type::STMT_LIST) {
            // stmt_list (else) ==> if_condbody (else if)
            doc.nestAsRight(Ast::Type::IF_CONDBODY);
            doc.fallIn(); // inner becomes if-condition
        } else  {
            doc.append(Ast::Type::IF_CONDBODY);
        }
        nextPush = doc.createModifyMode(true);
        return true;
    } else if (it == Ast::Type::IF_CONDBODY) { // outer is not if-list
        // if_condbody (bare) ==> if_list
        // if_list is ill-one and if_list.typeAt() == if_condbody
        // so this will create a if_condbody child
        doc.nestAsLeft(Ast::Type::IF_LIST);
        doc.fallIn();
        doc.sibling(+1);
        nextPush = doc.createModifyMode(true);
        return true;
    } else if (ot == Ast::Type::IF_CONDBODY && it == Ast::Type::STMT_LIST) {
        // stmt_list --> if_condbody, and recursive call macro()
        // when outer is a if_condbody,
        // for enabling method call marco (left-paren) inside if-condition,
        // only apply this macro when inner is if-body
        doc.digOut();
        return macroLeftParen(nextPush);
    } else {
        return macroBop(Key::LEFT_PAREN, nextPush);
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
        nextPush = doc.createModifyMode(true);
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

        nextPush = doc.createModifyMode(true);
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

    if (type == Ast::Type::CAST) {
        doc.nestAsRight(type);
        doc.fallIn();
    } else if (doc.getOuter().getType() == type && doc.getOuter().isList()) {
        doc.append(Ast::Type::META, op);
    } else {
        doc.nestAsLeft(type, op);
        if (!Ast::isFixSize(type, 1)) {
            doc.fallIn();
            doc.sibling(+1);
        }
    }

    if (!Ast::isFixSize(type, 1))
        nextPush = doc.createModifyMode(true);

    return true;
}

