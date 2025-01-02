#ifndef DATEFILTERPROXYMODEL_H
#define DATEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>

class DateFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    // Example property to store the date we want to filter on
    Q_PROPERTY(QDate filterDate READ filterDate WRITE setFilterDate NOTIFY filterDateChanged)
    Q_PROPERTY(bool filterByWeek READ filterByWeek WRITE setFilterByWeek NOTIFY filterByWeekChanged)

public:
    explicit DateFilterProxyModel(QObject *parent = nullptr);

    // For use in QML or otherwise:
    QDate filterDate() const;
    bool filterByWeek() const;
    void setFilterDate(const QDate &date);
    void setFilterByWeek(bool state);

signals:
    void filterDateChanged();
    void filterByWeekChanged();

protected:
    // Re-implement filterAcceptsRow to do the actual filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QDate m_filterDate;
    bool m_filterByWeek;
};
#endif // DATEFILTERPROXYMODEL_H
