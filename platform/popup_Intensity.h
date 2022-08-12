#pragma once
#ifndef POPUP_INTENSITY_H
#define POPUP_INTENSITY_H

#include <QDialog>
#include "ui_popup_Intensity.h"
#include "localIntensity.h"

namespace Ui {
	class popup_Intensity;
}

class popup_Intensity : public QDialog
{
	Q_OBJECT

public:
	explicit popup_Intensity(QWidget *parent = nullptr);
	~popup_Intensity();

	QButtonGroup *filterGroup;

	Ui::popup_Intensity *ui;

private:
	
};

#endif // POPUP_INTENSITY_H
