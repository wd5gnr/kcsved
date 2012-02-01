#ifndef EDITROW_H
#define EDITROW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "qcsvlineedit.h"
#include "qcsvpushbutton.h"


class EditRow
{
public:
    EditRow(void);
    ~EditRow();
    // Add a row
    void AddRow(QWidget *parent);
    // Access components
    QLineEdit *label(void) { return _label; }
    QPushButton *vButton(void) { return _vButton; }
    QPushButton *eButton(void) { return _eButton; }
    QCSVLineEdit *edit(void) { return _edit; }
protected:
    QHBoxLayout *box;
    QLineEdit *_label;
    QCSVPushButton *_vButton;
    QCSVPushButton *_eButton;
    QCSVLineEdit *_edit;
};

#endif // EDITROW_H
