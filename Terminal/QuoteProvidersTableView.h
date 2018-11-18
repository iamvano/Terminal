#pragma once

#include <QWidget>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "QuoteProvidersModel.h"



class QuoteProvidersTableView : public QWidget
{
	Q_OBJECT

public:
						QuoteProvidersTableView(QWidget *parent = Q_NULLPTR);
						~QuoteProvidersTableView();

	void				setModel(QuoteProvidersModel *model);

private slots:
	void				slotAddProviderButtonClicked();
	void				slotConnectProviderButtonClicked();
	void				slotDisconnectProviderButtonClicked();
	void				slotRemoveProviderButtonClicked();
	void				slotCurrentProviderChanged(const QModelIndex &current, const QModelIndex &previous);
	void				slotProviderAvailableToConnection(int row);
	void				slotProviderAvailableToDisconnection(int row);

private:
	QuoteProvidersModel	*providersModel;
	QTableView			*providersTable;
	QItemSelectionModel	*selectionModel;
	QLineEdit			*addressEdit;
	QLineEdit			*portFromEdit;
	QLineEdit			*portToEdit;
	QPushButton			*disconnectProviderButton;
	QPushButton			*connectProviderButton;
	QPushButton			*removeProviderButton;
	QPushButton			*addProviderButton;

	void				guiSetup();
};
