#pragma once
#ifndef POPUP_NGLDM_H
#define POPUP_NGLDM_H

#include <QDialog>
#include "ui_popup_NGLDM.h"
#include "ngldm.h"

namespace Ui {
	class popup_NGLDM;
}

class popup_NGLDM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_NGLDM(QWidget *parent = nullptr);
	~popup_NGLDM();

	QButtonGroup *filterGroup;

	Ui::popup_NGLDM *ui;

private:
	
};

#endif // POPUP_NGLDM_H
