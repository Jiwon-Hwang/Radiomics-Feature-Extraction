#pragma once
#ifndef POPUP_GLSZM_H
#define POPUP_GLSZM_H

#include <QDialog>
#include "ui_popup_GLSZM.h"
#include "glszm.h"

namespace Ui {
	class popup_GLSZM;
}

class popup_GLSZM : public QDialog
{
	Q_OBJECT

public:
	explicit popup_GLSZM(QWidget *parent = nullptr);
	~popup_GLSZM();

	QButtonGroup *filterGroup;

	Ui::popup_GLSZM *ui;

private:
	
};

#endif // POPUP_GLSZM_H
