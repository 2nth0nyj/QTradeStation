#ifndef TRADINGCARDWIDGET_H
#define TRADINGCARDWIDGET_H

#include <QFrame>
#include <QStringList>

class QLabel;
class QPushButton;
class QComboBox;
class QTimer;
class QVariantAnimation;
class QMouseEvent;

class TradingCardWidget : public QFrame
{
    Q_OBJECT

public:
    explicit TradingCardWidget(const QString &market,
                               const QString &symbol,
                               double price,
                               const QString &broker = QStringLiteral("Futu"),
                               QWidget *parent = nullptr);

    // Fixed card dimensions, exposed so the dashboard grid can compute how
    // many columns fit the current viewport width.
    static constexpr int CardWidth = 240;
    static constexpr int CardHeight = 190;

    // Supported broker / data sources for the per-card selector.
    static const QStringList Brokers;

    // Mime type used when dragging a card so the dashboard can identify it.
    static constexpr const char *CardMime = "application/x-qtrade-card";

signals:
    // Carries the currently selected broker (e.g. "Futu") and the symbol.
    void buyClicked(const QString &broker, const QString &symbol);
    void sellClicked(const QString &broker, const QString &symbol);
    void closeClicked(const QString &symbol);

protected:
    // Start a drag carrying this card's pointer so cards can be reordered.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QLabel *m_exchangeLabel;
    QComboBox *m_brokerCombo;
    QLabel *m_symbolLabel;
    QLabel *m_priceLabel;
    QPushButton *m_buyButton;
    QPushButton *m_sellButton;

    // Position where the mouse was pressed, used as the drag hotspot.
    QPoint m_dragStartPos;

    // Price simulation
    double m_price;
    QTimer *m_priceTimer;
    QVariantAnimation *m_flashAnimation;

    void updatePrice();
    void flashBackground(const QColor &flashColor);
};

#endif // TRADINGCARDWIDGET_H
