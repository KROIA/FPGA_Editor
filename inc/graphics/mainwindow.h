#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "vector"
#include "shape.h"
#include "grid.h"

#include "gate.h"
#include "logicGate.h"
#include "logicGate_CONST.h"
#include "logicGateImported.h"
#include "module.h"
#include "debug.h"

#include <QMenu>
#include "ribbon.h"

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
            void onFrameUpdate();
            void onViewSimulate();

            void onToolMoveButtonPressed();
            void onToolConnectButtonPressed();
            void onToolDisconnectButtonPressed();
            void onToolDeleteButtonPressed();

            void onToolAddIOInputButtonPressed();
            void onToolAddIOOutputButtonPressed();
            void onToolMoveModuleUp();

            void onToolAddGateAND();
            void onToolAddGateOR();
            void onToolAddGateXOR();
            void onToolAddGateNAND();
            void onToolAddGateNOR();
            void onToolAddGateXNOR();
            void onToolAddGateNOT();
            void onToolAddConst();



            void resizeEvent(QResizeEvent* e);
            void checkKeyEvents();

            void onSimulateIteration();

            // From Shapes
            void onDeleteRequest(Shape *shape);

            // From Gates
            void onPlaced(Gate *gate);
            void onStartMoving(Gate *gate);
            void onClicked(Gate *gate);
            void onAddCopyOf(Gate *gate);

            // From Modules
            void onInsideViewEnter(Module *module);
            void onInsideViewExit(Module *module);

    private:


        void addGate(Gate *gate);
        void addLogicGate(LogicGate::Logic logic);
        QToolButton* addRibbonButton(const string &tab,
                                     const string &group,
                                     const string &name,
                                     const string &logo,
                                     const string &toolTip,
                                     void (MainWindow::*slot)(void));

        void setupRibbon();

        Grid *m_grid;
        vector<Gate*>  m_shapes;
        vector<Shape*>  m_shapesDeleteLater;
        vector<Gate*>   m_shapesAddLater;
        Module *m_topModule;
        vector<Module*> m_openModuleStack;

        Gate*   m_makeCopyOf;
        QTimer  frameUpdateTimer;
        Canvas* SFMLView;

        Tool *m_tool_move;
        Tool *m_tool_connect;
        Tool *m_tool_disconnect;
        Tool *m_tool_moduleRemover;

        Ui::MainWindow *ui;

        QTimer *m_simulationTimer;
        int m_simulationTimeMS;

        // Buttons
            // Tools
        QToolButton *m_moveToolButton;
        QToolButton *m_connectToolButton;
        QToolButton *m_disconnectToolButton;
        QToolButton *m_deleteToolButton;
        void unselectAllButtons();

};
#endif // MAINWINDOW_H
