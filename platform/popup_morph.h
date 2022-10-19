#pragma once
#ifndef POPUP_MORPH_H
#define POPUP_MORPH_H

#include <QDialog>
#include "ui_popup_Morph.h"
#include "morphology.h"

namespace Ui {
	class popup_Morph;
}

class popup_Morph : public QDialog
{
	Q_OBJECT

public:
	explicit popup_Morph(QWidget *parent = nullptr);
	~popup_Morph();

	QButtonGroup *filterGroup;

	Ui::popup_Morph *ui;

private:
	
};

#endif // POPUP_MORPH_H
