#pragma once

#include <QWidget>
#include <QTableView>
#include <QLineEdit>



class QuotesModel;



class QuotesTableView : public QWidget
{
	Q_OBJECT

public:
						QuotesTableView(QWidget *parent = Q_NULLPTR);

	void				setModel(QuotesModel *model);
	void				setVisibleLinesLimit(int count);

private slots:
	void				slotVisibleLinesLimitEditingFinished();

private:
	QuotesModel			*quotesModel;
	QTableView			*quotesTable;
	QLineEdit			*visibleLinesLimitEdit;

	void				guiSetup();
};
