#include "inventoryselector.h"
#include "ui_inventoryselector.h"

InventorySelector::InventorySelector(Inventory *inv, InventoryAction act, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventorySelector)
{
    ui->setupUi(this);

    selectedRow = -1;

    currentProduct = NULL;

    inventory = inv;

    action = act;

    ui->dateEditExpirationDate->setDate(QDate::currentDate());

    reloadList();

    on_listViewProducts_clicked(ui->listViewProducts->model()->index(selectedRow, 0));

    setIcon();
}

InventorySelector::~InventorySelector()
{
    delete ui;
}

void InventorySelector::setIcon()
{
    switch(action)
    {
        case BUY:
            QDialog::setWindowIcon(QIcon("://ressources/icons/box-arrow-down.svg"));
            QDialog::setWindowTitle("Achat");
        break;
        case SELL:
            QDialog::setWindowIcon(QIcon("://ressources/icons/box-arrow-up.svg"));
            QDialog::setWindowTitle("Vente");
        break;
        case DUMP:
            QDialog::setWindowIcon(QIcon("://ressources/icons/trash.svg"));
            QDialog::setWindowTitle("Corbeille");
        break;
        case REPRODUCE:
            QDialog::setWindowIcon(QIcon("://ressources/icons/recycle.svg"));
            QDialog::setWindowTitle("Reproduire");
        break;
    }
}

int InventorySelector::exec()
{
    return QDialog::exec();
}


void InventorySelector::reloadList(bool reset)
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
        QStandardItem *item = new QStandardItem;
        item->setText(p->getName() + "\t" + p->getBrand() + "\t" + p->getBarcode());
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
}

void InventorySelector::on_listViewProducts_clicked(const QModelIndex &index)
{
    selectedRow = index.row();

    if(selectedRow <= products.size() && (selectedRow >= 0))
    {
        currentLogo = products.at(selectedRow)->getLogo();
        products.at(selectedRow)->loadLogoToLabel(ui->labelLogo);
        currentProduct = products.at(selectedRow);
    }

    qDebug() << "clicked on index = " << selectedRow << " id = " << index.data(Qt::UserRole).toInt() << "  //  " << products.size();
}


void InventorySelector::StockToUI(Inventory inv)
{

}

Stock *InventorySelector::UIToStock()
{
    Stock *stock = new Stock();

    stock->setQty(ui->lineEditQte->text().toInt());
    stock->setPrice(ui->lineEditPrice->text().toFloat());
    stock->setLot(ui->lineEditLot->text());
    stock->setClient(ui->lineEditClient->text());
    stock->setExpDate(ui->dateEditExpirationDate->date());
    stock->setBuyDate(QDate::currentDate());

    if(currentProduct)
        stock->setProductId(currentProduct->getId());
    else
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Il faut selectionner un produit de la liste"));
        return NULL;
    }

    return stock;
}

void InventorySelector::on_lineEditSearch_textChanged(const QString &arg1)
{
    reloadList();
}


void InventorySelector::on_pushButtonValidate_clicked()
{
    Stock *stock = UIToStock();

    if(stock && inventory)
    {
        int res = 0;
        switch(action)
        {
            case BUY:
                res = inventory->buy(stock);
            break;
            case SELL:
                res = inventory->sell(stock);
            break;
            case DUMP:
                res = inventory->dump(stock);
            break;
            case REPRODUCE:
                res = inventory->reproduce(stock);
            break;
        }
        if(!res)
            close();
    }
}

