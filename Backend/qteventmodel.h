#ifndef QTEVENTMODEL_H
#define QTEVENTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "qtevent.h"

class QtEventModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EventRole {
        UIDRole = Qt::UserRole + 1,
        SummaryRole,
        DescriptionRole,
        CategoriesRole,
        StatusRole,
        CompletedRole,
        DtStampRole,
        DtStartRole,
        DtEndRole
    };
    Q_ENUM(EventRole)

    explicit QtEventModel(QObject *parent = nullptr);

    // Required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Optional override: Provide role->name mapping so that
    // in QML or other usage you can access properties by name (e.g., "uid", "summary"…).
    QHash<int, QByteArray> roleNames() const override;

    // Method to set (replace) the entire list of events
    void setEvents(const QVector<QtEvent> &events);
    // Or you could add individual items, remove items, etc. as needed.

private:
    QVector<QtEvent> m_events;
};

#endif // QTEVENTMODEL_H
