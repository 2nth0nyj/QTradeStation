#include "dashboardwidget.h"
#include "tradingcardwidget.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

DashboardWidget::DashboardWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Title
    auto *titleLabel = new QLabel(QStringLiteral("Dashboard"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setContentsMargins(16, 12, 0, 4);
    outerLayout->addWidget(titleLabel);

    // Scroll area for cards
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_cardContainer = new QWidget();
    m_gridLayout = new QGridLayout(m_cardContainer);
    m_gridLayout->setContentsMargins(16, 8, 16, 16);
    m_gridLayout->setSpacing(16);
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_scrollArea->setWidget(m_cardContainer);
    outerLayout->addWidget(m_scrollArea);
}

void DashboardWidget::addCard(const QString &exchange,
                               const QString &symbol,
                               double price)
{
    auto *card = new TradingCardWidget(exchange, symbol, price, m_cardContainer);

    int row = m_cardCount / Columns;
    int col = m_cardCount % Columns;
    m_gridLayout->addWidget(card, row, col);

    connect(card, &TradingCardWidget::buyClicked, this,
            [](const QString &ex, const QString &sym) {
                qDebug("Buy  %s / %s", qPrintable(ex), qPrintable(sym));
            });
    connect(card, &TradingCardWidget::sellClicked, this,
            [](const QString &ex, const QString &sym) {
                qDebug("Sell %s / %s", qPrintable(ex), qPrintable(sym));
            });

    ++m_cardCount;
}
