#ifndef CONNEXIONDIALOG_HPP
#define CONNEXIONDIALOG_HPP

#include <QDialog>

namespace Ui {
class connexionDialog;
}

class connexionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit connexionDialog(QWidget *parent = 0);
    ~connexionDialog();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::connexionDialog *ui;
};

#endif // CONNEXIONDIALOG_HPP
