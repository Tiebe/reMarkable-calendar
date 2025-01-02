#ifndef QTEVENT_H
#define QTEVENT_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include "icalendarlib/date.h"
#include "icalendarlib/types.h"

// Forward-declare your original Event struct so we can use it in the constructor
struct Event;

static QDateTime toQDateTime(const Date &d)
{
    // Watch for invalid (0) year, etc.
    if (d.Data[YEAR] == 0 && d.Data[MONTH] == 0 && d.Data[DAY] == 0) {
        return QDateTime(); // invalid/empty
    }

    QDate date(d.Data[YEAR], d.Data[MONTH], d.Data[DAY]);
    if (d.WithTime) {
        QTime time(d.Data[HOUR], d.Data[MINUTE], d.Data[SECOND]);
        return QDateTime(date, time);
    }
    return QDateTime(date, QTime(0, 0, 0));
}

class QtEvent
{
public:
    QtEvent() = default;

    // Construct from your original Event struct
    explicit QtEvent(const Event &oldEvent)
    {
        m_uid         = QString::fromStdString(oldEvent.UID);
        m_summary     = QString::fromStdString(oldEvent.Summary);
        m_description = QString::fromStdString(oldEvent.Description);
        m_categories  = QString::fromStdString(oldEvent.Categories);
        m_status      = QString::fromStdString(oldEvent.Status);

        m_completed   = toQDateTime(oldEvent.Completed);
        m_dtStamp     = toQDateTime(oldEvent.DtStamp);
        m_dtStart     = toQDateTime(oldEvent.DtStart);
        m_dtEnd       = toQDateTime(oldEvent.DtEnd);
    }

    // Getters
    QString uid()         const { return m_uid; }
    QString summary()     const { return m_summary; }
    QString description() const { return m_description; }
    QString categories()  const { return m_categories; }
    QString status()      const { return m_status; }

    QDateTime completed() const { return m_completed; }
    QDateTime dtStamp()   const { return m_dtStamp; }
    QDateTime dtStart()   const { return m_dtStart; }
    QDateTime dtEnd()     const { return m_dtEnd; }

    // Setters (only if needed)
    void setUid(const QString &v)        { m_uid = v; }
    void setSummary(const QString &v)    { m_summary = v; }
    void setDescription(const QString &v){ m_description = v; }
    void setCategories(const QString &v) { m_categories = v; }
    void setStatus(const QString &v)     { m_status = v; }

    void setCompleted(const QDateTime &v){ m_completed = v; }
    void setDtStamp(const QDateTime &v)  { m_dtStamp = v; }
    void setDtStart(const QDateTime &v)  { m_dtStart = v; }
    void setDtEnd(const QDateTime &v)    { m_dtEnd = v; }

private:
    QString m_uid;
    QString m_summary;
    QString m_description;
    QString m_categories;
    QString m_status;

    QDateTime m_completed;
    QDateTime m_dtStamp;
    QDateTime m_dtStart;
    QDateTime m_dtEnd;
};
#endif // QTEVENT_H
