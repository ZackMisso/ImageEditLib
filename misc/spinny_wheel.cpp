#include <iostream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

#define PI 3.1415269

struct Circle
{
    Circle()
        : xpos(0.f),
          ypos(0.f),
          outer_radius(0.f),
          inner_radius(0.f) { }

    Circle(float xpos,
           float ypos,
           float inner_radius,
           float outer_radius)
        : xpos(xpos),
          ypos(ypos),
          inner_radius(inner_radius),
          outer_radius(outer_radius) { }

    // returns a 1 if hit, otherwise returns 0
    float evaluate(float x, float y) const
    {
        float sqr_dist = (xpos - x) * (xpos - x) +
                         (ypos - y) * (ypos - y);

        if (sqr_dist > inner_radius * inner_radius &&
            sqr_dist < outer_radius * outer_radius)
        {
            return 1.f;
        }

        return 0.f;
    }

    float get_angle(float x, float y, float base_angle) const
    {
        // edge case
        if (x == xpos && y == ypos) return 0.f;

        float dx = x - xpos;
        float dy = y - ypos;
        float dist = sqrt(dx*dx + dy*dy);
        dx /= dist;
        dy /= dist;

        // a dot b = |a||b|cosT
        float xx = cos(base_angle);
        float yy = sin(base_angle);
        return acos(dx*xx + dy*yy);
    }

    float xpos;
    float ypos;
    float outer_radius;
    float inner_radius;
};

struct Line
{
    Line()
        : start_x(0.f),
          start_y(0.f),
          end_x(0.f),
          end_y(0.f),
          thickness(0.f) { }

    // returns a 1 if hit, otherwise returns 0
    float evaluate(float x, float y) const
    {
        // TODO
        // float sqr_dist = (xpos - x) * (xpos - x) +
        //                  (ypos - y) * (ypos - y);
        //
        // if (sqr_dist > inner_radius && sqr_dist < outer_radius)
        // {
        //     return 1.f;
        // }
        //
        // return 0.f;

        return 0.f;
    }

    float start_x;
    float start_y;
    float end_x;
    float end_y;
    float thickness;
};

struct DrawEvent
{
    DrawEvent()
        : start_frame(0), end_frame(0) { }
    DrawEvent(int start_frame, int end_frame)
        : start_frame(start_frame),
          end_frame(end_frame) { }

    virtual ~DrawEvent() { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const = 0;

    int start_frame;
    int end_frame;
};

struct DrawFullCircle : public DrawEvent
{
    DrawFullCircle(Circle& circle,
                   imedit::Pixel color)
        : DrawEvent(), circle(circle), color(color) { }
    DrawFullCircle(Circle& circle,
                   imedit::Pixel color,
                   int start_frame,
                   int end_frame)
        : DrawEvent(start_frame, end_frame),
          circle(circle),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        if (circle.evaluate(x, y) > 0.f) pixel = color;
    }

    Circle circle;
    imedit::Pixel color;
};

struct DrawCircleFromAngleOutwards : public DrawEvent
{
    DrawCircleFromAngleOutwards(Circle& circle,
                                float angle,
                                imedit::Pixel color,
                                int start_frame,
                                int end_frame)
        : DrawEvent(start_frame, end_frame),
          circle(circle),
          angle(angle),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        float ang = circle.get_angle(x, y, angle);

        // ang -= angle;
        // if (ang < PI) ang = 2*PI + ang;
        // if (ang > PI) ang = ang - 2*PI;

        // we want the returned angle to be in the range [0, 1]
        if (circle.evaluate(x, y) > 0.f)
            pixel = imedit::Pixel(ang / PI);
        // TODO: actually implement
        // if (circle.evaluate(x, y) > 0.f) pixel = color;
    }

    Circle circle;
    imedit::Pixel color;
    float angle;
};

// TODO: this is basically a shader, I should just implement this in shader toy
void evaluate_pixel(float x,
                    float y,
                    imedit::Pixel& pixel,
                    std::vector<DrawEvent*>& events)
{
    for (int i = 0; i < events.size(); ++i)
    {
        events[i]->evaluate(x, y, pixel, 0);
        // if (circle.evaluate(x, y) > 0.f)
        //     pixel = imedit::Pixel(1.f);
    }
}

int main(int argc, char* argv[])
{
    int x_res = 512;
    int y_res = 512;
    int samples = 1;
    pcg32 rng = pcg32(0x1417, 0x32ad);

    imedit::Image image = imedit::Image(x_res, y_res);
    Circle circle = Circle(0.5, 0.5, 0.40, 0.41);

    std::vector<DrawEvent*> events = std::vector<DrawEvent*>();
    events.push_back(new DrawCircleFromAngleOutwards(circle, PI / 4.0, imedit::Pixel(1.f, 0.f, 0.f), 0, 0));

    for (int i = 0; i < y_res; ++i)
    {
        for(int j = 0; j < x_res; ++j)
        {
            for (int k = 0; k < samples; ++k)
            {
                imedit::Pixel pixel = imedit::Pixel(0.f);
                evaluate_pixel(float(j) / float(x_res),
                               float(i) / float(y_res),
                               pixel,
                               events);
                image(j, i) += pixel;
            }
            image(j, i) /= float(samples);
        }
    }

    image.write("spinny_wheel.png");

    for (int i = 0; i < events.size(); ++i)
    {
        delete events[i];
        events.clear();
    }

    return 0;
}
