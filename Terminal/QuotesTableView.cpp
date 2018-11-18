#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include "QuotesTableView.h"
#include "QuotesModel.h"



QuotesTableView::QuotesTableView(QWidget *parent)
	:	QWidget(parent),
		quotesModel(),
		quotesTable(new QTableView()),
		visibleLinesLimitEdit(new QLineEdit())
{
	guiSetup();
}



void QuotesTableView::setModel(QuotesModel *model)
{
	quotesModel = model;
	quotesTable->setModel(quotesModel);
	setVisibleLinesLimit(quotesModel->getLinesLimit());
}



void QuotesTableView::setVisibleLinesLimit(int count)
{
	visibleLinesLimitEdit->setText(QString::number(count));
}



void QuotesTableView::slotVisibleLinesLimitEditingFinished()
{
	int value = visibleLinesLimitEdit->text().toInt();
	quotesModel->setLinesLimit(value);
}



void QuotesTableView::guiSetup()
{
	quotesTable->setSelectionMode(QAbstractItemView::NoSelection);
	quotesTable->horizontalHeader()->setStretchLastSection(true);
	quotesTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	visibleLinesLimitEdit->setValidator(new QIntValidator());
	connect(visibleLinesLimitEdit, &QLineEdit::editingFinished, this, &QuotesTableView::slotVisibleLinesLimitEditingFinished);

	QHBoxLayout *showSettingsLayout = new QHBoxLayout();
	showSettingsLayout->addWidget(new QLabel("Show "));
	showSettingsLayout->addWidget(visibleLinesLimitEdit);
	showSettingsLayout->addWidget(new QLabel("lines"));
	showSettingsLayout->addStretch();

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(new QLabel("Aggregated sorted Quote's List"));
	layout->addWidget(quotesTable);
	layout->addLayout(showSettingsLayout);
	layout->setContentsMargins(0, 0, 0, 0);

	setLayout(layout);
}
