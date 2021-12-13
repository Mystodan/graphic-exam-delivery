#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include "stb_image.h"


#ifndef SPRITE_H
#define SPRITE_H


/**
 *  A storageclass for integer rectangles
 */
class intRect {
public:
    int     x0, y0,
            x1, y1;
    intRect(){};
    intRect(int x0, int y0, int x1, int y1) { this->x0 = x0; this->y0 = y0; this->x1 = x1; this->y1 = y1; }
};


/**
 *  A storageclass for float rectangles
 */
class floatRect {
public:
    float   x0, y0,
            x1, y1;
    floatRect(){};
    floatRect(float x0, float y0, float x1, float y1) { this->x0 = x0; this->y0 = y0; this->x1 = x1; this->y1 = y1; }
};


/**
 *  A class for drawing images onto the screen
 */
class Sprite {
private:
    float           posX,                   // X-position of the sprite on the screen
                    posY,                   // Y-position of the sprite on the screen
                    sizeX,                  // How wide the sprite is on the screen (2=full width)
                    sizeY,                  // how high the sprite is on the screen (2=full height)
                    angle;                  // Angle of the sprite in degrees (anti-clockwise)

    char*           spritesheet_filepath;   // (Absolute) filepath of the spritesheet
    floatRect       spritesheet_texRect;    // The rectangle to sample data from within the image file
    intRect         spritesheet_spriteRect; // The spritesheet's dimensions

    int             anim_step;              // Which step of its animation the sprite is in

    std::vector<float>*         vertices;
    std::vector<unsigned int>*  indices;
    GLuint                      vao, vbo, ebo, tex, shader;


public:
    float           getPositionX()  { return posX; }
    float           getPositionY()  { return posY; }
    float           getWidth()      { return sizeX; }
    float           getHeight()     { return sizeY; }
    float           getAngle()      { return angle; }
    int             getAnimStep()   { return anim_step; }

    Sprite () {}
    Sprite( char*       spritesheet_filepath,   GLuint   shader, 
            floatRect   spritesheet_texRect,    intRect  spritesheet_spriteRect,
            float       position_X = 0.f,       float    position_Y = 0.f, 
            float       size_X     = 1.f,       float    size_Y     = 1.f, 
            float       angle      = 0.f );
    
    void init_vbo( float posX  = 0.f, float posY  = 0.f,
                   float sizeX = 1.f, float sizeY = 1.f, 
                   float angle = 0.f );

    void init_spritesheet(char* filepath, floatRect texRect, intRect spriteRect);
    
    ~Sprite();
    
    void setAnimationStep   ( int step );
    void setTransformation  ( float x = NULL, float y = NULL, float width = NULL, float height = NULL, float angle = NULL);
    void setPosition        ( float x, float y );
    void setSize            ( float width, float height );
    void setAngle           ( float angle );
    void update_transformation();

    void draw();

    template <typename T>
    int  sizeof_v(std::vector<T> v);
    void rotate_point(float& px, float& py, float angle, float ox, float oy);
};


#endif //SPRITE_H