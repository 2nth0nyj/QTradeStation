#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class DashboardWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);

private:
    void buildTree();
    void setupDashboard();
    void setupPlaceholderPages();
    
    QTreeWidget *m_tree;
    QStackedWidget *m_stackedWidget;
    DashboardWidget *m_dashboard;
    QList<QWidget *> m_placeholderPages;
};

#endif // MAINWINDOW_H
