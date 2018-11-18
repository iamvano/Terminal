#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QItemSelectionModel>
#include <QHeaderView>
#include "QuoteProvidersTableView.h"



QuoteProvidersTableView::QuoteProvidersTableView(QWidget *parent)
	:	QWidget(parent),
		providersModel(),
		providersTable(new QTableView()),
		selectionModel(new QItemSelectionModel()),
		addressEdit(new QLineEdit("localhost")),
		portFromEdit(new QLineEdit("9090")),
		portToEdit(new QLineEdit("9090")),
		disconnectProviderButton(new QPushButton("Disconnect")),
		connectProviderButton(new QPushButton("Connect")),
		removeProviderButton(new QPushButton("Remove")),
		addProviderButton(new QPushButton("Add"))
{
	guiSetup();
}



QuoteProvidersTableView::~QuoteProvidersTableView()
{
}



void QuoteProvidersTableView::setModel(QuoteProvidersModel * model)
{
	providersModel = model;

	selectionModel->setModel(providersModel);
	connect(
		selectionModel,
		&QItemSelectionModel::currentChanged,
		this,
		&QuoteProvidersTableView::slotCurrentProviderChanged);

	providersTable->setModel(providersModel);
	providersTable->setSelectionModel(selectionModel);

	connect(
		providersModel,
		&QuoteProvidersModel::providerAvailableToDisconnection,
		this,
		&QuoteProvidersTableView::slotProviderAvailableToDisconnection);
}



void QuoteProvidersTableView::slotAddProviderButtonClicked()
{
	int from = portFromEdit->text().toInt();
	int to = portToEdit->text().toInt();

	for (int port = from; port <= to; ++port) {
		providersModel->addProvider(addressEdit->text(), port);
	}

	selectionModel->clear();
}



void QuoteProvidersTableView::slotConnectProviderButtonClicked()
{
	const QModelIndexList &selectedList = selectionModel->selectedRows();
	providersModel->connectProvider(selectedList[0].row());
	selectionModel->clear();
}



void QuoteProvidersTableView::slotDisconnectProviderButtonClicked()
{
	const QModelIndexList &selectedList = selectionModel->selectedRows();
	providersModel->disconnectProvider(selectedList[0].row());
	selectionModel->clear();
}



void QuoteProvidersTableView::slotRemoveProviderButtonClicked()
{
	const QModelIndexList &selectedList = selectionModel->selectedRows();
	providersModel->removeProvider(selectedList[0].row());
	selectionModel->clear();
}



void QuoteProvidersTableView::slotCurrentProviderChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if (!current.isValid()) {
		connectProviderButton->setEnabled(false);
		disconnectProviderButton->setEnabled(false);
		removeProviderButton->setEnabled(false);
		return;
	}

	if (current.row() == previous.row()) {
		return;
	}

	removeProviderButton->setEnabled(true);
	int row = current.row();

	if (providersModel->isProviderAvailableToDisconnection(row)) {
		disconnectProviderButton->setEnabled(true);
		connectProviderButton->setEnabled(false);
	}
	else if (providersModel->isProviderAvailableToConnection(row)) {
		connectProviderButton->setEnabled(true);
		disconnectProviderButton->setEnabled(false);
	}
	else {
		connectProviderButton->setEnabled(false);
		disconnectProviderButton->setEnabled(false);
	}
}



void QuoteProvidersTableView::slotProviderAvailableToConnection(int row)
{
	const QModelIndex &index = providersModel->index(row, 0);
	bool isRowSelected = selectionModel->isSelected(index);
	if (isRowSelected) {
		connectProviderButton->setEnabled(true);
	}
}



void QuoteProvidersTableView::slotProviderAvailableToDisconnection(int row) {
	const QModelIndex &index = providersModel->index(row, 0);
	bool isRowSelected = selectionModel->isSelected(index);
	if (isRowSelected) {
		disconnectProviderButton->setEnabled(true);
	}
}



void QuoteProvidersTableView::guiSetup()
{
	providersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	providersTable->setSelectionMode(QAbstractItemView::SingleSelection);
	providersTable->horizontalHeader()->setStretchLastSection(true);

	portFromEdit->setValidator(new QIntValidator());
	portToEdit->setValidator(new QIntValidator());

	disconnectProviderButton->setEnabled(false);
	connect(
		disconnectProviderButton, &QPushButton::clicked,
		this, &QuoteProvidersTableView::slotDisconnectProviderButtonClicked);

	connectProviderButton->setEnabled(false);
	connect(
		connectProviderButton, &QPushButton::clicked,
		this, &QuoteProvidersTableView::slotConnectProviderButtonClicked);

	removeProviderButton->setEnabled(false);
	connect(
		removeProviderButton, &QPushButton::clicked,
		this, &QuoteProvidersTableView::slotRemoveProviderButtonClicked);

	connect(
		addProviderButton, &QPushButton::clicked,
		this, &QuoteProvidersTableView::slotAddProviderButtonClicked);

	QHBoxLayout *selectedControlsLayout = new QHBoxLayout();
	selectedControlsLayout->addWidget(new QLabel("SELECTED PROVIDER>"));
	selectedControlsLayout->addStretch();
	selectedControlsLayout->addWidget(disconnectProviderButton, 1);
	selectedControlsLayout->addWidget(connectProviderButton, 1);
	selectedControlsLayout->addWidget(removeProviderButton, 1);

	QHBoxLayout *addControlsLayout = new QHBoxLayout();
	addControlsLayout->addWidget(new QLabel("NEW PROVIDER>"));
	addControlsLayout->addStretch();
	addControlsLayout->addWidget(new QLabel("Address:"));
	addControlsLayout->addWidget(addressEdit);
	addControlsLayout->addWidget(new QLabel("Ports: from"));
	addControlsLayout->addWidget(portFromEdit);
	addControlsLayout->addWidget(new QLabel("to"));
	addControlsLayout->addWidget(portToEdit);
	addControlsLayout->addWidget(addProviderButton);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(new QLabel("Quote's Providers"));
	layout->addWidget(providersTable);
	layout->addLayout(selectedControlsLayout);
	layout->addLayout(addControlsLayout);
	layout->setContentsMargins(0, 0, 0, 0);

	setLayout(layout);
}
