/**
 * @file registerdialog.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 用户注册页
 */
#ifndef REGISTERDIALO`G_H
#define REGISTERDIALOG_H
#include <QDialog>

namespace Ui {
class registerDialog;
}
/**
 * @brief QT用户注册页
 * 
 */
class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = nullptr);
    ~registerDialog();

private slots:
    /// 注册按钮触发
    void on_pushButton_clicked();
    
private:
    Ui::registerDialog *ui;
    /// 错误提醒
    void give_hint(QString qs);
};

#endif // REGISTERDIALOG_H
