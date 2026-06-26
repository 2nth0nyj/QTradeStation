#include "tradingcardwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

TradingCardWidget::TradingCardWidget(const QString &exchange,
                                     const QString &symbol,
                                     double price,
                                     QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(2);
    setMinimumSize(200, 160);
    setMaximumSize(260, 200);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 12, 10);
    mainLayout->setSpacing(6);

    // Exchange name
    m_exchangeLabel = new QLabel(exchange, this);
    QFont exchangeFont = m_exchangeLabel->font();
    exchangeFont.setPointSize(9);
    exchangeFont.setBold(false);
    m_exchangeLabel->setFont(exchangeFont);
    m_exchangeLabel->setStyleSheet("color: #888;");
    mainLayout->addWidget(m_exchangeLabel);

    // Trading symbol
    m_symbolLabel = new QLabel(symbol, this);
    QFont symbolFont = m_symbolLabel->font();
    symbolFont.setPointSize(14);
    symbolFont.setBold(true);
    m_symbolLabel->setFont(symbolFont);
    mainLayout->addWidget(m_symbolLabel);

    // Price
    m_priceLabel = new QLabel(QString("$%1").arg(price, 0, 'f', 2), this);
    QFont priceFont = m_priceLabel->font();
    priceFont.setPointSize(18);
    priceFont.setBold(true);
    m_priceLabel->setFont(priceFont);
    m_priceLabel->setStyleSheet("color: #2a82e4;");
    mainLayout->addWidget(m_priceLabel);

    mainLayout->addStretch();

    // Buy / Sell buttons row
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);

    m_buyButton = new QPushButton(QStringLiteral("Buy"), this);
    m_buyButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 6px 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }");
    m_buyButton->setMinimumHeight(32);

    m_sellButton = new QPushButton(QStringLiteral("Sell"), this);
    m_sellButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #f44336;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 6px 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e53935; }"
        "QPushButton:pressed { background-color: #c62828; }");
    m_sellButton->setMinimumHeight(32);

    buttonLayout->addWidget(m_buyButton);
    buttonLayout->addWidget(m_sellButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(m_buyButton, &QPushButton::clicked, this, [this, exchange, symbol]() {
        emit buyClicked(exchange, symbol);
    });
    connect(m_sellButton, &QPushButton::clicked, this, [this, exchange, symbol]() {
        emit sellClicked(exchange, symbol);
    });
}
