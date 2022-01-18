#ifndef PRINT_H
#define PRINT_H

#include <QDialog>

namespace Ui {
class print;
}

class print : public QDialog
{
    Q_OBJECT

public:
    explicit print(QWidget *parent = nullptr);
    ~print();

private:
    Ui::print *ui;
};

#endif // PRINT_H
