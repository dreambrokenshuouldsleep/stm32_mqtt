#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMqtt/qmqttclient.h>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void creat_list();
    void update_sqldate();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_sub_clicked();

    void on_pushButton_pub_clicked();

    void connect_mqtt();

    void recv_mqtt(const QString msg,const QMqttTopicName topic);

    void on_pushButton_setvalue_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_addcard_clicked();

    void on_pushButton_delcard_clicked();

private:
    Ui::Widget *ui;
    QMqttClient * client;
    QStandardItemModel *model;
    QSqlDatabase db;
};
#endif // WIDGET_H
