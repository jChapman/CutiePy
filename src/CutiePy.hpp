#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();

public slots:
    void boldToggled(bool boldOn);

private:
    QPlainTextEdit* mainText;
};

class Controller {
public:
    Controller(MainWindow* main);
    void requestAction(std::string action);

private:
    MainWindow* mainWindow;
};

class PythonConsole: public QPlainTextEdit {
	Q_OBJECT

public:
	PythonConsole(MainWindow *main, QWidget* parent = nullptr);
	~PythonConsole();

private:
	void keyPressEvent(QKeyEvent* e) override;
	void handleCommand();

	// Members
	QString currentCommand;
    MainWindow* mainWindow;
};
