#include <QThread>
#include <QDebug>
#include <QLabel>
#include "QuotesModel.h"



QuotesModel::QuotesModel(const QuotesData &quotesData, int linesLimit, QObject *parent)
	:	QAbstractTableModel(parent),
		quotesData(quotesData),
		linesLimit(linesLimit)
{
	connect(&quotesData, &QuotesData::dataChanged, this, &QuotesModel::slotDataChanged);
}



int QuotesModel::getLinesLimit() const
{
	return linesLimit;
}



void QuotesModel::setLinesLimit(int count)
{
	linesLimit = count;
}



int QuotesModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}



QVariant QuotesModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int column = index.column();

	if (role == Qt::DisplayRole) {
		return QVariant(quotesData.getSnapshotData(row, column));
	}

	return QVariant();
}



QModelIndex	QuotesModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}



QModelIndex	QuotesModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}



int QuotesModel::rowCount(const QModelIndex &parent) const
{
	return qMin(linesLimit, quotesData.getSnapshotAggregatedListSize());
}



QVariant QuotesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case COLUMN_INDEX_PRICE:
			return "Price";
		case COLUMN_INDEX_VOLUME:
			return "Volume";
		}
	}
	else if (orientation == Qt::Vertical) {
		return section + 1;
	}

	return QVariant();
}



void  QuotesModel::slotDataChanged()
{
	beginResetModel();

	quotesData.makeSnapshot();

	int bestPrice = quotesData.getSnapshotData(0, 0);
	emit bestPriceUpdate(bestPrice);

	int vwaPrice = quotesData.getSnapshotVolumeWeightedAveragePrice();
	emit volumeWeightedAveragePriceUpdate(vwaPrice);

	int aggregatedQuotesCount = quotesData.getSnapshotAggregatedListSize();
	emit aggregatedQuotesCountUpdate(aggregatedQuotesCount);

	int totalQuotesCount = quotesData.getSnapshotTotalListSize();
	emit totalQuotesCountUpdate(totalQuotesCount);

	endResetModel();
}
