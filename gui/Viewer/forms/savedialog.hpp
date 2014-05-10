#ifndef SAVEDIALOG_HPP
#define SAVEDIALOG_HPP

#include <QDialog>

namespace Ui {
class saveDialog;
}

class saveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit saveDialog(QWidget *parent = 0);
    ~saveDialog();

    void close();

private:
    Ui::saveDialog *ui;

private slots:
    void setProgressText(QString text);
    void setProgress(int val);
};

#endif // SAVEDIALOG_HPP
