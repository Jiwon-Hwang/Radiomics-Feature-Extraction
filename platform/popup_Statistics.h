#pragma once
#ifndef POPUP_HISTOGRAM_H
#define POPUP_HISTOGRAM_H

#include <QDialog>
#include "ui_popup_Histogram.h"
#include "intensityHistogram.h"

namespace Ui {
	class popup_Histogram;
}

class popup_Histogram : public QDialog
{
	Q_OBJECT

public:
	explicit popup_Histogram(QWidget *parent = nullptr);
	~popup_Histogram();

	QButtonGroup *filterGroup;

	Ui::popup_Histogram *ui;

private:
	
};

#endif // POPUP_HISTOGRAM_H
