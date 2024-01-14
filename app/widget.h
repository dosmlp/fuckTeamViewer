#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>
extern "C"{
#include <easyhook.h>
}

#include <QProcess>
#include <QFile>
#include <QDateTime>
#include <QGridLayout>
#include <QSettings>
#include <QFileInfo>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QByteArray>
#include <random>

using namespace std;
class widget : public QWidget
{
    Q_OBJECT

public:
    widget(QWidget *parent = 0);
    ~widget();
private:
    void doInjector();
    void loadRndBin();
    QByteArray random8Byte();
    void explorerFile();
    void regenRndBin();
    char rndbin_[128];
    default_random_engine e;
    //ui
    void setupUi();
    QString tv_path_;
    QLabel* ui_label_tvPath_;
    QLineEdit* ui_lineEdit_tvPath_;
    QPushButton* ui_button_tvFile_;
    QGridLayout* ui_layout_main_;
    QPushButton* ui_button_regen_;
    QPushButton* ui_button_start_;
};

#endif // WIDGET_H
