#include "curvedialog.h"
#include "ui_curvedialog.h"

CurveDialog::CurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveDialog)
{
    ui->setupUi(this);
}

CurveDialog::~CurveDialog()
{
    delete ui;
}

int CurveDialog::getCtlNum()
{
    return ui->spinBoxCurveCtlNum->text().toInt();
}

QString CurveDialog::getCurveType()
{
    return ui->comboBoxCurveType->currentText();
}
