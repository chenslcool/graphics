#include "polygondialog.h"
#include "ui_polygondialog.h"

PolygonDialog::PolygonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PolygonDialog)
{
    ui->setupUi(this);
}

PolygonDialog::~PolygonDialog()
{
    delete ui;
}

int PolygonDialog::getLineNum()
{
    return ui->spinBoxPolygonLineNum->text().toInt();
}
