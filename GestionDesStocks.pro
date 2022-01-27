QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    databasetable.cpp \
    history.cpp \
    homePage.cpp \
    homePage.cpp \
    inventory.cpp \
    inventoryPage.cpp \
    inventoryselector.cpp \
    main.cpp \
    mainwindow.cpp \
    print.cpp \
    product.cpp \
    producteditor.cpp \
    settingsPage.cpp \
    stock.cpp

HEADERS += \
    database.h \
    databasetable.h \
    history.h \
    inventory.h \
    inventoryselector.h \
    mainwindow.h \
    print.h \
    product.h \
    producteditor.h \
    stock.h

FORMS += \
    inventoryselector.ui \
    mainwindow.ui \
    print.ui \
    producteditor.ui

TRANSLATIONS += \
    GestionDesStocks_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    sheme.sql
