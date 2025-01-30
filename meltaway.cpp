#include "meltaway.h"
#include "ui_meltaway.h"

MeltAway::MeltAway(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MeltAway)
{
    ui->setupUi(this);
    ui->lineEdit->setText("D:/Prog/TrialOGL3D/mdls/fuell.stl");
//    ui->OpenButton->click();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(1);
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(AcceptLineEdit()));
}

MeltAway::~MeltAway()
{
    delete ui;
}

void MeltAway::on_OpenButton_clicked()
{
    ui->openGLWidget->setGeometry(ui->lineEdit->text());
}


void MeltAway::on_action_exit_triggered()
{
    close();
}


void MeltAway::on_action_setViewType_solid_triggered()
{
    ui->action_setViewType_wire->setChecked(false);
    ui->openGLWidget->setViewType(0);
}


void MeltAway::on_action_setViewType_wire_triggered()
{
    ui->action_setViewType_solid->setChecked(false);
    ui->openGLWidget->setViewType(1);
}

void MeltAway::TimerSlot()
{
    if (ui->openGLWidget->modelSize() > 0) {
        QVector3D coords = ui->openGLWidget->m_ModelCoords;
        QString str = QString::number(coords.x()) + "; " + QString::number(coords.y()) + "; " + QString::number(coords.z());
        ui->statusbar->showMessage(str, 100);
    }
}

void MeltAway::AcceptLineEdit()
{
    ui->OpenButton->click();
}
