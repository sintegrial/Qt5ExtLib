#include <QTextEdit>
#include <QDockWidget>

#include "mainwindow.h"
#include "texteditfindwidget.h"

MainWindow::MainWindow()
{
	QTextEdit* textEdit = new QTextEdit;
    setCentralWidget(textEdit);
	textEdit->setText("Here you can put some text to search words in...");

	TextEditFindWidget* textFinder = new TextEditFindWidget(AbstractFindWidget::FindFlags(), this);
	textFinder->setTextEdit(textEdit);

	QDockWidget* dock = new QDockWidget;
	dock->setWidget(textFinder);
	addDockWidget(Qt::TopDockWidgetArea, dock);
}
