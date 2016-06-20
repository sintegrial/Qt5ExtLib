#include <QApplication>

#include <qtgradientdialog.h>


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtgradienteditor);

    QApplication app(argc, argv);
	
	QtGradientDialog dialog;
	dialog.show();

    return app.exec();
}
