#ifndef QCSVLINEEDIT_H
#define QCSVLINEEDIT_H

#include <QLineEdit>

class QCSVLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QCSVLineEdit(QWidget *parent = 0);

signals:
    void focussed(bool hasFocus);

public slots:

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

};

#endif // QCSVLINEEDIT_H
