#include "tradingcardwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>
#include <QByteArray>
#include <QDataStream>
#include <QApplication>

const QStringList TradingCardWidget::Brokers = {
    QStringLiteral("Futu"),
    QStringLiteral("IB"),
    QStringLiteral("Binance"),
};

TradingCardWidget::TradingCardWidget(const QString &market,
                                     const QString &symbol,
                                     double price,
                                     const QString &broker,
                                     QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(2);
    setFixedSize(240, 190);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 12, 10);
    mainLayout->setSpacing(6);

    // Header row: market (e.g. NYSE/NASDAQ) on the left, broker selector on
    // the right so each card can choose its data source / venue.
    auto *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    m_exchangeLabel = new QLabel(market, this);
    QFont exchangeFont = m_exchangeLabel->font();
    exchangeFont.setPointSize(9);
    exchangeFont.setBold(false);
    m_exchangeLabel->setFont(exchangeFont);
    m_exchangeLabel->setStyleSheet("color: #888;");
    headerLayout->addWidget(m_exchangeLabel);
    headerLayout->addStretch();

    m_brokerCombo = new QComboBox(this);
    m_brokerCombo->addItems(Brokers);
    m_brokerCombo->setCurrentText(broker);
    m_brokerCombo->setStyleSheet(
        "QComboBox {"
        "  font-size: 9pt;"
        "  padding: 1px 4px;"
        "}");
    headerLayout->addWidget(m_brokerCombo);

    mainLayout->addLayout(headerLayout);

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

    // Connect signals — emit with the broker that is currently selected.
    connect(m_buyButton, &QPushButton::clicked, this, [this, symbol]() {
        emit buyClicked(m_brokerCombo->currentText(), symbol);
    });
    connect(m_sellButton, &QPushButton::clicked, this, [this, symbol]() {
        emit sellClicked(m_brokerCombo->currentText(), symbol);
    });
}

void TradingCardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Record the press position; the actual drag starts on mouse move
        // once it exceeds QApplication's drag distance threshold.
        m_dragStartPos = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void TradingCardWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    // Only start a drag once the mouse has moved past the standard threshold.
    if ((event->pos() - m_dragStartPos).manhattanLength()
        < QApplication::startDragDistance()) {
        return;
    }

    // Package this widget's pointer into the mime data so the dashboard can
    // tell which card is being dragged.
    QByteArray data;
    data.resize(sizeof(qintptr));
    *reinterpret_cast<qintptr *>(data.data()) = reinterpret_cast<qintptr>(this);

    auto *mime = new QMimeData;
    mime->setData(QLatin1String(CardMime), data);

    auto *drag = new QDrag(this);
    drag->setMimeData(mime);
    drag->setPixmap(grab());                 // visual: drag the card's image
    drag->setHotSpot(m_dragStartPos);        // under the cursor
    drag->exec(Qt::MoveAction);
}
