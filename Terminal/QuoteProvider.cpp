#include "QuoteProvider.h"
#include "ConnectionWorker.h"



QuoteProvider::QuoteProvider(QuotesData &quotesData, const QString &address, int port, QObject *parent)
	:	QThread(parent),
		connectionWorker(),
		quotesData(quotesData),
		address(address),
		port(port),
		connectionState(CONNECTION_STATE_CONNECTING)
{
}



QuoteProvider::~QuoteProvider()
{
	quit();
	requestInterruption();
	wait();
	delete connectionWorker;
}



void QuoteProvider::run()
{
	connectionWorker = new ConnectionWorker(quotesData, address, port);

	connect(connectionWorker, &ConnectionWorker::connecting, this, &QuoteProvider::slotConnecting);
	connect(connectionWorker, &ConnectionWorker::connected, this, &QuoteProvider::slotConnected);
	connect(connectionWorker, &ConnectionWorker::disconnected, this, &QuoteProvider::slotDisconnected);
	connect(connectionWorker, &ConnectionWorker::connectionError, this, &QuoteProvider::slotConnectionError);

	connect(this, &QuoteProvider::connectToHost, connectionWorker, &ConnectionWorker::slotConnectToHost);
	connect(this, &QuoteProvider::disconnectFromHost, connectionWorker, &ConnectionWorker::slotDisconnectFromHost);

	connectionWorker->tryConnection();

	emit connectionStateChanged(this);

	exec();

	connectionWorker->close();
}



void QuoteProvider::needConnectToHost()
{
	emit connectToHost();
}



void QuoteProvider::needDisconnectFromHost()
{
	emit disconnectFromHost();
}



void QuoteProvider::slotConnecting()
{
	changeState(CONNECTION_STATE_CONNECTING);
}



void QuoteProvider::slotConnected()
{
	changeState(CONNECTION_STATE_CONNECTED);
}



void QuoteProvider::slotDisconnected()
{
	changeState(CONNECTION_STATE_DISCONNECTED);
}



void QuoteProvider::slotConnectionError(QAbstractSocket::SocketError error)
{
	changeState(CONNECTION_STATE_ERROR);
}



void QuoteProvider::changeState(ConnectionState state)
{
	connectionState = state;
	emit connectionStateChanged(this);
}
