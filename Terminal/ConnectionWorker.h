#pragma once

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "QuotesData.h"



class ConnectionWorker : public QObject
{
	Q_OBJECT

public:
					ConnectionWorker(
						QuotesData &quotesData,
						const QString &address,
						int port,
						QObject *parent = Q_NULLPTR);
					~ConnectionWorker();

	void			tryConnection();
	void			close();

signals:
	void			socketError();
	void			connecting();
	void			connected();
	void			disconnected();
	void			connectionError(QAbstractSocket::SocketError error);

public slots:
	void			slotConnectToHost();
	void			slotDisconnectFromHost();

private slots:
	void			slotReadyRead();
	void			slotError(QAbstractSocket::SocketError error);
	void			slotConnected();
	void			slotDisconnected();
	void			slotReconnectionRequest();

private:
	QuotesData		&quotesData;
	const QString	&address;
	int				port;
	QTcpSocket		tcpSocket;
	int				nextBlockSize;
	QList<Quote>	*previousQuotes;
	QList<Quote>	*currentQuotes;

	void			cancelQuotes();
};
