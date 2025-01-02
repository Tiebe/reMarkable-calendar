#include "Calendar.h"
#include "icalendarlib/icalendar.h"
#include <dirent.h>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>

QVector<QtEvent> toQtEvents(const std::list<Event *> &eventList)
{
    QVector<QtEvent> result;
    result.reserve(static_cast<int>(eventList.size()));
    for (auto *ev : eventList) {
        if (ev) {
            result.push_back(QtEvent(*ev));
        }
    }
    return result;
}


void Calendar::loadIcsFromLink(const QString &url, const QString &name) {
    auto request = QNetworkRequest(QUrl(url));

    request.setHeader(QNetworkRequest::ServerHeader, name);

    manager->get(request);
}

void Calendar::onIcsReply(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    auto request = reply->request();
    QString name = request.header(QNetworkRequest::ServerHeader).toString();

    ICalendar calendar;
    calendar.LoadFromString(answer.toStdString());
    QVector<QtEvent> qtEvents = toQtEvents(calendar.Events);

    events[name] = qtEvents;

    QVector<QtEvent> allEvents;
    for (auto event : events.values()) {
        allEvents += event;
    }

    _eventModel->setEvents(allEvents);

    QString ENV_ROOT = "/home/root/xovi/exthome/calendar";

    // 1. Build the path to "calendars.json"
    QString filePath = ENV_ROOT + "/" + name;
    QFile file(filePath);

    // Open the file in WriteOnly mode, which overwrites existing contents
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        reply->deleteLater();
        return;
    }

    QTextStream out(&file);
    out << answer;

    file.flush();
    file.close();

    reply->deleteLater();
}

void Calendar::loadCalendars() {
    qDebug() << "1";
    QString ENV_ROOT = "/home/root/xovi/exthome/calendar";

    // 1. Build the path to "calendars.json"
    QString filePath = ENV_ROOT + "/calendars.json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open" << filePath << "for reading";
        return;
    }

    // 2. Read all data from the file and close it
    QByteArray content = file.readAll();
    file.close();

    // 3. Parse the JSON into a QJsonDocument
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(content, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON in" << filePath << ":" << parseError.errorString();
        return;
    }

    // 4. Verify that we have an array at the top level
    if (!doc.isArray()) {
        qWarning() << "Expected a JSON array in" << filePath;
        return;
    }

    QJsonArray array = doc.array();

    // 5. Extract "icsLink" from each object
    std::vector<QString> links;
    links.reserve(array.size());

    for (const QJsonValue &val : array) {
        qDebug() << val;
        if (!val.isObject())
            continue;  // Skip any array elements that aren't objects

        QJsonObject obj = val.toObject();
        if (obj.contains("icsLink") && obj["icsLink"].isString() && obj.contains("name") && obj["name"].isString()) {
            auto link = obj["icsLink"].toString();
            auto name = obj["name"].toString();
            qDebug() << name;

            QString pathStr = QString(ENV_ROOT) + "/" + name;
            QByteArray ba = pathStr.toLatin1();
            char *calendarPath = ba.data();
            qDebug() << calendarPath;
            ICalendar icalendar(calendarPath);

            QVector<QtEvent> qtEvents = toQtEvents(icalendar.Events);
            qDebug() << icalendar.Events.size();
            qDebug() << qtEvents.length();
            events[name] = qtEvents;

            loadIcsFromLink(link, name);
        } else {
            qDebug() << "Unknown calendar: " << val;
        }
    }

    QVector<QtEvent> allEvents;
    for (auto event : events.values()) {
        allEvents += event;
    }

    _eventModel->setEvents(allEvents);
}
