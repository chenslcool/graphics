#include "clipdialog.h"
#include "ui_clipdialog.h"

clipDialog::clipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::clipDialog)
{
    ui->setupUi(this);
}

clipDialog::~clipDialog()
{
    delete ui;
}

QString clipDialog::getClipTyoe()
{
    return ui->comboBoxClipType->currentText();
}
