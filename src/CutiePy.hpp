#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
};

class PythonConsole: public QPlainTextEdit {
	Q_OBJECT

public:
	PythonConsole(QWidget* parent = nullptr);

private:
	QString currentCommand;
};
