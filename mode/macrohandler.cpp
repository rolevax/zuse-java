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
        if (ot == Ast::Type::MEMBER_LIST && it == Ast::Type::DECL_VAR) {
            // decl_var ==> decl_method
            doc.cast(Ast::Type::DECL_METHOD);
            // use offset 2 to skip return type and identifier
            nextPush = doc.createModifyMode(true, 2);
        } else if (ot == Ast::Type::DECL_VAR) {
            // dector --> decl_var ==> decl_method
            doc.digOut();
            doc.cast(Ast::Type::DECL_METHOD);
            // use offset 2 to skip return type and identifier
            nextPush = doc.createModifyMode(true, 2);
        } else if (ot == Ast::Type::IF_LIST) {
            // TODO
        } else {
            return macroBop(key, nextPush);
        }
        return true;
    case Key::COMMA: {
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
    case Key::ENTER:
    case Key::S_ENTER: {
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

            if (key == Key::ENTER)
                doc.append(doc.getOuter().typeAt(0));
            else
                doc.insert(doc.getOuter().typeAt(0));

            nextPush = doc.createModifyMode(true);
            return true;
        }
    }
    default:
        return macroBop(key, nextPush);
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
    default:
        return false;
    }

    // precondition from this line: inner is an expression

    if (doc.getOuter().getType() == type && doc.getOuter().isList()) {
        doc.append(Ast::Type::META, op);
    } else {
        doc.nestAsLeft(type, op);
        doc.fallIn();
        doc.sibling(+1);
    }
    nextPush = doc.createModifyMode(true);
    return true;
}

