#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H
#include <SFML/Graphics.hpp>
#include <qwidget.h>
#include <qtimer.h>


class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
        //Q_OBJECT
    public:
        explicit QSFMLCanvas(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 20);
        virtual void showEvent(QShowEvent*);
        virtual QPaintEngine* paintEngine() const;
        virtual void paintEvent(QPaintEvent*);
        virtual ~QSFMLCanvas();
        virtual void OnInit();
        virtual void OnUpdate();
    private:
        QTimer myTimer;
        bool myInitialized;
};
#endif // SFMLCANVAS_H
