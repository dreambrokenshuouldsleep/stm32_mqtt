#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    client = new QMqttClient(this);

    QObject::connect(client, &QMqttClient::connected , this , &Widget::connect_mqtt);

    ui->comboBox->addItem("烟雾阈值");
    ui->comboBox->addItem("光照阈值");
    ui->comboBox->addItem("温度阈值");
    ui->comboBox->addItem("湿度阈值");
    this->setAttribute(Qt::WA_StyledBackground);
    model = new QStandardItemModel(this);
    creat_list();
}

Widget::~Widget()
{
    db.close();
    delete ui;
}

void Widget::creat_list()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("用户"));
    model->setHorizontalHeaderItem(1,new QStandardItem("时间"));
    model->setHorizontalHeaderItem(2,new QStandardItem("烟雾浓度"));
    model->setHorizontalHeaderItem(3,new QStandardItem("光照强度"));
    model->setHorizontalHeaderItem(4,new QStandardItem("温度"));
    model->setHorizontalHeaderItem(5,new QStandardItem("湿度"));
    model->setHorizontalHeaderItem(6,new QStandardItem("有无停留"));
    model->setHorizontalHeaderItem(7,new QStandardItem("异常类型"));
    ui->tableView->setModel(model);
    //添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //指定数据库驱动路径
    db.setDatabaseName("stu.db");

    if(!db.open()){
        qDebug()<<"打开数据库失败";
    }
    QString sql = QString("create table if not exists home(id ntext,time ntext,MQ ntext,"
                          "GY ntext,temp ntext,humi ntext,staylong ntext,type ntext)");
    QSqlQuery query;
    if(!query.exec(sql)){
        qDebug()<<"建表失败";
    }
}
void Widget::on_pushButton_connect_clicked()
{
    client->setHostname(ui->lineEdit_hostaddr->text());
    client->setPort(ui->lineEdit_hostport->text().toInt());
    client->setUsername(ui->lineEdit_usrname->text());
    client->setPassword(ui->lineEdit_password->text());
    client->setClientId(ui->lineEdit_client->text());
    client->connectToHost();
}

void Widget::on_pushButton_sub_clicked()
{
    client->subscribe(ui->lineEdit_sub->text());
}

void Widget::on_pushButton_pub_clicked()
{
    QString msg = ui->msgEitd_pub->text();
    QByteArray ba;
    ba.append(msg);
    client->publish(ui->popTopic->text(),ba);
}

void Widget::connect_mqtt()
{
    ui->pushButton_connect->setEnabled(false);
    QMessageBox::warning(this, tr("连接提示"),
                                       tr("连接成功！"),
                                       QMessageBox::Save);
    connect(client , &QMqttClient::messageReceived ,this , &Widget::recv_mqtt);
    connect(client , &QMqttClient::disconnected , [this]()
    {
        QMessageBox::warning(this, tr("连接提示"),
                                       tr("断开连接！"),
                                       QMessageBox::Save);
        ui->pushButton_connect->setEnabled(true);
    });
}

