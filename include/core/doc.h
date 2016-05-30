#ifndef DOC_H
#define DOC_H

#include "core/editabledoc.h"
#include "core/keycode.h"
#include "core/tokens.h"
#include "ast/rootast.h"

class BopListAst;
class PDoc;

class Doc : public EditableDoc
{
public:
    Doc(PDoc &dob, TokensObserver &tob);

    Doc(const Doc&) = delete;
    Doc &operator=(const Doc&) = delete;

    void load(const std::string &filename);
    void save(const std::string &filename);
    void keyboard(Key key); // TODO XXX: change here, and be a M

private:
    // TODO: re-sort
    void push(Mode *mode);
    void pop();
    void handleModeResult(const Mode::Result &res);

    /// @name EditableDoc interface
    ///@{
    const InternalAst &getOuter() const override;
    const Ast &getInner() const override;
    size_t getInnerIndex() const override;

    void fallIn() override;
    void digOut() override;
    void sibling(int step, bool skipHidden) override;
    void jackKick(bool down) override;
    void hackLead(bool right) override;
    void focusInBig(bool match(const Ast*)) override;
    void focusInBig(Ast::Type match) override;
    void dollyOutBig(bool match(const Ast*)) override;
    void dollyOutBig(Ast::Type match) override;
    void insert(Ast::Type type, int bop) override;
    void append(Ast::Type type, int bop) override;
    void assart(Ast::Type type, int bop) override;
    void remove() override;
    void change(Ast *a) override;
    void change(Ast::Type type) override;
    void nestAsLeft(Ast::Type type, int bop) override;
    void nestAsRight(Ast::Type type, int bop) override;
    void expose() override;
    void cast(Ast::Type type) override;

    Mode *createModifyMode(bool clear, size_t offset, bool macroContext) override;
    void scalarAppend(const char *str) override;
    void scalarAppend(char c) override;
    void scalarClear() override;
    void listClear() override;

    void toggleAbstract() override;
    void toggleFinal() override;
    void toggleAccess(bool up) override;
    void toggleStatic() override;
    void toggleTransient() override;
    void toggleVolatile() override;
    void toggleNative() override;
    void toggleSynchronized() override;

    void setHotLight(HotLightLevel level) override;
    void toggleTension(bool b) override;
    ///@}

    /// @name "really" private functions
    ///@{
    void setBop(BopListAst &blist, size_t pos, int bop);
    Ast *newTree(Ast::Type type);
    ///@}

private:
    std::vector<std::unique_ptr<Mode>> modes;
    std::unique_ptr<RootAst> root = nullptr;
    InternalAst *outer = nullptr;
    size_t inner = 0;
    std::unique_ptr<Ast> clipboard = nullptr;
    Tokens tokens;
    PDoc &ob;
};

#endif // DOC_H
