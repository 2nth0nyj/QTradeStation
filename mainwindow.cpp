#include "mainwindow.h"
#include "dashboardwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QStyle>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("QTradeStation");

    // Central area with stacked widget to switch between views
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Left navigation tree, docked like a trade-station side panel.
    auto *navDock = new QDockWidget(QStringLiteral("Navigation"), this);
    navDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    navDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    navDock->setObjectName(QStringLiteral("NavDock"));

    m_tree = new QTreeWidget(navDock);
    m_tree->setHeaderHidden(true);
    m_tree->setUniformRowHeights(true);
    m_tree->setAnimated(true);
    navDock->setWidget(m_tree);
    addDockWidget(Qt::LeftDockWidgetArea, navDock);

    buildTree();
    setupDashboard();
    setupPlaceholderPages();

    // Connect tree item clicks
    connect(m_tree, &QTreeWidget::itemClicked,
            this, &MainWindow::onTreeItemClicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::buildTree()
{
    const QIcon folderIcon = style()->standardIcon(QStyle::SP_DirIcon);

    auto addFolder = [&](QTreeWidgetItem *parent, const QString &text) {
        auto *item = new QTreeWidgetItem(parent);
        item->setText(0, text);
        item->setIcon(0, folderIcon);
        return item;
    };

    // Top-level nodes.
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Dashboard"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Orders"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Positions"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Markets"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("News"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Portfolio Analytics"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Accounts"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Settings"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Help"));

    // Select "Dashboard" by default so the tree highlights the active page.
    m_tree->setCurrentItem(m_tree->topLevelItem(0));
}

void MainWindow::setupDashboard()
{
    m_dashboard = new DashboardWidget(this);

    // Add 60 sample trading cards to fill the screen, each tagged with its broker / venue.
    m_dashboard->addCard("NYSE",      "AAPL",   210.45, "Futu");
    m_dashboard->addCard("NASDAQ",    "GOOGL",  185.20, "IB");
    m_dashboard->addCard("NASDAQ",    "MSFT",   425.30, "Futu");
    m_dashboard->addCard("NYSE",      "JPM",    198.75, "IB");
    m_dashboard->addCard("NYSE",      "TSLA",   245.60, "Binance");
    m_dashboard->addCard("NASDAQ",    "AMZN",   178.90, "Futu");
    m_dashboard->addCard("NYSE",      "BAC",     39.55, "IB");
    m_dashboard->addCard("NASDAQ",    "NVDA",   880.10, "Binance");
    m_dashboard->addCard("NYSE",      "WMT",    172.30, "Futu");
    m_dashboard->addCard("NYSE",      "XOM",    118.75, "IB");
    m_dashboard->addCard("NASDAQ",    "META",   505.60, "Futu");
    m_dashboard->addCard("NYSE",      "KO",      65.20, "IB");
    m_dashboard->addCard("NASDAQ",    "AMD",    162.80, "Binance");
    m_dashboard->addCard("NYSE",      "DIS",    112.40, "Futu");
    m_dashboard->addCard("NYSE",      "PFE",     28.90, "IB");
    m_dashboard->addCard("NASDAQ",    "TSM",    168.50, "Futu");
    m_dashboard->addCard("NYSE",      "V",      278.30, "IB");
    m_dashboard->addCard("NASDAQ",    "NFLX",   675.20, "Binance");
    m_dashboard->addCard("NYSE",      "JNJ",    148.60, "Futu");
    m_dashboard->addCard("NYSE",      "HD",     345.80, "IB");
    m_dashboard->addCard("NASDAQ",    "INTC",    31.45, "Futu");
    m_dashboard->addCard("NYSE",      "PG",     165.90, "IB");
    m_dashboard->addCard("NASDAQ",    "ADBE",   495.30, "Binance");
    m_dashboard->addCard("NYSE",      "CRM",    245.70, "Futu");
    m_dashboard->addCard("NYSE",      "ABNB",   155.40, "IB");
    m_dashboard->addCard("NASDAQ",    "CSCO",    48.25, "Futu");
    m_dashboard->addCard("NYSE",      "BA",     182.90, "IB");
    m_dashboard->addCard("NASDAQ",    "QCOM",   205.60, "Binance");
    m_dashboard->addCard("NYSE",      "CAT",    335.10, "Futu");
    m_dashboard->addCard("NYSE",      "GE",     168.75, "IB");
    m_dashboard->addCard("NASDAQ",    "AMAT",   225.40, "Futu");
    m_dashboard->addCard("NYSE",      "IBM",    185.30, "IB");
    m_dashboard->addCard("NASDAQ",    "MU",     128.90, "Binance");
    m_dashboard->addCard("NYSE",      "UPS",    148.20, "Futu");
    m_dashboard->addCard("NYSE",      "MMM",    102.50, "IB");
    m_dashboard->addCard("NASDAQ",    "LRCX",   980.60, "Futu");
    m_dashboard->addCard("NYSE",      "AXP",    245.80, "IB");
    m_dashboard->addCard("NASDAQ",    "SNPS",   610.40, "Binance");
    m_dashboard->addCard("NYSE",      "BLK",    835.20, "Futu");
    m_dashboard->addCard("NYSE",      "GS",     485.70, "IB");
    m_dashboard->addCard("NASDAQ",    "MRVL",    72.30, "Futu");
    m_dashboard->addCard("NYSE",      "NKE",     95.60, "IB");
    m_dashboard->addCard("NASDAQ",    "PANW",   295.80, "Binance");
    m_dashboard->addCard("NYSE",      "SBUX",    78.40, "Futu");
    m_dashboard->addCard("NYSE",      "TXN",    198.50, "IB");
    m_dashboard->addCard("NASDAQ",    "KLAC",   780.20, "Futu");
    m_dashboard->addCard("NYSE",      "UNH",    528.90, "IB");
    m_dashboard->addCard("NASDAQ",    "ASML",   950.30, "Binance");
    m_dashboard->addCard("NYSE",      "MA",     468.10, "Futu");
    m_dashboard->addCard("NYSE",      "CVX",    158.75, "IB");
    m_dashboard->addCard("NASDAQ",    "COST",   725.40, "Futu");
    m_dashboard->addCard("NYSE",      "ABT",    112.60, "IB");
    m_dashboard->addCard("NASDAQ",    "INTU",   635.80, "Binance");
    m_dashboard->addCard("NYSE",      "LOW",    235.20, "Futu");
    m_dashboard->addCard("NYSE",      "TMO",    585.90, "IB");
    m_dashboard->addCard("NASDAQ",    "CMCSA",    40.30, "Futu");
    m_dashboard->addCard("NYSE",      "MDT",     84.50, "IB");
    m_dashboard->addCard("NASDAQ",    "AMGN",   315.70, "Binance");
    m_dashboard->addCard("NYSE",      "PM",     108.40, "Futu");
    m_dashboard->addCard("NYSE",      "SPGI",   445.20, "IB");
    m_dashboard->addCard("NASDAQ",    "GILD",    72.80, "Futu");
    m_dashboard->addCard("NYSE",      "RTX",    105.60, "IB");
    m_dashboard->addCard("NASDAQ",    "ADI",    238.90, "Binance");
    m_dashboard->addCard("NYSE",      "PLD",    125.30, "Futu");
    m_dashboard->addCard("NYSE",      "ETN",    318.70, "IB");
    m_stackedWidget->addWidget(m_dashboard);
    m_stackedWidget->setCurrentWidget(m_dashboard);
}

