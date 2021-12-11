#ifndef ACTIVECONFIGDIALOG_H
#define ACTIVECONFIGDIALOG_H

#include <QDialog>


namespace Ui {
class ActiveConfigDialog;
}

class ActiveConfigDialog : public QDialog{
        Q_OBJECT

    public:
        explicit ActiveConfigDialog(QWidget *parent = nullptr);
        ~ActiveConfigDialog();

    private slots:
        void on_radioButtonCustomNameservers_clicked();
        void on_radioButtonRandomNameservers_clicked();
        void on_radioButtonSingleNameserver_clicked();

private:
        Ui::ActiveConfigDialog *ui;
};

#endif // ACTIVECONFIGDIALOG_H