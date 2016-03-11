#ifndef MODE_H
#define MODE_H

#include "ast/ast.h"

class EditableDoc;

class Mode
{
public:
    enum class ResultType
    {
        THROW, STAY, POP
    };

    struct Result
    {
        ResultType type;
        Mode *nextPush;
    };

    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;
    virtual ~Mode() = default;

    virtual Result keyboard(char key)
    {
        (void) key;
        return { ResultType::THROW, nullptr };
    }
    virtual void onPushed() {}
    virtual void onPopped() {}
    virtual void onResume() {}
    virtual const char *name() = 0;

protected:
    Mode(EditableDoc &doc) : doc(doc) {}
    EditableDoc &doc;
};

#endif // MODE_H
