#include "CutiePy.hpp"

#include <QDockWidget>
#include <QApplication>
#include <QRegExp>

#undef slots
#include <pybind11/embed.h>
#include <pybind11/iostream.h>
#define slots Q_SLOTS
namespace py = pybind11;

namespace {
    struct cout_redirect {
        cout_redirect(std::streambuf* new_buffer)
            : old(std::cout.rdbuf(new_buffer))
        { }

        ~cout_redirect() {
            std::cout.rdbuf(old);
        }

    private:
        std::streambuf* old;
    };
}


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

    py::initialize_interpreter();
}

PythonConsole::~PythonConsole() {
    py::finalize_interpreter();
}

void PythonConsole::keyPressEvent(QKeyEvent* e)
{
    // TODO handle ctrl+c
    switch (e->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        handleCommand();
        //QPlainTextEdit::keyPressEvent(e);
    case Qt::Key_Backspace:
        currentCommand.chop(1);
        QPlainTextEdit::keyPressEvent(e);
        break;
    default:
        if (e->text().length())
            currentCommand += e->text();
        QPlainTextEdit::keyPressEvent(e);
        break;
    }
}

void PythonConsole::handleCommand()
{
    QRegExp assignmentExpression("^\\w+\\s*=");
    // Try to determine if the current command is a statement
    bool isExpression = false;
    if (assignmentExpression.indexIn(currentCommand) == -1) {
        isExpression = true;
        currentCommand.prepend("_ = ");
    }

    try {
        py::exec(currentCommand.toStdString());
        if (isExpression) {
            auto out = py::eval("str(_)").cast<std::string>();
        }
    }
    catch (py::error_already_set const& pyError) {
        appendPlainText(QString(pyError.what()));
    }
    currentCommand = "";
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
