#pragma once

#include <QtWidgets/QMainWindow>
#include "QuotesData.h"
#include "QuotesModel.h"
#include "QuoteProvidersModel.h"



class Terminal : public QMainWindow
{
	Q_OBJECT

public:
						Terminal(QWidget *parent = Q_NULLPTR);

private:
	QuotesData			quotesData;
	QuotesModel			quotesModel;
	QuoteProvidersModel quoteProvidersModel;

	void				guiSetup();
};
