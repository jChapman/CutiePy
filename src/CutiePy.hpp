#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();

public slots:
    void boldToggled(bool boldOn);
    void starInsertRequested();

private:
    QPlainTextEdit* mainText;
};

class PythonConsole: public QPlainTextEdit {
	Q_OBJECT

public:
	PythonConsole(QWidget* parent = nullptr);
	~PythonConsole();

private:
	void keyPressEvent(QKeyEvent* e) override;
	void handleCommand();

	// Members
	QString currentCommand;
};
