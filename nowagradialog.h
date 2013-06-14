#ifndef NOWAGRADIALOG_H
#define NOWAGRADIALOG_H

#include <QDialog>
#include "gra.h"
#include "edytor.h"
#include <QtGui> //do QFile

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
	QString saveSciezka;
	void accept();
	bool validateAndSetSave();

private slots:
	void on_radioButtonPlanszaPlik_clicked();
	void on_radioButtonPlanszaEdytor_clicked();
	void on_radioButtonPlanszaPusta_clicked();
	void on_pushButtonWybierz_clicked();

private:
	Ui::NowaGraDialog *ui;

};

#endif // NOWAGRADIALOG_H
