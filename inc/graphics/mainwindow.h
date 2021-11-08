#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "vector"
#include "shape.h"
#include "grid.h"

#include "gate.h"
#include "logicGate_AND.h"
#include "logicGate_CONST.h"
#include "debug.h"

#include <QMenu>
#include "ribbon.h"

#include "blockParser.h"
#include "QTimer"


using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();



    private slots:
            void onViewSimulate();

            void onToolMoveButtonPressed();
            void onToolConnectButtonPressed();
            void onToolDisconnectButtonPressed();

            void resizeEvent(QResizeEvent* e);
            void checkKeyEvents();

            void onSimulateIteration();

    private:
        //void resetToolButtonState();

        void setupRibbon();

        Grid *m_grid;
        vector<Shape*>  m_shapes;
        Canvas* SFMLView;

        Tool *m_tool_move;
        Tool *m_tool_connect;
        Tool *m_tool_disconnect;

        Ui::MainWindow *ui;

        QTimer *m_simulationTimer;
        int m_simulationTimeMS;

        // Buttons
            // Tools
        QToolButton *m_moveToolButton;
        QToolButton *m_connectToolButton;
        QToolButton *m_disconnectToolButton;
};
#endif // MAINWINDOW_H
