#ifndef P_DOC_H
#define P_DOC_H

#include "libzuse/zuse.h"

#include <QObject>



class PDoc : public QObject, public DocListener
{
    Q_OBJECT
public:
    explicit PDoc(QObject *parent = nullptr);

    Q_INVOKABLE void load(QString filename);
    Q_INVOKABLE void save(QString filename);
    Q_INVOKABLE void keyboard(QString key, int modifier);

    void onLineInserted(size_t r, size_t ct) override;
    void onLineRemoved(size_t r, size_t ct) override;
    void onLineUpdated(size_t r, const std::string &s) override;
    void onLight(size_t lbr, size_t lbc,
                      size_t ler, size_t lec,
                      size_t hbr, size_t hbc,
                      size_t her, size_t hec) override;
    void onHotLight(ssize_t back) override;
    void onCursorTension(bool b) override;
    void onModePushed(const char *name) override;
    void onModePopped() override;
    void onClipSwitched(char c) override;

signals:
    void message(QString text);
    void tension(bool b);
    void pushed(QString name);
    void popped();

    void clipSwitched(char c);

    void insertLine(int r, int ct);
    void removeLine(int r, int ct);
    void updateLine(int r, const QString str);
    void hotLighted(int back);
    void lighted(int lbr, int lbc, int ler, int lec,
                 int hbr, int hbc, int her, int hec);

public slots:

private:
    Doc mDoc;
};



#endif // P_DOC_H
