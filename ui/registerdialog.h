#ifndef REGISTERDIALO`G_H
#define REGISTERDIALOG_H
#include <QDialog>

namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = nullptr);
    ~registerDialog();

private slots:
    void on_pushButton_clicked();
    
private:
    Ui::registerDialog *ui;
    void give_hint(QString qs);
};

#endif // REGISTERDIALOG_H
