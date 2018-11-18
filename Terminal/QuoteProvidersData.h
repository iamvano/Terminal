#pragma once

#include <QObject>

class QuoteProvidersData : public QObject
{
	Q_OBJECT

public:
	QuoteProvidersData(QObject *parent);
	~QuoteProvidersData();
};
