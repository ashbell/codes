#include <QApplication>
#include <QDialog>

#include "ui_untitled.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Ui::GotoCellDialog ui;
    QDialog *dialog = new QDialog;
    ui.setupUi(dialog);
    dialog->show();

    return app.exec();




}
