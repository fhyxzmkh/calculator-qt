#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLineEdit* m_line;
    double m_lastValue {};

    enum Operator {
        None,
        Plus,
        Minus,
        Multiply,
        Divide
    };

    Operator m_lastOperator = None;

    void setLineText(QString);

    void pressOperator(Operator);
    void pressNumber(int);
    void pressEqual();

    void pressBackSpace();
    void pressDot();
    void pressClear();

};
#endif // MAINWINDOW_H
