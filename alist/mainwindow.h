#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_b_add_clicked();

    void on_pushButton_clicked();

    void on_list_clicked(const QModelIndex &index);

    void on_list_pressed(const QModelIndex &index);

    void on_list_activated(const QModelIndex &index);

    void on_list_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
