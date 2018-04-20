#include "p_doc.h"

#include <Qt>
#include <QKeySequence>
#include <QChar>

#include <QDebug>

PDoc::PDoc(QObject *parent) :
    QObject(parent),
    mDoc(*this)
{
}

void PDoc::load(QString filename)
{
    try {
        mDoc.load(filename.toStdString());
    } catch (const std::exception &e) {
        emit message(e.what());
    }
}

void PDoc::save(QString filename)
{
    try {
        mDoc.save(filename.toStdString());
    } catch (const std::exception &e) {
        emit message(e.what());
    }
}

void PDoc::keyboard(QString key, int modifier)
{
    if (key.size() > 0) {
        char c = key.at(0).toLatin1();
        Key k = KeyCode::fromChar(c);
        if (modifier & Qt::ShiftModifier)
            k = KeyCode::makeShifted(k);
        mDoc.keyboard(k);
    }
}

void PDoc::onLineInserted(size_t r, size_t ct)
{
    emit insertLine(r, ct);
}

void PDoc::onLineRemoved(size_t r, size_t ct)
{
    emit removeLine(r, ct);
}

void PDoc::onLineUpdated(size_t r, const std::string &s)
{
    emit updateLine(r, QString::fromStdString(s));
}

void PDoc::onLight(size_t lbr, size_t lbc,
                            size_t ler, size_t lec,
                            size_t hbr, size_t hbc,
                            size_t her, size_t hec)
{
    emit lighted(lbr, lbc, ler, lec, hbr, hbc, her, hec);
}

void PDoc::onHotLight(ssize_t back)
{
    emit hotLighted(back);
}

void PDoc::onCursorTension(bool b)
{
    emit tension(b);
}

void PDoc::onModePushed(const char *name)
{
    emit pushed(name);
}

void PDoc::onModePopped()
{
    emit popped();
}

void PDoc::onClipSwitched(char c)
{
    emit clipSwitched(c);
}