void MainWindow::setupPlaceholderPages()
{
    // List of tree item names (excluding "Dashboard" which has its own widget)
    const QStringList pageNames = {
        QStringLiteral("Orders"),
        QStringLiteral("Positions"),
        QStringLiteral("Markets"),
        QStringLiteral("News"),
        QStringLiteral("Portfolio Analytics"),
        QStringLiteral("Accounts"),
        QStringLiteral("Settings"),
        QStringLiteral("Help")
    };

    for (const QString &name : pageNames) {
        auto *page = new QWidget(this);
        auto *layout = new QVBoxLayout(page);
        auto *label = new QLabel(
            QStringLiteral("%1 - Coming Soon").arg(name), page);
        QFont font = label->font();
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        m_stackedWidget->addWidget(page);
        m_placeholderPages.append(page);
    }
}

void MainWindow::onTreeItemClicked(QTreeWidgetItem *item, int /*column*/)
{
    const QString text = item->text(0);

    if (text == QStringLiteral("Dashboard")) {
        m_stackedWidget->setCurrentWidget(m_dashboard);
        return;
    }

    // Map tree item names to placeholder page indices
    // The placeholder pages are created in the same order as the tree items
    // (excluding Dashboard which is at index 0)
    const QStringList pageNames = {
        QStringLiteral("Orders"),
        QStringLiteral("Positions"),
        QStringLiteral("Markets"),
        QStringLiteral("News"),
        QStringLiteral("Portfolio Analytics"),
        QStringLiteral("Accounts"),
        QStringLiteral("Settings"),
        QStringLiteral("Help")
    };

    int index = pageNames.indexOf(text);
    if (index >= 0 && index < m_placeholderPages.size()) {
        m_stackedWidget->setCurrentWidget(m_placeholderPages.at(index));
    }
}
