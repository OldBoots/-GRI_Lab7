#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QBitArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Info
{
    int size_img;
    int width;
    int hieght;
    int begin;
    Info(){}
    Info(int s, int w, int h, int b)
    {
        size_img = s;
        width = w;
        hieght = h;
        begin = b;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void load_img_slot();
    void load_text_slot();
    void write_file_slot();
private:
    int sum_Byts(QByteArray arr, bool forward);
private:
    Ui::MainWindow *ui;
    QString path_img;
    Info inf;
    QByteArray arr_img;
    QBitArray bit_arr_text;
    int max_size_text;
    int size_text;
};
#endif // MAINWINDOW_H
