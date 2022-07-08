#pragma once
#ifndef POPUP_GLRLM_H
#define POPUP_GLRLM_H

#include <QDialog>
#include "ui_popup_GLRLM.h"
#include "glrlm.h"

namespace Ui {
	class popup_GLRLM;
}

class popup_GLRLM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_GLRLM(QWidget *parent = nullptr);
	~popup_GLRLM();

	QButtonGroup *filterGroup;

	Ui::popup_GLRLM *ui;

private:
	
};

#endif // POPUP_GLRLM_H
