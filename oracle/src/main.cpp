#include "oracleimporter.h"

#include <QApplication>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    OracleImporter importer("");
    QTextStream stream(stdin);
    QByteArray array = stream.readAll().toUtf8();
    if (importer.readSetsFromByteArray(array)) {
        int amount = importer.startImport();
        if (!importer.saveToFile("./cards.xml", "", "")) {
            qDebug() << "could not save file" << amount << "sets";
        }
    } else {
        qDebug() << "could not read sets from array" << array.length() << "chars";
    }
}
