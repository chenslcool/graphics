#ifndef CLIPDIALOG_H
#define CLIPDIALOG_H

#include <QDialog>

namespace Ui {
class clipDialog;
}

class clipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit clipDialog(QWidget *parent = nullptr);
    ~clipDialog();
    QString getClipTyoe();
private:
    Ui::clipDialog *ui;
};

#endif // CLIPDIALOG_H
