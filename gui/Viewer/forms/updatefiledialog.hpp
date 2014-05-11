#ifndef UPDATEFILEDIALOG_HPP
#define UPDATEFILEDIALOG_HPP

#include <objects/sockreceiver.hpp>

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class updateFileDialog;
}

class updateFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit updateFileDialog(QWidget *parent = 0);
    ~updateFileDialog();

    void askFile(int idDoctor, int idPatient = -1, std::string filename = "");

private:
    Ui::updateFileDialog *ui;
    QVector<getFile*> dataList;

private slots:
    void fileRcv(getFile *fileRecv);
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // UPDATEFILEDIALOG_HPP
