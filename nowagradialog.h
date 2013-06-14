#ifndef NOWAGRADIALOG_H
#define NOWAGRADIALOG_H

#include <QDialog>
#include "gra.h"
#include "edytor.h"

namespace Ui {
class NowaGraDialog;
}

class NowaGraDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NowaGraDialog(QWidget *parent = 0);
	~NowaGraDialog();
	Tryb* ustawienia;

private slots:
	void on_buttonBox_accepted();

private:
	Ui::NowaGraDialog *ui;

};

#endif // NOWAGRADIALOG_H
