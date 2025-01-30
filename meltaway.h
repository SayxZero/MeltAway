#ifndef MELTAWAY_H
#define MELTAWAY_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MeltAway; }
QT_END_NAMESPACE

class MeltAway : public QMainWindow
{
    Q_OBJECT

public:
    MeltAway(QWidget *parent = nullptr);
    ~MeltAway();

private slots:
    void on_OpenButton_clicked();

    void on_action_exit_triggered();

    void on_action_setViewType_solid_triggered();

    void on_action_setViewType_wire_triggered();

    void TimerSlot();

    void AcceptLineEdit();

private:
    Ui::MeltAway *ui;
    QTimer *timer;

};
#endif // MELTAWAY_H
