#include "datefilterproxymodel.h"
#include <QDebug>
#include <QDateTime>
#include <QVariant>
#include "qteventmodel.h"

DateFilterProxyModel::DateFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    // If you're doing sorting by default, set up a sort role here, e.g.:
    // setSortRole(MyEventStartTimeRole);
    // setSortCaseSensitivity(Qt::CaseInsensitive);
    // setDynamicSortFilter(true);
}

QDate DateFilterProxyModel::filterDate() const
{
    return m_filterDate;
}

bool DateFilterProxyModel::filterByWeek() const {
    return m_filterByWeek;
}

void DateFilterProxyModel::setFilterDate(const QDate &date)
{
    if (m_filterDate == date)
        return;

    m_filterDate = date;
    emit filterDateChanged();

    // Trigger the model to re-check all rows
    invalidateFilter();
}

void DateFilterProxyModel::setFilterByWeek(bool state) {
    if (m_filterByWeek == state) return;

    m_filterByWeek = state;
    emit filterByWeekChanged();

    invalidateFilter();
}

bool DateFilterProxyModel::filterAcceptsRow(int sourceRow,
                                            const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;

    // Use QtEventModel::DtStartRole instead of a hardcoded integer
    QVariant startVar = sourceModel()->data(index, QtEventModel::DtStartRole);
    if (!startVar.isValid())
        return false;

    QDateTime dtStart = startVar.toDateTime();
    if (!dtStart.isValid())
        return false;

    // Compare to our filter date
    if (!m_filterByWeek) {
        return (dtStart.date() == m_filterDate);
    } else {
        int dtStartYear;
        int filterDateYear;
        bool weekMatch = dtStart.date().weekNumber(&dtStartYear) == m_filterDate.weekNumber(&filterDateYear);

        return (weekMatch && (dtStartYear == filterDateYear));
    }
}
