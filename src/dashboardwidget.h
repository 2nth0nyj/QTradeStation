#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>

class QScrollArea;
class QGridLayout;

class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget *parent = nullptr);

    void addCard(const QString &exchange,
                 const QString &symbol,
                 double price);

private:
    QScrollArea *m_scrollArea;
    QWidget *m_cardContainer;
    QGridLayout *m_gridLayout;
    int m_cardCount = 0;
    static constexpr int Columns = 3;
};

#endif // DASHBOARDWIDGET_H