void Widget::recv_mqtt(const QString msg,const QMqttTopicName topic)
{

    //ui->textEdit_recvmsg->setText(topic.name()+msg);
    QString buf = msg;
    qDebug() << topic <<buf;
    if(buf.indexOf("Warn") == -1)
    {
        QStringList list = buf.split(",");
        if(buf.indexOf("gy=")!=0)
        {
             QString gy = list[0].remove(":gy=");
             ui->lineEdit_GY->setText(gy);
        }

        if(buf.indexOf("ppm=")!=0)
        {
             QString ppm = list[1].remove("ppm=");
             ui->lineEdit_PPM->setText(ppm);
        }

        if(buf.indexOf("temp=")!=0)
        {
             QString temp = list[2].remove("temp=");
             ui->lineEdit_TEMP->setText(temp);
        }

        if(buf.indexOf("humi=")!=0)
        {
             QString humi = list[3].remove("humi=");
             ui->lineEdit_HUMI->setText(humi);
        }
    }
    else {
        QStringList list = buf.split(",");
        QString gy,ppm,temp,humi,usr,time,stay,type;
        if(buf.indexOf("gy=")!=0)
        {
             gy = list[0].remove("Warn:gy=");
        }

        if(buf.indexOf("ppm=")!=0)
        {
             ppm = list[1].remove("ppm=");
        }

        if(buf.indexOf("temp=")!=0)
        {
             temp = list[2].remove("temp=");
        }

        if(buf.indexOf("humi=")!=0)
        {
             humi = list[3].remove("humi=");
        }

        if(buf.indexOf("usr=")!=0)
        {
             usr = list[4].remove("usr=");
        }

        if(buf.indexOf("time=")!=0)
        {
            time = list[5].remove("time=");
        }

        if(buf.indexOf("stay=")!=0)
        {
             stay = list[6].remove("stay=");
        }
        if(buf.indexOf("type=")!=0)
        {
             type = list[7].remove("type=");
             if(type == "GY")
             {
                type = "光照异常";
             }
             else if(type == "MQ"){
                 type = "烟雾异常";
             }
             else if(type == "TEMP"){
                 type = "温度异常";
             }
             else if(type == "HUMI"){
                 type = "湿度异常";
             }
             else if(type == "ERR")
             {
                 type = "无效卡";
             }
             else
             {
                 type = "正常";
             }
        }
         QString sql = QString("insert into home values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(usr)
                 .arg(time).arg(ppm).arg(gy).arg(temp).arg(humi).arg(stay).arg(type);

         QSqlQuery query;
         if(!query.exec(sql)){
             qDebug()<<"插入失败";

         }
        update_sqldate();


    }



}

void Widget::update_sqldate()
{
    QString sql = QString("select * from  home;");

    quint16 count = 0;

    //删除之前显示
    this->model->removeRows(0,this->model->rowCount());

    QSqlQuery query;
    if(!query.exec(sql)){
        qDebug()<<"查询失败";
    }

    //获取查询结果
    while(query.next()){
        model->setItem(count,0,new QStandardItem(query.value(0).toString()));
        model->setItem(count,1,new QStandardItem(query.value(1).toString()));
        model->setItem(count,2,new QStandardItem(query.value(2).toString()));
        model->setItem(count,3,new QStandardItem(query.value(3).toString()));
        model->setItem(count,4,new QStandardItem(query.value(4).toString()));
        model->setItem(count,5,new QStandardItem(query.value(5).toString()));
        model->setItem(count,6,new QStandardItem(query.value(6).toString()));
        model->setItem(count,7,new QStandardItem(query.value(7).toString()));
        count++;
    }
}
void Widget::on_pushButton_setvalue_clicked()
{
    if(ui->comboBox->currentText() == "烟雾阈值")
    {
        QString MQ = "Data:mq=" + ui->lineEdit_setvalue->text()+",";
        QByteArray ba;
        ba.append(MQ);
        client->publish(ui->popTopic->text(),ba);
    }
    else if(ui->comboBox->currentText() == "光照阈值")
    {
        QString GY = "Data:GY=" + ui->lineEdit_setvalue->text()+",";
        QByteArray ba;
        ba.append(GY);
        client->publish(ui->popTopic->text(),ba);
    }
    else if(ui->comboBox->currentText() == "温度阈值")
    {
        QString WD = "Data:WD=" + ui->lineEdit_setvalue->text()+",";
        QByteArray ba;
        ba.append(WD);
        client->publish(ui->popTopic->text(),ba);
    }
    else if(ui->comboBox->currentText() == "湿度阈值")
    {
        QString RH = "Data:RH=" + ui->lineEdit_setvalue->text()+",";
        QByteArray ba;
        ba.append(RH);
        client->publish(ui->popTopic->text(),ba);
    }
}

void Widget::on_pushButton_open_clicked()
{
    QString OPEN = "Data:OPEN=1,";
    QByteArray ba;
    ba.append(OPEN);
    client->publish(ui->popTopic->text(),ba);
}

void Widget::on_pushButton_addcard_clicked()
{
    QString doc = ui->lineEdit_add->text();
    bool ok;
    int doc1 = doc.toInt(&ok , 16);
    QString ADD = QString("Data:ADD=%1,").arg(doc1) ;
    QByteArray ba;
    ba.append(ADD);
    client->publish(ui->popTopic->text(),ba);
}

void Widget::on_pushButton_delcard_clicked()
{
    QString doc = ui->lineEdit_add->text();
    bool ok;
    int doc1 = doc.toInt(&ok , 16);
    QString DEL = QString("Data:DEL=%1,").arg(doc1) ;
    QByteArray ba;
    ba.append(DEL);
    client->publish(ui->popTopic->text(),ba);
}
