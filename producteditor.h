#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QDialog>
#include "product.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

#define MAX_FILE_SIZE 20000000

namespace Ui {
class ProductEditor;
}

class ProductEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ProductEditor(QWidget *parent = nullptr);
    ~ProductEditor();

    Product* UItoProduct();
    void ProductToUI(Product *product);

    void reloadList(bool reset = true);

    bool loadImageToUI(QByteArray img);
private slots:
    void on_lineEditSearch_textEdited(const QString &arg1);

    void on_pushButtonAddProduct_clicked();


    void on_listViewProducts_clicked(const QModelIndex &index);


    void on_pushButtonDelete_clicked();

    void on_pushButtonChangePhoto_clicked();

    void on_pushButtonDeletePhoto_clicked();

    void on_pushButtonValidate_clicked();

private:
    Ui::ProductEditor *ui;

    QVector<Product*> products;

    Product *currentProduct;
    int selectedRow;

    QByteArray currentLogo;
};

#endif // PRODUCTEDITOR_H
