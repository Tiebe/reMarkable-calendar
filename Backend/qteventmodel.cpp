#include "qteventmodel.h"

QtEventModel::QtEventModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int QtEventModel::rowCount(const QModelIndex &parent) const
{
    // For list models, we typically ignore the parent index
    if (parent.isValid())
        return 0;

    return m_events.size();
}

QVariant QtEventModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    if (row < 0 || row >= m_events.size())
        return QVariant();

    const QtEvent &evt = m_events.at(row);

    switch (role) {
    case UIDRole:
        return evt.uid();

    case SummaryRole:
        return evt.summary();

    case DescriptionRole:
        return evt.description();

    case CategoriesRole:
        return evt.categories();

    case StatusRole:
        return evt.status();

    case CompletedRole:
        // Return a string, a QDateTime, or however you want to handle it
        return evt.completed();

    case DtStampRole:
        return evt.dtStamp();

    case DtStartRole:
        return evt.dtStart();

    case DtEndRole:
        return evt.dtEnd();
    }

    return QVariant();
}

QHash<int, QByteArray> QtEventModel::roleNames() const
{
    // Map each role to a name (useful in QML)
    static const QHash<int, QByteArray> roles = {
                                                 { UIDRole,         "uid" },
                                                 { SummaryRole,     "summary" },
                                                 { DescriptionRole, "description" },
                                                 { CategoriesRole,  "categories" },
                                                 { StatusRole,      "status" },
                                                 { CompletedRole,   "completed" },
                                                 { DtStampRole,     "dtStamp" },
                                                 { DtStartRole,     "dtStart" },
                                                 { DtEndRole,       "dtEnd" },
                                                 };
    return roles;
}

void QtEventModel::setEvents(const QVector<QtEvent> &events)
{
    // We want to reset the entire model data
    beginResetModel();
    m_events = events;
    endResetModel();
}
