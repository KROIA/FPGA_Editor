#ifndef CANVAS_H
#define CANVAS_H

#include "SFMLCanvas.h"
#include <SFML/Graphics.hpp>
#include "shape.h"
#include "tool.h"
#include <vector>
#include "debug.h"

using std::vector;

class Canvas : public QSFMLCanvas
{
        Q_OBJECT
    public :
        Canvas(QWidget* Parent,
                 const QPoint& Position,
                 const QSize& Size
                /* vector<Shape*> *shapes*/);
        ~Canvas();

        void addShape(size_t layer,Shape *shape);
        void removeShape(Shape *shape);
        void clearShape(size_t layer);
        void clearShape();

        void OnInit();
        void OnUpdate();

        void setPosition(QPoint &pos);
        void setSize(QSize &size);

        static Vector2i getMousePosition();

        void mouseDoubleClickEvent(QMouseEvent * e);
        void updateEvents();



    signals:
        void checkKeyEvents();

    private :
        void p_checkKeyEvents();
      //  void onKeyPressEvent(sf::Event *e);
      //  void onKeyReleaseEvent(sf::Event *e);
        void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow* window, float zoom);
        float m_zoomValue;
        bool m_moving;
        bool m_doubleClickEvent;
        Vector2f m_movingOldPos;

        Tool m_selectedTool;
        //vector<Shape*> *m_shapes;

        static Canvas *m_instance;
        vector<vector<Shape *> >m_shapeList;


        //sf::Clock myClock;
       /* sf::Texture myImage;
        sf::Sprite mySprite;*/
};
#endif // CANVAS_H
