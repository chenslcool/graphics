#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include <QDialog>

namespace Ui {
class LineDialog;
}

class LineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineDialog(QWidget *parent = nullptr);
    ~LineDialog();
    QString getLineType();
private:
    Ui::LineDialog *ui;
};

#endif // LINEDIALOG_H
