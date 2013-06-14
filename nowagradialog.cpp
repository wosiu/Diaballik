#include "nowagradialog.h"
#include "ui_nowagradialog.h"

NowaGraDialog::NowaGraDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NowaGraDialog)
{
	ui->setupUi(this);
}

NowaGraDialog::~NowaGraDialog()
{
	delete ui;
}

void NowaGraDialog::on_buttonBox_accepted()
{
	if ( ui->radioButtonPlanszaEdytor ->isChecked() )
		ustawienia = new Edytor();
	else
		ustawienia = new Gra();

	if ( ui->radioButtonCZCzlowiek->isChecked() )
		ustawienia->typGracza[ 0 ] = Tryb::CZLOWIEK;
	else
		ustawienia->typGracza[ 0 ] = Tryb::KOMPUTER;

	if ( ui->radioButtonBCzlowiek->isChecked() )
		ustawienia->typGracza[ 1 ] = Tryb::CZLOWIEK;
	else
		ustawienia->typGracza[ 1 ] = Tryb::KOMPUTER;

	if ( ui->radioButtonPlanszaPlik ->isChecked() )
		qDebug() << "sprawdzic czy ok";
}
