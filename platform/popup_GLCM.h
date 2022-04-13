#pragma once
#ifndef POPUP_GLCM_H
#define POPUP_GLCM_H

#include <QDialog>
#include "ui_popup_GLCM.h"
#include "glcm.h"

namespace Ui {
	class popup_GLCM;
}

class popup_GLCM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_GLCM(QWidget *parent = nullptr);
	~popup_GLCM();

	QButtonGroup *filterGroup;

	Ui::popup_GLCM *ui;

private:
	
};

#endif // POPUP_GLCM_H
