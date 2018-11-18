#include <QThread>
#include <QTimer>
#include <QRandomGenerator>
#include <QDataStream>
#include "ConnectionWorker.h"
#include "quotes.pb.h"



ConnectionWorker::ConnectionWorker(QuotesData &quotesData, const QString &address, int port, QObject *parent)
	:	QObject(parent),
		quotesData(quotesData),
		address(address),
		port(port),
		tcpSocket(),
		nextBlockSize(),
		previousQuotes(new QList<Quote>()),
		currentQuotes(new QList<Quote>())
{
	connect(&tcpSocket, &QTcpSocket::connected, this, &ConnectionWorker::slotConnected);
	connect(&tcpSocket, &QTcpSocket::disconnected, this, &ConnectionWorker::slotDisconnected);
	connect(&tcpSocket, &QTcpSocket::readyRead, this, &ConnectionWorker::slotReadyRead);
	qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
	connect(
		&tcpSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
		this, &ConnectionWorker::slotError);
}



ConnectionWorker::~ConnectionWorker()
{
	tcpSocket.disconnect();
	this->disconnect();
	if (previousQuotes) {
		delete previousQuotes;
	}
	if (currentQuotes) {
		delete currentQuotes;
	}
}



void ConnectionWorker::tryConnection()
{
	emit connecting();
	tcpSocket.connectToHost(address, port, QIODevice::ReadOnly);
}



void ConnectionWorker::close()
{
	tcpSocket.close();
	cancelQuotes();
}



void ConnectionWorker::slotReadyRead()
{
	QDataStream in(&tcpSocket);
	in.setVersion(QDataStream::Qt_5_11);

	for (;;) {

		if (!nextBlockSize) {
			QString strLen;
			char currentChar = '*';
			char previousChar;
			do {
				previousChar = currentChar;
				int realLen = in.readRawData(&currentChar, 1);
				if (realLen < 1) {
					return;
				}
				strLen += currentChar;
			} while (!(currentChar == '\n' && previousChar == '\r'));
			strLen.truncate(strLen.length() - 2);
			bool isOk;
			nextBlockSize = strLen.toInt(&isOk);
		}

		if (tcpSocket.bytesAvailable() < nextBlockSize) {
			break;
		}

		char *block = new char[nextBlockSize];
		in.readRawData(block, nextBlockSize);

		quotes::Quotes quotes;
		bool parseSuccess = quotes.ParseFromArray(block, nextBlockSize);
		delete[] block;

		delete previousQuotes;
		previousQuotes = currentQuotes;
		currentQuotes = new QList<Quote>();

		for (int quoteIndex = 0; quoteIndex < quotes.quote_size(); ++quoteIndex) {
			const quotes::Quotes_Quote &quote = quotes.quote(quoteIndex);
			currentQuotes->push_back(Quote(quote.price(), quote.volume()));
		}

		quotesData.updateQuotes(currentQuotes, previousQuotes);
		previousQuotes->clear();

		nextBlockSize = 0;
	}
}


void ConnectionWorker::slotConnectToHost()
{
	tryConnection();
}



void ConnectionWorker::slotDisconnectFromHost()
{
	tcpSocket.disconnectFromHost();
}



void ConnectionWorker::slotError(QAbstractSocket::SocketError error)
{
	cancelQuotes();
	emit connectionError(error);
	int timerOffset = QRandomGenerator::global()->bounded(-500, 500);
	QTimer::singleShot(3 * 1000 + timerOffset, this, &ConnectionWorker::slotReconnectionRequest);
}



void ConnectionWorker::slotConnected()
{
	emit connected();
}



void ConnectionWorker::slotDisconnected()
{
	cancelQuotes();
	emit disconnected();
}



void ConnectionWorker::slotReconnectionRequest()
{
	tryConnection();
}



void ConnectionWorker::cancelQuotes()
{
	quotesData.updateQuotes(0, previousQuotes);
	previousQuotes->clear();

	quotesData.updateQuotes(0, currentQuotes);
	currentQuotes->clear();
}
