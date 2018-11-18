#pragma once

#include <QAbstractTableModel>
#include "QuotesData.h"



class QuotesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
							QuotesModel(
								const QuotesData &quotesData,
								int linesLimit = DEFAULT_LINES_LIMIT,
								QObject *parent = Q_NULLPTR);

	int						getLinesLimit() const;
	void					setLinesLimit(int count);

	// QAbstractItemModel virtual
	virtual int				columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant		data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QModelIndex		index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex		parent(const QModelIndex &index) const;
	virtual int				rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
	void					bestPriceUpdate(int newValue);
	void					volumeWeightedAveragePriceUpdate(int newValue);
	void					totalQuotesCountUpdate(int newValue);
	void					aggregatedQuotesCountUpdate(int newValue);

private slots:
	void					slotDataChanged();

private:
	static const int		DEFAULT_LINES_LIMIT = 10;

	enum ColumnIndex {
		COLUMN_INDEX_PRICE,
		COLUMN_INDEX_VOLUME
	};

	const QuotesData		&quotesData;
	int						linesLimit;

};
