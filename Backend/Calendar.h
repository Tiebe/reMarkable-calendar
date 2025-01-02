#include <QObject>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "qteventmodel.h"
#include "icalendarlib/icalendar.h"

class Calendar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date NOTIFY dateChanged)
    // Expose the model to QML
    Q_PROPERTY(QAbstractItemModel* eventModel READ eventModel CONSTANT)

public:
    explicit Calendar(QObject *parent = nullptr)
        : QObject(parent)
    {
        _date = QDate::currentDate();
        emit dateChanged();
        connect(manager, &QNetworkAccessManager::finished,
                this, &Calendar::onIcsReply);
    }

    QDate date() const { return _date; }

    // We add a getter that QML can call to retrieve the model
    QAbstractItemModel* eventModel() const {
        return _eventModel;
    }

    // Example functions for changing the date
    Q_INVOKABLE void addMonth()    { _date = _date.addMonths(1);  emit dateChanged(); }
    Q_INVOKABLE void addDay()      { _date = _date.addDays(1);    emit dateChanged(); }
    Q_INVOKABLE void addWeek()     { _date = _date.addDays(7);    emit dateChanged(); }
    Q_INVOKABLE void subtractMonth(){ _date = _date.addMonths(-1);emit dateChanged(); }
    Q_INVOKABLE void subtractDay() { _date = _date.addDays(-1);   emit dateChanged(); }
    Q_INVOKABLE void subtractWeek(){ _date = _date.addDays(-7);   emit dateChanged(); }

    // Function to load ICS from a link
    Q_INVOKABLE void loadIcsFromLink(const QString &url, const QString &name);
    Q_INVOKABLE void loadCalendars();

signals:
    void dateChanged();

private slots:
    void onIcsReply(QNetworkReply *reply);

private:
    QDate _date;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Our QtEventModel pointer
    QtEventModel *_eventModel = new QtEventModel(this);
    QMap<QString, QVector<QtEvent>> events;
};
