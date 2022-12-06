#pragma once
#ifndef POPUP_NGTDM_H
#define POPUP_NGTDM_H

#include <QDialog>
#include "ui_popup_NGTDM.h"
#include "ngtdm.h"

namespace Ui {
	class popup_NGTDM;
}

class popup_NGTDM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_NGTDM(QWidget *parent = nullptr);
	~popup_NGTDM();

	QButtonGroup *filterGroup;

	Ui::popup_NGTDM *ui;

private:
	
};

#endif // POPUP_NGTDM_H
