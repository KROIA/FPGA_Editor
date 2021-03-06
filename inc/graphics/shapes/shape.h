#ifndef SHAPE_H
#define SHAPE_H
#include <SFML/Graphics.hpp>

#include "tool.h"
#include "AABB.h"
#include "toollistener.h"
#include "physics.h"
#include <QObject>
#include <QDebug>



using std::vector;
using sf::Vector2f;
using sf::Vector2i;
using sf::Color;

class Shape :   public QObject, public ToolListener
{
        Q_OBJECT
    public:
        Shape();
        Shape(Shape *parent);
        Shape(const Shape &other);
        virtual ~Shape();

        void setOwner(Shape *parent);
        Shape *getOwner() const;
        void setPos(Vector2i pos);
        Vector2i getPos() const;

        void setVisibility(bool visible);
        bool isVisible() const;

        virtual void draw(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));
        virtual void drawDebug(sf::RenderWindow *window, Vector2i drawPos = Vector2i(0,0));



        virtual void updateEvents(vector<sf::Event> *events,
                                  bool mouseDoubleClickEvent,
                                  sf::RenderWindow *window);

        static void global_updateEvents(vector<sf::Event> *events,
                                        sf::RenderWindow *window/*,
                                        Tool *selectedTool*/);
        static void global_draw(sf::RenderWindow *window);
        static void global_drawDebug(sf::RenderWindow *window);

        bool overlaps(const Shape &other);

        size_t getObjectID();

    signals:


    protected:
        // From ToolListener
        virtual void toolChanged(Tool *oldTool, Tool *newTool);
        virtual void toolCleared(Tool *oldTool);

        virtual void onEventUpdate(sf::RenderWindow *window);
        virtual void onMouseOver(Vector2i mousePos);
        virtual void onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos);
        virtual void onDoubleKlick();

        Vector2i getGridSnaped(Vector2i pos);


        Shape *m_parent;
        Vector2i m_position;
        AABB m_boundingBox;
        bool m_isVisible;

        static Vector2i m_gridsize;
        bool m_useGrid;

    private:



        static vector<Shape*> global_shapeList;


};
#endif
