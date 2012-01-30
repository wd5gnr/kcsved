#ifndef QCSVPUSHBUTTON_H
#define QCSVPUSHBUTTON_H

#include <QPushButton>

class QCSVPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QCSVPushButton(QWidget *parent = 0);
    explicit QCSVPushButton(const QIcon &icon, const QString &title, QWidget *parent=0);


signals:
    void focussed(bool hasFocus);

public slots:

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

};

#endif // QCSVPUSHBUTTON_H
