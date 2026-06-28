#include "tradingcardwidget.h"

#include <QLabel>
#include <QPushButton>
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
#include <QTimer>
#include <QVariantAnimation>
#include <QRandomGenerator>
#include <QPalette>
#include <QStyle>

TradingCardWidget::TradingCardWidget(const QString &market,
                                     const QString &symbol,
                                     double price,
                                     const QString &broker,
                                     QWidget *parent)
    : QFrame(parent), m_broker(broker)
{
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(2);
    setFixedSize(240, 190);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 12, 10);
    mainLayout->setSpacing(6);

    // Header row: market (e.g. NYSE/NASDAQ) on the left, broker icon and
    // close button on the right — all aligned in one row.
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

    // Broker icon (replaces the old combo box)
    m_brokerIcon = new QLabel(this);
    m_brokerIcon->setFixedSize(24, 24);
    m_brokerIcon->setScaledContents(true);
    loadBrokerIcon(broker);
    headerLayout->addWidget(m_brokerIcon);

    // Close button (X) to remove this card — aligned in the same header row
    auto *closeButton = new QPushButton(QStringLiteral("✕"), this);
    closeButton->setFixedSize(22, 22);
    closeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(0,0,0,0);"
        "  color: #999;"
        "  border: none;"
        "  font-size: 13pt;"
        "  font-weight: bold;"
        "  padding: 0px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f44336;"
        "  color: white;"
        "  border-radius: 11px;"
        "}");
    headerLayout->addWidget(closeButton);

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
        emit buyClicked(m_broker, symbol);
    });
    connect(m_sellButton, &QPushButton::clicked, this, [this, symbol]() {
        emit sellClicked(m_broker, symbol);
    });
    connect(closeButton, &QPushButton::clicked, this, [this, symbol]() {
        emit closeClicked(symbol);
    });

    // Price simulation — random updates every 2-5 seconds
    m_price = price;
    m_flashAnimation = nullptr;
    m_priceTimer = new QTimer(this);
    m_priceTimer->setInterval(QRandomGenerator::global()->bounded(2000, 5001));
    connect(m_priceTimer, &QTimer::timeout, this, &TradingCardWidget::updatePrice);
    m_priceTimer->start();
}

void TradingCardWidget::loadBrokerIcon(const QString &broker)
{
    QString iconPath;
    if (broker == QStringLiteral("Futu"))
        iconPath = QStringLiteral(":/futu.svg");
    else if (broker == QStringLiteral("IB"))
        iconPath = QStringLiteral(":/ib.svg");
    else if (broker == QStringLiteral("Binance"))
        iconPath = QStringLiteral(":/binance.svg");
    else
        return;

    QPixmap pix(iconPath);
    if (!pix.isNull()) {
        m_brokerIcon->setPixmap(pix);
    }
}

void TradingCardWidget::updatePrice()
{
    // Random price change: -3% to +3%
    const double change = m_price * (QRandomGenerator::global()->bounded(-300, 301) / 10000.0);
    const double oldPrice = m_price;
    m_price += change;

    // Update the price label
    m_priceLabel->setText(QString("$%1").arg(m_price, 0, 'f', 2));

    // Flash background: green for up, red for down
    if (m_price > oldPrice) {
        flashBackground(QColor(76, 175, 80, 180));   // green
    } else if (m_price < oldPrice) {
        flashBackground(QColor(244, 67, 54, 180));    // red
    }

    // Randomize next interval (2-5 seconds)
    m_priceTimer->setInterval(QRandomGenerator::global()->bounded(2000, 5001));
}

void TradingCardWidget::flashBackground(const QColor &flashColor)
{
    // Stop any ongoing flash animation and clear the pointer
    if (m_flashAnimation) {
        m_flashAnimation->stop();
        // Disconnect to prevent finished() from deleting again
        m_flashAnimation->disconnect();
        m_flashAnimation->deleteLater();
        m_flashAnimation = nullptr;
    }

    // Set the flash color as the background
    QPalette pal = palette();
    pal.setColor(QPalette::Window, flashColor);
    setPalette(pal);
    setAutoFillBackground(true);

    // Animate the background alpha from flashColor down to fully transparent
    auto *anim = new QVariantAnimation(this);
    anim->setDuration(800);
    anim->setStartValue(180);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    connect(anim, &QVariantAnimation::valueChanged, this, [this, flashColor](const QVariant &value) {
        if (!m_priceLabel) return; // guard against deleted card
        int alpha = value.toInt();
        QPalette p = palette();
        QColor c = flashColor;
        c.setAlpha(alpha);
        p.setColor(QPalette::Window, c);
        setPalette(p);
    });
    connect(anim, &QVariantAnimation::finished, this, [this]() {
        setAutoFillBackground(false);
        m_flashAnimation = nullptr;
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    m_flashAnimation = anim;
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
