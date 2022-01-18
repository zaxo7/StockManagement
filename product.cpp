#include "product.h"

Product::Product() : product("Product")
{
    id = -1;
    price = 0.0;

    //add the columns
    product.addColumn("name");
    product.addColumn("price");
    product.addColumn("description");
    product.addColumn("brand");
    product.addColumn("barcode");
    product.addColumn("logo");
}

void Product::load()
{
    Q_ASSERT(id != -1);

    product.setWhereClause("id = " + QString::number(id));

    product.prepareSelect();

    product.exec();

    QSqlQuery query = product.getQuery();

    if(query.next())
        loadFromRecord(query.record());
}


void Product::insert()
{
    product.prepareInsert();

    product.addValue(name);
    product.addValue(price);
    product.addValue(description);
    product.addValue(brand);
    product.addValue(barcode);
    product.addValue(logo);

    id = product.exec();
}

void Product::update()
{
    Q_ASSERT(id != -1);

    product.setWhereClause("id = " + QString::number(id));

    product.prepareUpdate();


    product.addValue(name);
    product.addValue(price);
    product.addValue(description);
    product.addValue(brand);
    product.addValue(barcode);
    product.addValue(logo);

    product.exec();
}

void Product::deletep()
{
    if(id == -1)
        return;

    product.setWhereClause("id = " + QString::number(id));

    product.prepareDelete();

    product.exec();
}

void Product::store()
{
    if(id != -1)
    {
        update();
    }
    else
    {
        insert();
    }
}

void Product::loadFromRecord(QSqlRecord rec)
{
    id = rec.value("id").toInt();
    name = rec.value("name").toString();
    price = rec.value("price").toFloat();
    description = rec.value("description").toString();
    brand = rec.value("brand").toString();
    barcode = rec.value("barcode").toString();
    logo = rec.value("logo").toByteArray();
}

int Product::getId() const
{
    return id;
}

void Product::setId(int newId)
{
    id = newId;
}

const QString &Product::getName() const
{
    return name;
}

void Product::setName(const QString &newName)
{
    name = newName;
}

float Product::getPrice() const
{
    return price;
}

void Product::setPrice(float newPrice)
{
    price = newPrice;
}

const QByteArray &Product::getLogo() const
{
    return logo;
}

void Product::setLogo(const QByteArray &newLogo)
{
    logo = newLogo;
}

const QString &Product::getDescription() const
{
    return description;
}

void Product::setDescription(const QString &newDescription)
{
    description = newDescription;
}

const QString &Product::getBrand() const
{
    return brand;
}

void Product::setBrand(const QString &newBrand)
{
    brand = newBrand;
}

const QString &Product::getBarcode() const
{
    return barcode;
}

void Product::setBarcode(const QString &newBarcode)
{
    barcode = newBarcode;
}

bool Product::loadLogoToLabel(QLabel *label)
{
    QPixmap pixmap;
    if(logo.size() < 10)
    {
        label->setPixmap(pixmap);
        return true;
    }

    if (!pixmap.loadFromData(logo))
    {
        qDebug() << "error in image";
        return false;
    }

    int width = label->width();
    int height = label->height();
    //Show uploaded picture and move the MediaItem to the end
    label->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));

    return true;
}

bool ProductFactory::compare(Product *p1, Product *p2)
{
    if(!p1 || !p2)
        return false;
    if((p1->name != p2->name)
            || (p1->price != p2->price)
            || (p1->logo.size() != p2->logo.size())
            || (p1->description != p2->description)
            || (p1->brand != p2->brand)
            || (p1->barcode != p2->barcode))
    {
        if(p1->id != p2->id)
            qDebug() << "the two products differ in id "
                     << QString::number(p1->id)
                     << " != "
                     << QString::number(p2->id);
        return false;
    }

    return true;
}
