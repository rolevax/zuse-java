#ifndef ZUSE_DOC_H
#define ZUSE_DOC_H

#include "doc_editable.h"
#include "keycode.h"
#include "tokens.h"

#include "../ast/ast_root.h"



namespace zuse
{



class AstListBop;

class Doc : public DocEditable
{
public:
    explicit Doc(DocListener &listener);

    Doc(const Doc&) = delete;
    Doc &operator=(const Doc&) = delete;

    void load(const std::string &filename);
    void save(const std::string &filename);
    void keyboard(Key key); // TODO XXX: change here, and be a M

private:
    void push(std::unique_ptr<Mode> mode);
    void pop();
    void handleModeResult(Mode::Result res);

    /// \name DocEditable interface
    ///@{
    const AstInternal &getOuter() const override;
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
    void siblingBig(bool match(const Ast*), bool right) override;
    void siblingBig(Ast::Type match, bool right) override;
    void insert(Ast::Type type, Bop bop) override;
    void append(Ast::Type type, Bop bop) override;
    void assart(Ast::Type type, Bop bop) override;
    void remove() override;
    void change(std::unique_ptr<Ast> a) override;
    void change(Ast::Type type) override;
    void nestAsLeft(Ast::Type type, Bop bop) override;
    void nestAsRight(Ast::Type type, Bop bop) override;
    void expose() override;
    void cast(Ast::Type type) override;

    std::unique_ptr<Mode> createModifyMode(bool clear, size_t offset, bool macroContext) override;
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

    void switchClip(char c) override;
    void yank(const Ast &a) override;
    void paste() override;

    void setHotLight(HotLightLevel level) override;
    void toggleTension(bool b) override;
    ///@}

    /// \name internal detail functions
    ///@{
    void setBop(AstListBop &blist, size_t pos, Bop bop);
    std::unique_ptr<Ast> newTree(Ast::Type type);
    std::unique_ptr<AstInternal> newInternalTree(Ast::Type type);
    ///@}

private:
    std::vector<std::unique_ptr<Mode>> mModes;
    std::unique_ptr<AstRoot> mRoot;
    util::Observer<AstInternal *> mOuter = nullptr;
    size_t mInner = 0;
    std::array<std::unique_ptr<Ast>, 26> mClipslots;
    size_t mClipIndex = 0;
    Tokens mTokens;
    DocListener &mListener;
};



} // namespace zuse



#endif // ZUSE_DOC_H
