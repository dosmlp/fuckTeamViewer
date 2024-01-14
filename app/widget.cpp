#include "widget.h"
#include <QDebug>
#include <QMessageBox>

widget::widget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    e.seed(static_cast<uint32_t>(QDateTime::currentMSecsSinceEpoch()));
    loadRndBin();
}

widget::~widget() {
}

void widget::doInjector() {
    tv_path_ = ui_lineEdit_tvPath_->text();
    if (tv_path_.isEmpty()) return;

    STARTUPINFO         si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    wchar_t tv_path[ 512 ] = {0};
    tv_path_.toWCharArray(tv_path);
    if (CreateProcess(tv_path, NULL, NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, &si, &pi) ==
        0) {
        QMessageBox::information(this, "提示", "启动失败");
        return;
    }
    NTSTATUS nt = RhInjectLibrary(pi.dwProcessId, 0, EASYHOOK_INJECT_DEFAULT, (WCHAR *)L"fk.dll",
                                  nullptr, rndbin_, 128);

    if (nt != 0) {
        QMessageBox::information(this, QString::number(pi.dwProcessId),
                                 QString::fromWCharArray(RtlGetLastErrorString()));
        return;
    }
    ResumeThread(pi.hThread);
}

void widget::loadRndBin() {
    QFileInfo rndFileInfo("RND.bin");

    if (!rndFileInfo.exists() || rndFileInfo.size() < 128) {
        regenRndBin();
        return;
    }
    QFile rndFile("RND.bin");
    if (rndFile.open(QIODevice::ReadOnly)) {
        rndFile.read(rndbin_, 128);
        rndFile.close();
        return;
    }
    QMessageBox::information(this, "提示", "配置读取失败");
}

QByteArray widget::random8Byte() {

    uint32_t   r1 = e();
    uint32_t   r2 = e();
    QByteArray rnd8byte(8, 0x00);
    memcpy(rnd8byte.data(), &r1, 4);
    memcpy(rnd8byte.data() + 4, &r2, 4);
    return rnd8byte;
}

void widget::explorerFile() {
    QString fileName =QFileDialog::getOpenFileName(this, "打开", "C:/", "TeamViewer.exe");
    if (fileName.isEmpty())
        return;
    ui_lineEdit_tvPath_->setText(fileName);
}

void widget::regenRndBin() {
    QFile rndFile("RND.bin");
    if (rndFile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        for (int i = 0; i < 16; ++i) {
            QByteArray b = random8Byte();
            memcpy(rndbin_ + i * 8, b.data(), 8);
        }
        rndFile.write(rndbin_, 128);
        rndFile.close();
        QSettings tvreg("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node", QSettings::NativeFormat);
        tvreg.remove("TeamViewer");
    }
}

void widget::setupUi() {
    ui_layout_main_     = new QGridLayout();
    ui_label_tvPath_    = new QLabel("teamviewer路径:", this);
    ui_lineEdit_tvPath_ = new QLineEdit(this);
    ui_button_tvFile_   = new QPushButton("手动选择", this);

    ui_layout_main_->addWidget(ui_label_tvPath_, 0, 0);
    ui_layout_main_->addWidget(ui_lineEdit_tvPath_, 0, 1);
    ui_layout_main_->addWidget(ui_button_tvFile_, 0, 2);

    ui_button_regen_ = new QPushButton("重新生成", this);
    ui_button_start_ = new QPushButton("启动", this);
    ui_layout_main_->addWidget(ui_button_regen_, 1, 1);
    ui_layout_main_->addWidget(ui_button_start_, 1, 2);
    setLayout(ui_layout_main_);

    connect(ui_button_tvFile_, &QPushButton::clicked, this, &widget::explorerFile);
    connect(ui_button_regen_, &QPushButton::clicked, this, &widget::regenRndBin);
    connect(ui_button_start_,&QPushButton::clicked,this,&widget::doInjector);

    for (int i = 0; i < ui_layout_main_->count(); ++i) {
        ui_layout_main_->itemAt(i)->widget()->setMaximumSize(500, 500);
    }
}
