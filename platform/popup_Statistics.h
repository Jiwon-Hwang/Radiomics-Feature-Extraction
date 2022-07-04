#pragma once
#ifndef POPUP_STATISTICS_H
#define POPUP_STATISTICS_H

#include <QDialog>
#include "ui_popup_Statistics.h"
#include "intensityStatistics.h"

namespace Ui {
	class popup_Statistics;
}

class popup_Statistics : public QDialog
{
	Q_OBJECT

public:
	explicit popup_Statistics(QWidget *parent = nullptr);
	~popup_Statistics();

	QButtonGroup *filterGroup;

	Ui::popup_Statistics *ui;

private:
	
};

#endif // POPUP_STATISTICS_H
