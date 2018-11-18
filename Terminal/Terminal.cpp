#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include "Terminal.h"
#include "QuotesTableView.h"
#include "QuoteProvidersTableView.h"



Terminal::Terminal(QWidget *parent)
	:	QMainWindow(parent),
		quotesData(),
		quotesModel(quotesData),
		quoteProvidersModel(quotesData)
{
	guiSetup();
}



void Terminal::guiSetup()
{
	QLCDNumber *bestPriceLcd = new QLCDNumber(8);
	connect(
		&quotesModel, &QuotesModel::bestPriceUpdate,
		bestPriceLcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

	QLCDNumber *vwapLcd = new QLCDNumber(8);
	connect(
		&quotesModel, &QuotesModel::volumeWeightedAveragePriceUpdate,
		vwapLcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

	QLCDNumber *aggregatedQuotesLcd = new QLCDNumber(8);
	connect(
		&quotesModel, &QuotesModel::aggregatedQuotesCountUpdate,
		aggregatedQuotesLcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

	QLCDNumber *totalQuotesLcd = new QLCDNumber(8);
	connect(
		&quotesModel, &QuotesModel::totalQuotesCountUpdate,
		totalQuotesLcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

	QHBoxLayout *topLayout = new QHBoxLayout();
	topLayout->addWidget(new QLabel("Best Price:"));
	topLayout->addWidget(bestPriceLcd, 5);
	topLayout->addStretch(1);
	topLayout->addWidget(new QLabel("VWAP:"));
	topLayout->addWidget(vwapLcd, 5);
	topLayout->addStretch(1);
	topLayout->addWidget(new QLabel("Aggregated Quotes:"));
	topLayout->addWidget(aggregatedQuotesLcd, 5);
	topLayout->addStretch(1);
	topLayout->addWidget(new QLabel("Total Quotes:"));
	topLayout->addWidget(totalQuotesLcd, 5);

	QuoteProvidersTableView *quoteProvidersTableView = new QuoteProvidersTableView();
	quoteProvidersTableView->setModel(&quoteProvidersModel);
	quoteProvidersTableView->setContentsMargins(5, 10, 0, 10);

	QuotesTableView *quotesTableView = new QuotesTableView();
	quotesTableView->setModel(&quotesModel);
	quotesTableView->setContentsMargins(0, 10, 5, 10);

	QSplitter *splitter = new QSplitter(Qt::Horizontal);
	splitter->addWidget(quoteProvidersTableView);
	splitter->addWidget(quotesTableView);
	splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));

	QVBoxLayout *centralLayout = new QVBoxLayout();
	centralLayout->addLayout(topLayout, 1);
	centralLayout->addWidget(splitter, 8);

	QWidget *window = new QWidget();
	window->setLayout(centralLayout);

	setCentralWidget(window);
}
