drop table if exists Inventory;
Create table if not exists Inventory (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(200), productId INTEGER NOT NULL, stockQty INTEGER DEFAULT 0, stockPrice FLOAT DEFAULT 0.0, client VARCHAR(200) DEFAULT '');
SELECT * FROM Inventory;



