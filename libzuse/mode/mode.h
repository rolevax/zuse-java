#ifndef ZUSE_MODE_H
#define ZUSE_MODE_H

#include "../core/keycode.h"
#include "../ast/ast.h"



namespace zuse
{



class DocEditable;



///
/// \brief Base class for editing modes
///
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
        std::unique_ptr<Mode> nextPush;
        bool isDone() const { return type != ResultType::RAISE_POP; }
        bool toPop() const { return type != ResultType::DONE_STAY; }

        static Result doneStayNoPush()
        {
            return { ResultType::DONE_STAY, nullptr };
        }

        static Result donePopNoPush()
        {
            return { ResultType::DONE_POP, nullptr };
        }

        static Result raisePopNoPush()
        {
            return { ResultType::RAISE_POP, nullptr };
        }
    };

    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;
    virtual ~Mode() = default;

    virtual Result keyboard(Key key) { (void) key; return Result::doneStayNoPush(); }
    virtual Result onPushed()  { return Result::doneStayNoPush(); }
    virtual void onPopped() {}
    virtual Result onResume() { return Result::doneStayNoPush(); }
    virtual const char *name() = 0;

protected:
    Mode(DocEditable &doc) : mDoc(doc) {}
    DocEditable &mDoc;
};



} // namespace zuse



#endif // ZUSE_MODE_H
