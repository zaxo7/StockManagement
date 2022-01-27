#include "producteditor.h"
#include "ui_producteditor.h"

ProductEditor::ProductEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductEditor)
{
    ui->setupUi(this);

    currentProduct = NULL;
    selectedRow = -1;

    Product p;

    p.setId(1);

    p.load();

    //p.insert();

    reloadList();


    QAbstractItemModel *ind = ui->listViewProducts->model();

    on_listViewProducts_clicked(ind->index(selectedRow, 0));
}

ProductEditor::~ProductEditor()
{
    delete ui;
}


Product* ProductEditor::UItoProduct()
{
    Product *product = new Product();

    product->setName(ui->lineEditName->text());
    product->setPrice(ui->lineEditPrice->text().toFloat());
    product->setBrand(ui->lineEditBrand->text());
    product->setBarcode(ui->lineEditBarcode->text());
    product->setDescription(ui->textEditDescription->toPlainText());
    product->setLogo(currentLogo);

    return product;
}

void ProductEditor::ProductToUI(Product *product)
{
    ui->lineEditName->setText(product->getName());
    ui->lineEditPrice->setText(QString::number(product->getPrice()));
    ui->lineEditBrand->setText(product->getBrand());
    ui->lineEditBarcode->setText(product->getBarcode());
    ui->textEditDescription->setText(product->getDescription());

    loadImageToUI(product->getLogo());

}

void ProductEditor::reloadList(bool reset)
{
    if(reset)
    {
        QString filter = ui->lineEditSearch->text().trimmed();


        DatabaseTable productsTable("Product");

        if(!filter.isEmpty())
        {
            QString where = "name LIKE '%" + filter + "%' or brand LIKE '%" + filter + "%' or barcode LIKE '%" + filter + "%'";
            productsTable.setWhereClause(where);
        }

        productsTable.prepareSelect();

        productsTable.exec();

        QSqlQuery results = productsTable.getQuery();

        //qDebug() << QString(" resutls = ") + QString::number(results.) + results.next();

        products.clear();

        while(results.next())
        {
            Product *p = new Product();

            p->loadFromRecord(results.record());

            products.push_back(p);
        }
    }

    QMap<int, QString> data;

    QStandardItemModel *model = new QStandardItemModel();
    QMapIterator<int, QString> it(data);

    for(int i = 0; i < products.count(); i++)
    {
        Product *p = products.at(i);

        if(currentProduct /*&& (currentProduct->getId() != -1)*/ && (currentProduct == p))
        {
            selectedRow = i;
        }

        QStandardItem *item = new QStandardItem;
        item->setText((p->getName().isEmpty() ? "sans nom":p->getName()) + "\t" + p->getBrand() + "\t" + p->getBarcode());
        item->setData((int)p->getId(), Qt::UserRole);

        //qDebug() << "id = " << p->getId();

        model->appendRow(item);
    }

    ui->listViewProducts->setModel(model);

    //reselect last item
    if(selectedRow == -1)
    {
        selectedRow = 0;
    }
    else if(selectedRow >= products.count())
    {
        selectedRow = products.count() - 1;
    }

    QAbstractItemModel *ind = ui->listViewProducts->model();

    ui->listViewProducts->blockSignals(true);
    ui->listViewProducts->clicked(ind->index(selectedRow, 0));
    ui->listViewProducts->setCurrentIndex(ind->index(selectedRow, 0));
    ui->listViewProducts->blockSignals(false);


    enableEditingFourm(products.count() != 0);
}

bool ProductEditor::loadImageToUI(QByteArray pic)
{
    currentLogo = pic;
    QPixmap pixmap;
    if(pic.size() < 10)
    {
        ui->labelLogo->setPixmap(pixmap);
        return true;
    }

    if (!pixmap.loadFromData(pic))
    {
        qDebug() << "error in image";
        return false;
    }

    int width = ui->labelLogo->width();
    int height = ui->labelLogo->height();
    //Show uploaded picture and move the MediaItem to the end
    ui->labelLogo->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));

    return true;
}

