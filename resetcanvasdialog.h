#ifndef RESETCANVASDIALOG_H
#define RESETCANVASDIALOG_H

#include <QDialog>

namespace Ui {
class resetCanvasDialog;
}

class resetCanvasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resetCanvasDialog(QWidget *parent = nullptr);
    ~resetCanvasDialog();
    int getWidth();
    int getHeight();
private:
    Ui::resetCanvasDialog *ui;
};

#endif // RESETCANVASDIALOG_H
