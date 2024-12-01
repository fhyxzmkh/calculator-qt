#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Calculator");
    setFixedSize(400, 300);

    setCentralWidget(new QWidget);
    QGridLayout *layout = new QGridLayout;
    centralWidget()->setLayout(layout);

    m_line = new QLineEdit;
    m_line->setText("0");
    m_line->setValidator(new QDoubleValidator);
    layout->addWidget(m_line, 0, 0, 1, 4);

    for (int i = 1; i <= 9; ++ i) {
        QPushButton *btn = new QPushButton;
        btn->setText(QString::number(i));
        layout->addWidget(btn, 1 + (i-1) / 3, (i-1) % 3);

        connect(btn, &QPushButton::clicked, this, [this, i] {
            this->pressNumber(i);
        });
    }

    {
        QPushButton *btn = new QPushButton;
        btn->setText("0");
        layout->addWidget(btn, 4, 1);

        connect(btn, &QPushButton::clicked, this, [this] {
            this->pressNumber(0);
        });
    }

    {
        QPushButton *btn = new QPushButton;
        btn->setText("⌫");
        layout->addWidget(btn, 4, 2);

        connect(btn, &QPushButton::clicked, this, &MainWindow::pressBackSpace);
    }

    {
        QPushButton *btn = new QPushButton;
        btn->setText("0");
        layout->addWidget(btn, 4, 1);

        connect(btn, &QPushButton::clicked, this, [this] {
            this->pressNumber(0);
        });
    }

    {
        QPushButton *btn = new QPushButton;
        btn->setText(".");
        layout->addWidget(btn, 4, 0);

        connect(btn, &QPushButton::clicked, this, &MainWindow::pressDot);
    }

    QList<QPair<Operator, QString>> operators = {
        {Plus, "+"},
        {Minus, "-"},
        {Multiply, "*"},
        {Divide, "/"}
    };

    for (int i = 0; const auto& [opt, name] : operators) {
        QPushButton *btn = new QPushButton;
        btn->setText(name);
        layout->addWidget(btn, ++i, 3);

        connect(btn, &QPushButton::clicked, this, [this, opt] {
            this->pressOperator(opt);
        });
    }

    QPushButton *btn = new QPushButton;
    btn->setText("=");
    layout->addWidget(btn, 5, 0, 1, 3);

    {
        QPushButton *btn = new QPushButton;
        btn->setText("C");
        layout->addWidget(btn, 5, 3);

        connect(btn, &QPushButton::clicked, this, &MainWindow::pressClear);
    }


    connect(btn, &QPushButton::clicked, this, &MainWindow::pressEqual);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLineText(QString text)
{
    int pos;
    if (m_line->validator()->validate(text, pos) != QValidator::Invalid) {

        while (text.size() >= 2 && text[0] == '0' && (text[1] >= '0' && text[1] <= '9')) {
            text.remove(0, 1);
        }
        if (text.isEmpty()) {
            text = "0";
        }

        m_line->setText(text);
    }
}

void MainWindow::pressOperator(Operator opt) {
    double value = m_line->text().toDouble();
    m_lastValue = value;
    m_lastOperator = opt;

    setLineText("0");
}

void MainWindow::pressNumber(int i)
{
    QString text = m_line->text();
    text += QString::number(i);
    setLineText(text);
}

void MainWindow::pressEqual()
{
    double value1 = m_lastValue;
    double value2 = m_line->text().toDouble();
    double value {};

    switch (m_lastOperator) {
        case None: value = value2;
            break;
        case Plus: value = value1 + value2;
            break;
        case Minus: value = value1 - value2;
            break;
        case Multiply: value = value1 * value2;
            break;
        case Divide: value = value1 / value2;
            break;
    }

    m_lastOperator = None;
    setLineText(QString::number(value, 'g', 10));
}

void MainWindow::pressBackSpace()
{
    QString text = m_line->text();
    if (!text.isEmpty()) {
        text.remove(text.size() - 1, 1);
    }

    setLineText(text);
}

void MainWindow::pressDot()
{
    QString text = m_line->text();
    text += ".";
    setLineText(text);
}

void MainWindow::pressClear()
{
    setLineText("0");
}
