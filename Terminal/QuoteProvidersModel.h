#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include "QuoteProvider.h"
#include "QuotesData.h"



class QuoteProvidersModel : public QAbstractTableModel
{
	Q_OBJECT

public:
							QuoteProvidersModel(QuotesData &quotesData, QObject *parent = Q_NULLPTR);
							~QuoteProvidersModel();

	// QAbstractItemModel virtual
	virtual int				columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant		data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QModelIndex		index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex		parent(const QModelIndex &index) const;
	virtual int				rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void					addProvider(QString address, int port);
	void					connectProvider(int number);
	void					disconnectProvider(int number);
	void					removeProvider(int number);
	void					removeAllProviders();
	bool					isProviderAvailableToConnection(int row);
	bool					isProviderAvailableToDisconnection(int row);

public slots:
	void					slotProviderConnectionStateChanged(const QuoteProvider *quoteProvider);

signals:
	void					providerAvailableToConnection(int row);
	void					providerAvailableToDisconnection(int row);

private:
	enum ColumnIndex {
		COLUMN_INDEX_ADDRESS,
		COLUMN_INDEX_PORT,
		COLUMN_INDEX_STATE
	};

	enum UserRole {
		USER_ROLE_RAW_FIELD = Qt::UserRole
	};

	QuotesData				&quotesData;
	QList<QuoteProvider *>	providers;

};
