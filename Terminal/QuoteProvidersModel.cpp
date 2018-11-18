#include "QuoteProvidersModel.h"



QuoteProvidersModel::QuoteProvidersModel(QuotesData &quotesData, QObject *parent)
	:	QAbstractTableModel(parent),
		quotesData(quotesData),
		providers()
{
}



QuoteProvidersModel::~QuoteProvidersModel()
{
	removeAllProviders();
}



int QuoteProvidersModel::columnCount(const QModelIndex &parent) const
{
	return 3;
}



QVariant QuoteProvidersModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int column = index.column();

	if (role == Qt::DisplayRole) {
		switch (column) {
		case COLUMN_INDEX_ADDRESS:
			return QVariant(providers.at(row)->getAddress());
		case COLUMN_INDEX_PORT:
			return QVariant(providers.at(row)->getPort());
		case COLUMN_INDEX_STATE:
			QuoteProvider::ConnectionState state = providers.at(row)->getConnectionState();
			switch (state) {
			case QuoteProvider::CONNECTION_STATE_CONNECTING:
				return QVariant(QString("Connecting..."));
			case QuoteProvider::CONNECTION_STATE_CONNECTED:
				return QVariant(QString("Connected"));
			case QuoteProvider::CONNECTION_STATE_DISCONNECTED:
				return QVariant(QString("Disconnected"));
			case QuoteProvider::CONNECTION_STATE_ERROR:
				return QVariant(QString("Error"));
			}
		}
	}
	else if (role == USER_ROLE_RAW_FIELD) {
		switch (column) {
		case COLUMN_INDEX_ADDRESS:
			return QVariant(providers.at(row)->getAddress());
		case COLUMN_INDEX_PORT:
			return QVariant(providers.at(row)->getPort());
		case COLUMN_INDEX_STATE:
			return QVariant(providers.at(row)->getConnectionState());
		}
	}

	return QVariant();
}



QModelIndex	QuoteProvidersModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}



QModelIndex	QuoteProvidersModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}



int QuoteProvidersModel::rowCount(const QModelIndex &parent) const
{
	return providers.size();
}



QVariant QuoteProvidersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case COLUMN_INDEX_ADDRESS:
			return "Address";
		case COLUMN_INDEX_PORT:
			return "Port";
		case COLUMN_INDEX_STATE:
			return "State";
		}
	}

	if (orientation == Qt::Vertical) {
		return section + 1;
	}

	return QVariant();
}



void QuoteProvidersModel::addProvider(QString address, int port)
{
	beginInsertRows(QModelIndex(), 0, 0);
	
	QuoteProvider *provider = new QuoteProvider(quotesData, address, port);
	connect(
		provider,
		&QuoteProvider::connectionStateChanged,
		this,
		&QuoteProvidersModel::slotProviderConnectionStateChanged);
	providers.push_front(provider);
	provider->start();
	endInsertRows();
}



void QuoteProvidersModel::connectProvider(int number)
{
	QuoteProvider *provider = providers.at(number);
	provider->needConnectToHost();
}



void QuoteProvidersModel::disconnectProvider(int number)
{
	QuoteProvider *provider = providers.at(number);
	provider->needDisconnectFromHost();
}



void QuoteProvidersModel::removeProvider(int number)
{
	beginRemoveRows(QModelIndex(), number, number);

	QuoteProvider *provider = providers.at(number);
	delete provider;
	providers.removeAt(number);

	endRemoveRows();
}



void QuoteProvidersModel::removeAllProviders()
{
	if (providers.empty()) {
		return;
	}

	beginRemoveRows(QModelIndex(), 0, providers.size() - 1);

	typedef QList<QuoteProvider *>::iterator ProviderIter;
	for (ProviderIter provider = providers.begin(); provider != providers.end(); ++provider) {
		delete *provider;
	}

	providers.clear();

	endRemoveRows();
}



bool QuoteProvidersModel::isProviderAvailableToConnection(int row)
{
	QModelIndex idx = index(row, COLUMN_INDEX_STATE);
	QuoteProvider::ConnectionState state =
		(QuoteProvider::ConnectionState)data(idx, USER_ROLE_RAW_FIELD).toInt();
	return state == QuoteProvider::CONNECTION_STATE_DISCONNECTED;
}



bool QuoteProvidersModel::isProviderAvailableToDisconnection(int row)
{
	QModelIndex idx = index(row, COLUMN_INDEX_STATE);
	QuoteProvider::ConnectionState state =
		(QuoteProvider::ConnectionState)data(idx, USER_ROLE_RAW_FIELD).toInt();
	return state == QuoteProvider::CONNECTION_STATE_CONNECTED;
}



void QuoteProvidersModel::slotProviderConnectionStateChanged(const QuoteProvider *quoteProvider)
{
	const QuoteProvider * const provider = quoteProvider;
	for (int row = 0; row < providers.size(); ++row) {
		if (providers.at(row) != quoteProvider) {
			continue;
		}
		QModelIndex idx = index(row, COLUMN_INDEX_STATE);
		emit dataChanged(idx, idx);
		QuoteProvider::ConnectionState state =
			(QuoteProvider::ConnectionState)data(idx, USER_ROLE_RAW_FIELD).toInt();
		if (state == QuoteProvider::CONNECTION_STATE_CONNECTED) {
			emit providerAvailableToDisconnection(row);
		}
		if (state == QuoteProvider::CONNECTION_STATE_DISCONNECTED) {
			emit providerAvailableToConnection(row);
		}
	}
}
