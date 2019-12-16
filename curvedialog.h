#ifndef CURVEDIALOG_H
#define CURVEDIALOG_H

#include <QDialog>

namespace Ui {
class CurveDialog;
}

class CurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurveDialog(QWidget *parent = nullptr);
    ~CurveDialog();
    int getCtlNum();
    QString getCurveType();

private:
    Ui::CurveDialog *ui;
};

#endif // CURVEDIALOG_H
