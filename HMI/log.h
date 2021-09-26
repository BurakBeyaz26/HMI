#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QDir>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

    QString GetTime(bool existseconds);
    void logWork(QString FunctionName=0, QString State=0, QString Value=0, QString Comment=0);

signals:

};

#endif // LOG_H
