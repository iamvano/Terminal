#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include <QList>
#include <QMutex>



typedef QPair<int, int> Quote;



class QuotesData : public QObject
{
	Q_OBJECT

public:
							QuotesData(QObject *parent = Q_NULLPTR);
							~QuotesData();

	void					makeSnapshot() const;
	void					updateQuotes(QList<Quote> *currentQuotes, QList<Quote> *previousQuotes);

	int						getSnapshotVolumeWeightedAveragePrice() const;
	int						getSnapshotAggregatedListSize() const;
	int						getSnapshotTotalListSize() const;
	int						getSnapshotData(int row, int column) const;

signals:
	void					dataChanged();

private:
	QMap<int, QList<int>>	aggregatedQuotesMap;
	mutable QVector<Quote>	snapshot;
	mutable int				snapshotTotalQuotes;
	mutable int				snapshotVolumeWeightedAveragePrice;
	mutable QMutex			mutex;
};
