#ifndef AABB_H
#define AABB_H

#include "SFML/Graphics.hpp"

using sf::Vector2i;
using sf::Color;

class AABB{
    public:
        AABB();
        AABB(const AABB &other);
        AABB(Vector2i begin, Vector2i end);
        ~AABB();

        const AABB &operator=(const AABB &other);

        void set(Vector2i begin, Vector2i end);
        void begin(Vector2i begin);
        void end(Vector2i end);

        Vector2i begin() const;
        Vector2i end() const;

        int width() const;
        int height() const;

        bool overlaps(const AABB &other);
        bool contains(const AABB &other);
        bool contains(const Vector2i point);

        void draw(sf::RenderWindow *window, Color color = Color(100,100,100));



    protected:

    private:
        Vector2i m_begin;
        Vector2i m_end;
};
#endif
