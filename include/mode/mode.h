#ifndef MODE_H
#define MODE_H

#include "ast/ast.h"

class EditableDoc;

class Mode
{
public:
    enum class ResultType
    {
        DONE_STAY, DONE_POP, RAISE_POP
    };

    struct Result
    {
        ResultType type;
        Mode *nextPush;
        bool handled() const { return type != ResultType::RAISE_POP; }
        bool toPop() const { return type != ResultType::DONE_STAY; }
    };

    static constexpr Result DONE_STAY_NOPUSH = { ResultType::DONE_STAY, nullptr };
    static constexpr Result DONE_POP_NOPUSH = { ResultType::DONE_POP, nullptr };
    static constexpr Result RAISE_POP_NOPUSH = { ResultType::RAISE_POP, nullptr };

    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;
    virtual ~Mode() = default;

    virtual Result keyboard(char key) { (void) key; return DONE_STAY_NOPUSH; }
    virtual Result onPushed()  { return DONE_STAY_NOPUSH; }
    virtual void onPopped() {}
    virtual void onResume() {}
    virtual const char *name() = 0;

protected:
    Mode(EditableDoc &doc) : doc(doc) {}
    EditableDoc &doc;
};

#endif // MODE_H
