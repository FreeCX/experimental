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

void MainWindow::on_list_currentRowChanged(int currentRow)
{
    list_t *p = base;
    for ( int i = 0; i < currentRow; i++ ) {
        p = p->next;
    }
    ui->e_org_name->setText( p->org_name );
    ui->e_rus_name->setText( p->rus_name );
    ui->s_series->setValue( p->series );
    ui->comboBox->setCurrentIndex( p->status );
}

void MainWindow::on_b_change_clicked()
{
    int selected = ui->list->currentIndex().row();
    list_t *p = base;
    for ( int i = 0; i < selected; i++ ) {
        p = p->next;
    }
    free( p->org_name );
    free( p->rus_name );
    p->org_name = (char *) malloc( ui->e_org_name->text().length() );
    p->rus_name = (char *) malloc( ui->e_rus_name->text().length() );
    strcpy( p->org_name, ui->e_org_name->text().toUtf8().data() );
    strcpy( p->rus_name, ui->e_rus_name->text().toUtf8().data() );
    p->series = ui->s_series->value();
    p->status = ui->comboBox->currentIndex();
    ui->list->item( selected )->setText( p->org_name );
}
