#include "mainwindow.h"
#include "ui_mainwindow.h"
extern "C" {
#include "logic.h"
}
#include <iostream>
#include <cstring>

list_t *base = NULL;
char file_list[] = "anime-list";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    list_t *p;
    base = list_load( file_list );
    p = base;
    ui->setupUi(this);
    while ( p != NULL ) {
        ui->list->addItem( p->org_name );
        p = p->next;
    }
    int i = 0;
    while ( list_status( i ) != NULL ) {
        ui->comboBox->addItem( list_status( i ) );
        i++;
    }
}

MainWindow::~MainWindow()
{
    list_clean( base );
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    exit(0);
}

void MainWindow::on_list_clicked(const QModelIndex &index)
{
    list_t *p = base;
    for ( int i = 0; i < index.row(); i++ ) {
        p = p->next;
    }
    ui->e_org_name->setText( p->org_name );
    ui->e_rus_name->setText( p->rus_name );
    ui->s_series->setValue( p->series );
    ui->comboBox->setCurrentIndex( p->status );
}
