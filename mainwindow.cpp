#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    size_text = 0;
    max_size_text = 0;
    connect(ui->butt_load_img, SIGNAL(clicked()), SLOT(load_img_slot()));
    connect(ui->butt_load_text, SIGNAL(clicked()), SLOT(load_text_slot()));
    connect(ui->butt_start, SIGNAL(clicked()), SLOT(write_file_slot()));
}

int MainWindow::sum_Byts(QByteArray arr, bool forward)
{
    if(forward){
        return QByteArray::fromRawData(arr, arr.size()).toHex().toInt(nullptr, 16);
    }else{
        int n = arr.size();
        if(n % 2){
            n--;
        }
        for(int i = 0; i < n / 2; i++){
            qSwap(arr[i], arr[arr.size() - 1 - i]);
        }
        return QByteArray::fromRawData(arr, arr.size()).toHex().toInt(nullptr, 16);
    }
}

void MainWindow::load_img_slot(){
    path_img = QFileDialog::getOpenFileName();
    ui->label_img->setPixmap(QPixmap(path_img).scaled(400,
                                                    400,
                                                    Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation));
    QFile file(path_img);
    file.open(QIODevice::ReadOnly);
    arr_img = file.readAll();
    file.close();
    inf.size_img = sum_Byts(QByteArray::fromRawData(&arr_img[2], 4), false);
    inf.width = sum_Byts(QByteArray::fromRawData(&arr_img[18], 4), false);
    inf.hieght = sum_Byts(QByteArray::fromRawData(&arr_img[22], 4), false);
    inf.begin = sum_Byts(QByteArray::fromRawData(&arr_img[10], 4), false);
}

void MainWindow::load_text_slot()
{
    QByteArray byte_arr_text;
    QFile file(QFileDialog::getOpenFileName());
    file.open(QIODevice::ReadOnly);
    byte_arr_text = file.readAll();
    file.close();
    if(byte_arr_text.isEmpty()){
        ui->butt_load_text->setStyleSheet("background-color: red");
        return;
    }
    ui->butt_load_text->setStyleSheet("background-color: green");
    size_text = byte_arr_text.size();
    bit_arr_text = QBitArray::fromBits(byte_arr_text, byte_arr_text.size() * 8);
    ui->textEdit_2->clear();
}

void MainWindow::write_file_slot(){
    max_size_text = inf.size_img * 0.1;
    qDebug() << size_text << max_size_text;
    if(size_text > max_size_text || size_text == 0){
        return;
    }
//    int size_mas_pix = inf.size_img - inf.begin;
    QBitArray byte;
    for(int i = 0, index_bit = 0; i < 4 && index_bit < bit_arr_text.size(); i++){
        for(int index_byte = inf.begin; index_byte < inf.size_img && index_bit < bit_arr_text.size(); index_byte++, index_bit++){
            byte = QBitArray::fromBits(&arr_img[index_byte], 8);
            byte[i] = bit_arr_text[index_bit];
            arr_img[index_byte] = byte.bits()[0];
        }
    }
    QByteArray byte_arr_text;
    bit_arr_text.fill(false);
    for(int i = 0, index_bit = 0; i < 4 && index_bit < bit_arr_text.size(); i++){
        for(int index_byte = inf.begin; index_byte < inf.size_img && index_bit < bit_arr_text.size(); index_byte++, index_bit++){
            byte = QBitArray::fromBits(&arr_img[index_byte], 8);
            bit_arr_text[index_bit] = byte[i];
        }
    }
    byte_arr_text = QByteArray::fromRawData(bit_arr_text.bits(), bit_arr_text.size() / 8);
    ui->textEdit_2->setText(QString::fromStdString(byte_arr_text.toStdString()));
    QFile file("C:\\Qt\\Project\\GRI_Lab7\\text.txt");
    file.open(QIODevice::WriteOnly);
    for(int i = 0; i < byte_arr_text.size(); i++){
        file.putChar(byte_arr_text[i]);
    }
    file.close();
    file.setFileName("C:\\Qt\\Project\\GRI_Lab7\\result.bmp");
    file.open(QIODevice::WriteOnly);
    for(int i = 0; i < arr_img.size(); i++){
        file.putChar(arr_img[i]);
    }
    file.close();
    ui->label_img->setPixmap(QPixmap("C:\\Qt\\Project\\GRI_Lab7\\result.bmp").scaled(400,
                                                                                     400,
                                                                                     Qt::KeepAspectRatio,
                                                                                     Qt::SmoothTransformation));
}

MainWindow::~MainWindow()
{
    delete ui;
}

