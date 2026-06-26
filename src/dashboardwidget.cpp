#include "dashboardwidget.h"
#include "tradingcardwidget.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QShowEvent>
#include <QtMath>

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
    m_cards.append(card);

    connect(card, &TradingCardWidget::buyClicked, this,
            [](const QString &ex, const QString &sym) {
                qDebug("Buy  %s / %s", qPrintable(ex), qPrintable(sym));
            });
    connect(card, &TradingCardWidget::sellClicked, this,
            [](const QString &ex, const QString &sym) {
                qDebug("Sell %s / %s", qPrintable(ex), qPrintable(sym));
            });

    relayoutGrid();
}

void DashboardWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    relayoutGrid();
}

void DashboardWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // On first show the viewport finally has a real width. Force a full
    // re-layout so cards spread out into the grid instead of stacking.
    m_currentColumns = 0;
    relayoutGrid();
}

void DashboardWidget::relayoutGrid()
{
    const int viewportWidth = m_scrollArea->viewport()->width();
    const int left = m_gridLayout->contentsMargins().left();
    const int right = m_gridLayout->contentsMargins().right();
    const int spacing = m_gridLayout->spacing();

    const int usableWidth = viewportWidth - left - right;

    // Cards have a fixed width; just decide how many fit per row given the
    // current viewport width: usableWidth = cols*cardW + (cols-1)*spacing.
    const int cardWidth = TradingCardWidget::CardWidth;
    int columns;
    if (usableWidth <= 0) {
        // Viewport not laid out yet (first construction). Use a sane default
        // so cards spread out on first launch; showEvent will refine it.
        columns = DefaultColumns;
    } else {
        columns = (usableWidth + spacing) / (cardWidth + spacing);
    }
    if (columns < MinColumns)
        columns = MinColumns;

    m_currentColumns = columns;

    // No column stretch: keep each card at its fixed size and let the grid
    // align to the top-left. Extra space stays empty on the right.
    for (int col = 0; col < m_gridLayout->columnCount(); ++col)
        m_gridLayout->setColumnStretch(col, 0);

    // Re-place every card into its grid cell. We always redo the whole grid
    // because a newly added card must get a cell even when the column count
    // is unchanged (otherwise it would stack on top of an existing card).
    for (int i = 0; i < m_cards.size(); ++i) {
        const int row = i / columns;
        const int col = i % columns;
        m_gridLayout->addWidget(m_cards.at(i), row, col);
    }
}
