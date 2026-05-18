#ifndef ZONA_H
#define ZONA_H

class zona
{

public:

    float x_min, x_max;
    float y_min, y_max;

    zona(float x_min, float x_max,
         float y_min, float y_max)
        : x_min(x_min), x_max(x_max),
        y_min(y_min), y_max(y_max)
    {}

};

#endif // ZONA_H
