#pragma once
#ifndef POPUP_GLDZM_H
#define POPUP_GLDZM_H

#include <QDialog>
#include "ui_popup_GLDZM.h"
#include "gldzm.h"

namespace Ui {
	class popup_GLDZM;
}

class popup_GLDZM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_GLDZM(QWidget *parent = nullptr);
	~popup_GLDZM();

	QButtonGroup *filterGroup;

	Ui::popup_GLDZM *ui;

private:
	
};

#endif // POPUP_GLDZM_H