void ProductEditor::clearFields()
{
    QObjectList objects = ui->groupBox_caracterestics->children();

    foreach(QObject *obj, objects)
    {
        QString className = obj->metaObject()->className();

        if(className == "QLineEdit")
        {
            ((QLineEdit*)obj)->clear();
        }
        else if(className == "QTextEdit")
        {
            ((QTextEdit*)obj)->clear();
        }
    }
    ui->textEditDescription->clear();

    loadImageToUI(QByteArray());
}

void ProductEditor::enableEditingFourm(bool state)
{
    ui->groupBox_caracterestics->setEnabled(state);
}

void ProductEditor::on_lineEditSearch_textEdited(const QString &arg1)
{
    reloadList();
}


void ProductEditor::on_pushButtonAddProduct_clicked()
{
    Product *p = new Product;
    p->setName("produit");
    products.push_back(p);

    clearFields();

    currentProduct = p;
    selectedRow = products.count() - 1;

    QAbstractItemModel *ind = ui->listViewProducts->model();

    ui->listViewProducts->blockSignals(true);
    on_listViewProducts_clicked(ind->index(selectedRow, 0));
    ui->listViewProducts->setCurrentIndex(ind->index(selectedRow, 0));
    ui->listViewProducts->blockSignals(false);

    reloadList(false);
}


void ProductEditor::on_listViewProducts_clicked(const QModelIndex&index)
{
    if(currentProduct)
    {
        //get the info from UI
        Product *newProduct = UItoProduct();
        //copy the id, from the old product because they are not set with UItoProduct fct
        newProduct->setId(currentProduct->getId());

        if(!ProductFactory::compare(currentProduct, newProduct))
        {
            newProduct->store();

            products.replace(selectedRow, newProduct);

            delete currentProduct;
            currentProduct = newProduct;

            reloadList(false);
        }

    }

    selectedRow = index.row();

    if(selectedRow <= products.size() && (selectedRow >= 0))
    {
        ProductToUI(products.at(selectedRow));
        currentProduct = products.at(selectedRow);
    }

    qDebug() << "clicked on index = " << selectedRow << " id = " << index.data(Qt::UserRole).toInt() << "  //  " << products.size();
}



void ProductEditor::on_pushButtonDelete_clicked()
{
    if(currentProduct)
    {
        currentProduct->deletep();
        products.remove(selectedRow);
        reloadList(false);
    }
}


void ProductEditor::on_pushButtonChangePhoto_clicked()
{
Retry:
    QString file = QFileDialog::getOpenFileName(this,tr("Selectioner une image"),QString(),tr("Images (*.png *.jpg *jpeg)"));
    if (file.isEmpty())
        return;
    QFile f(file);
    if (f.size() > MAX_FILE_SIZE)
    {
        QMessageBox::critical(this,tr("Télechargement de l'image"),QString(tr("La taille maximale de l'image est %1 MegaOctets")).arg(MAX_FILE_SIZE/1024));
        f.close();
        return;
    }
    f.open(QFile::ReadOnly);

    QByteArray pic (f.readAll());

    QPixmap pixmap;
    if (!pixmap.loadFromData(pic))
    {
        //Selected file is not a valid image
        if (QMessageBox::question(this,tr("Erreur das l'image"),tr("Le fichier que vous avez selectioné n'est pas une image")) == QMessageBox::Yes)
            goto Retry;
        else
            return;
    }

    loadImageToUI(pic);

}




void ProductEditor::on_pushButtonDeletePhoto_clicked()
{
    loadImageToUI(QByteArray());
}


void ProductEditor::on_pushButtonValidate_clicked()
{
    if(currentProduct)
    {
        //get the info from UI
        Product *newProduct = UItoProduct();
        //copy the id, from the old product because they are not set with UItoProduct fct
        newProduct->setId(currentProduct->getId());

        if(!ProductFactory::compare(currentProduct, newProduct))
        {
            newProduct->store();

            products.replace(selectedRow, newProduct);

            delete currentProduct;
            currentProduct = newProduct;

            reloadList(false);
        }

    }
    close();
}

