#include "CutiePy.hpp"

#include <QDockWidget>
#include <QApplication>

MainWindow::MainWindow() {
	auto textEdit = new QPlainTextEdit();
	setCentralWidget(textEdit);
	auto consoleContainer = new QDockWidget(this);
	auto console = new PythonConsole();
	consoleContainer->setWidget(console);
	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, consoleContainer);
}

PythonConsole::PythonConsole(QWidget* parent) : QPlainTextEdit(parent) {
	QPalette p = palette();
	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, Qt::green);
	setPalette(p);
	auto consoleFont = QFont("Courier New");
	consoleFont.setStyleHint(QFont::StyleHint::Monospace);
	setFont(consoleFont);
}

int main(int argc, char* argv[])
{
	auto guiApp = true;
	for (int i = 1; i < argc; ++i)
		if (!qstrcmp(argv[i], "-no-gui"))
			guiApp = false;
	if (guiApp) {
		auto app = std::make_unique<QApplication>(argc, argv);
		MainWindow main;
		main.show();
		return app->exec();
	}
	else {
		// start non-GUI version...
		return -1;
	}
}
