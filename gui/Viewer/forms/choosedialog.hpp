#ifndef CHOOSEDIALOG_HPP
#define CHOOSEDIALOG_HPP

#include <QDialog>

namespace Ui {
class chooseDialog;
}

class chooseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit chooseDialog(QWidget *parent = 0);
    ~chooseDialog();
    
private:
    Ui::chooseDialog *ui;
};

#endif // CHOOSEDIALOG_HPP
