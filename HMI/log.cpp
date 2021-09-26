#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::logWork(QString FunctionName, QString State, QString Value, QString Comment)
{
    QDir dir;
    QFile logfile(dir.absolutePath()+"/log.json");
    QTextStream stream( &logfile);
    QString currentTime = GetTime(true);
    QJsonObject json_object;
    std::string returnmessage;

    json_object["Date"] = currentTime;
    json_object["Function"] = FunctionName;
    json_object["State"] = State;
    json_object["Value"] = Value;
    json_object["Comment"] = Comment;

    QJsonDocument doc;
    doc.setObject(json_object);

    stream << doc.toJson() << endl;
}
QString Log::GetTime(bool existseconds)
{
    QString dateTimeString;

    QDateTime dateTime = dateTime.currentDateTime();
    if(existseconds == false)
    {
        dateTimeString = dateTime.toString("dd.MM.yyyy hh:mm");
    }
    else if(existseconds == true)
    {
        dateTimeString = dateTime.toString("dd.MM.yyyy hh:mm:ss");
    }

    return dateTimeString;
}
