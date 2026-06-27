#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QList>

class QScrollArea;
class QGridLayout;
class QResizeEvent;
class QShowEvent;
class TradingCardWidget;

class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget *parent = nullptr);

    void addCard(const QString &market,
                 const QString &symbol,
                 double price,
                 const QString &broker = QStringLiteral("Futu"));

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // Recompute the number of columns for the current width and re-place
    // every card so the grid stays neat and fills the available space.
    void relayoutGrid();

    // Swap the dragged card with the card occupying the grid cell under the
    // given point (in container coordinates).
    void handleCardDrop(const QPoint &pos, qintptr draggedPtr);

    QScrollArea *m_scrollArea;
    QWidget *m_cardContainer;
    QGridLayout *m_gridLayout;
    QList<TradingCardWidget *> m_cards;
    int m_currentColumns = 0;

    static constexpr int MinColumns = 1;
    // Columns used before the real viewport width is known (e.g. before the
    // first show), so cards spread out instead of stacking on first launch.
    static constexpr int DefaultColumns = 3;
};

#endif // DASHBOARDWIDGET_H
