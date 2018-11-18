#pragma once

#include <QThread>
#include <QAbstractSocket>
#include "ConnectionWorker.h"
#include "QuotesData.h"



class QuoteProvider : public QThread
{
	Q_OBJECT

public:
					QuoteProvider(
						QuotesData &quotesData,
						const QString &address,
						int port,
						QObject *parent = Q_NULLPTR);
					~QuoteProvider();

	enum ConnectionState {
		CONNECTION_STATE_CONNECTING,
		CONNECTION_STATE_CONNECTED,
		CONNECTION_STATE_DISCONNECTED,
		CONNECTION_STATE_ERROR
	};

	virtual void		run();

	void				needConnectToHost();
	void				needDisconnectFromHost();

	const QString		&getAddress() const { return address; };
	int					getPort() const { return port; };
	ConnectionState		getConnectionState() const { return connectionState; };

signals:
	void				connectionStateChanged(const QuoteProvider *quoteProvider);
	void				connectToHost();
	void				disconnectFromHost();

private slots:
	void				slotConnecting();
	void				slotConnected();
	void				slotDisconnected();
	void				slotConnectionError(QAbstractSocket::SocketError error);

private:
	ConnectionWorker	*connectionWorker;
	QuotesData			&quotesData;
	QString				address;
	int					port;
	ConnectionState		connectionState;

	void				changeState(ConnectionState state);

};
