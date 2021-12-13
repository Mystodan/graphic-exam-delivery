#include "Sprite.h"
#include "functions.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/**
 *  Deconstructor
 */
Sprite::~Sprite() {
    glDeleteProgram(shader);
    CleanVAO(vao, &ebo);

    delete vertices;
    delete indices;
}


/**
*   Initializes a new sprite with the given spritesheet and other variables
*   @param spritesheet_filepath - Global filepath to the texture of the sprite
*   @param texRect - The bounds of the rectangle which data in the image is sampled from
*   @param spriteRect - The dimensions of the spritesheet
*   @param position_X - The x-position of the sprite
*   @param position_Y - The y-position of the sprite
*   @param size_X - The width of the sprite (0-2)
*   @param size_Y - The height of the sprite (0-2)
*   @param angle - The angle of the sprite (0-2pi)
*/
Sprite::Sprite( char*       spritesheet_filepath,   GLuint   shader, 
                floatRect   spritesheet_texRect,    intRect  spritesheet_spriteRect,
                float       position_X /*= 0.f*/,   float    position_Y /*= 0.f*/, 
                float       size_X     /*= 1.f*/,   float    size_Y     /*= 1.f*/, 
                float       angle      /*= 0.f*/ ) {

    // Set physical sprite vars
    this->shader = shader;
    posX = position_X;
    posY = position_Y;
    sizeX = size_X;
    sizeY = size_Y;
    this->angle = angle;
    anim_step = 0;
        
    // Initialize shaderprogram parameters
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "u_TransformationMat"), 1, false, glm::value_ptr(glm::mat4(1)));

    // Initialize spritesheet
    init_spritesheet(spritesheet_filepath, spritesheet_texRect, spritesheet_spriteRect);
}


/**
  * Sets the animation step and updates the sprite's texture
  * @param step - The animationstep
  */
void Sprite::setAnimationStep(int step) {
    // Get the width and height of the spritesheet
    int     sw = spritesheet_spriteRect.x1 - spritesheet_spriteRect.x0,
            sh = spritesheet_spriteRect.y1 - spritesheet_spriteRect.y0;

    // Get the width and height of the texture rectangle
    float   tw = spritesheet_texRect.x1 - spritesheet_texRect.x0,
            th = spritesheet_texRect.y1 - spritesheet_texRect.y0;

    // Get anim_step
    anim_step = step % (sw * sh);

    // Find x and y of the current tile on the spritesheet
    int     sx = spritesheet_spriteRect.x0 + anim_step % sw,
            sy = spritesheet_spriteRect.y0 + floor(anim_step / sw);

    // Translate those to x and y on the texture rectangle
    float   tx = spritesheet_texRect.x0 + (float)sx/(float)sw*tw,
            ty = spritesheet_texRect.y0 + (float)sy/(float)sh*th;

    // Using openGL's coordinate-system would mean that the bottom-left tile of the spritesheet
    // would be the first animation step. This code flips the tiles so that the top-left tile
    // is the first step instead.
    ty = spritesheet_spriteRect.y1 - ty;


    // Get the width and height of one sprite within the texture
    float   tsw = tw/(float)sw,
            tsh = th/(float)sh;

    // Update the texture-coordinates for the buffer with the new rectangle's boundries
    (*vertices)[2 + 4*0 + 0] = tx + tsw;    // Top right
    (*vertices)[2 + 4*0 + 1] = ty + tsh;
    (*vertices)[2 + 4*1 + 0] = tx + tsw;    // Bottom right
    (*vertices)[2 + 4*1 + 1] = ty;
    (*vertices)[2 + 4*2 + 0] = tx;          // Bottom left
    (*vertices)[2 + 4*2 + 1] = ty;
    (*vertices)[2 + 4*3 + 0] = tx;          // Top left
    (*vertices)[2 + 4*3 + 1] = ty + tsh;

    // Update buffer data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_v(*vertices), &(*vertices)[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
 *  Sets the base transformations of the vbo
 *  @param ...
 */
void Sprite::init_vbo( float posX /*= 0.f*/, float posY /*= 0.f*/, float sizeX /*= 1.f*/, float sizeY /*= 1.f*/, float angle /*= 0.f*/) {
    // Get the points in the quad without any rotation and their centre
    float   topRx = posX + sizeX / 2.f,
            topRy = posY + sizeY / 2.f,
            botRx = posX + sizeX / 2.f,
            botRy = posY - sizeY / 2.f,
            botLx = posX - sizeX / 2.f,
            botLy = posY - sizeY / 2.f,
            topLx = posX - sizeX / 2.f,
            topLy = posY + sizeY / 2.f,
            centrex = (topRx + botRx + botLx + topLx) / 4.f,
            centrey = (topRy + botRy + botLy + topLy) / 4.f;

    // Rotate them
    rotate_point(topRx, topRy, angle, centrex, centrey);
    rotate_point(botRx, botRy, angle, centrex, centrey);
    rotate_point(botLx, botLy, angle, centrex, centrey);
    rotate_point(topLx, topLy, angle, centrex, centrey);

    // Update the vertices with the new coordinates (and sizes)
    (*vertices)[4*0 + 0] = topRx; // top right
    (*vertices)[4*0 + 1] = topRy;
    (*vertices)[4*1 + 0] = botRx; // bottom right
    (*vertices)[4*1 + 1] = botRy;
    (*vertices)[4*2 + 0] = botLx; // bottom left
    (*vertices)[4*2 + 1] = botLy;
    (*vertices)[4*3 + 0] = topLx; // top left
    (*vertices)[4*3 + 1] = topLy;

    // Update buffer data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_v(*vertices), &(*vertices)[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
 *  Updates the sprite with its transformations (pos, angle, etc) using its shaderprogram
 */
void Sprite::update_transformation() {
    // Enable the right shaderprogram
    glUseProgram(shader);

    // Create transformation
    glm::mat4 translation    = glm::translate(glm::mat4(1), glm::vec3(posX, posY, 0.f));//posX, posY, 0.f));
    glm::mat4 rotation       = glm::rotate   (glm::mat4(1), glm::radians(angle), glm::vec3(0, 0, 1));
    glm::mat4 scale          = glm::scale    (glm::mat4(1), glm::vec3(sizeX, sizeY, 1.f));
    glm::mat4 transformation = translation * rotation * scale;

    // ...And lastly, get uniform position and send the data to the shader
    GLuint transformationmat = glGetUniformLocation(shader, "u_TransformationMat");
    glUniformMatrix4fv(transformationmat, 1, false, glm::value_ptr(transformation));
}


/**
 *  Sets the transformations of the sprite.
 *  Leaving the parameters as NULL won't change them.
 *  @param x - The x-position of the sprite
 *  @param y - The y-position of the sprite
 *  @param width - The width of the sprite (0-2)
 *  @param height - The height of the sprite (0-2)
 *  @param angle - The angle of the sprite (0-2pi)
 */
void Sprite::setTransformation(float x/*=NULL*/, float y/*=NULL*/, float width/*=NULL*/, float height/*=NULL*/, float angle/*=NULL*/) {
    // Set posX and posY
    posX = (x == NULL ? posX : x);
    posY = (y == NULL ? posY : y);

    // Set size
    sizeX = (width == NULL ? sizeX : width);
    sizeY = (height == NULL ? sizeX : height);

    // Set angle
    this->angle = (angle == NULL ? this->angle : angle);

    // Update
    update_transformation();
}


/**
 *  Sets the position of the sprite and updates it
 *  @param x - The x-position of the sprite
 *  @param y - The y-position of the sprite
 *  @see Sprite::update_transformation()
 */
void Sprite::setPosition(float x, float y) {
    // Set posX and posY
    posX = x;
    posY = y;

    // Update
    update_transformation();
}


/**
 *  Sets the size of the sprite and updates it
 *  @param width - The width of the sprite (0-2)
 *  @param height - The height of the sprite (0-2)
 *  @see Sprite::update_transformation()
 */
void Sprite::setSize(float width, float height) {
    // Set posX and posY
    sizeX = width;
    sizeY = height;

    // Update
    update_transformation();
}


/**
 *  Sets the angle of the sprite and updates it
 *  @param angle - The angle of the sprite
 *  @see Sprite::update_transformation()
 */
void Sprite::setAngle(float angle) {
    // Set angle
    this->angle = angle;

    // Update
    update_transformation();
}


/**
*  Initializes a spritesheet for the sprite
*   @param filepath - The (absolute) filepath of the spritesheet
*   @param texRect - The bounds of the rectangle which data in the image is sampled from
*   @param spriteRect - The dimensions of the spritesheet
*/
void Sprite::init_spritesheet( char* filepath, floatRect texRect, intRect spriteRect) {
    // Set vars
    spritesheet_filepath    = filepath;
    spritesheet_texRect     = texRect;
    spritesheet_spriteRect  = spriteRect;

    // Set up data for the square of the sprite
    vertices = new std::vector<float>{
            // Positions        // Texture coords
            0.25f,  0.25f,      1.0f, 1.0f, // top right        0
            0.25f,  -0.25f,     1.0f, 0.0f, // bottom right     1
            -0.25f, -0.25f,     0.0f, 0.0f, // bottom left      2
            -0.25f, 0.25f,      0.0f, 1.0f  // top left         3
    };

    // Set up data for vertices will belong to which triangle in the square
    indices = new std::vector<unsigned int> {
        0, 1, 3,
        1, 2, 3
    };

    // Generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_v(*vertices), &(*vertices)[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_v(*indices), &(*indices)[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    // Texture coorinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Generate texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the spritesheet's image
    stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load(spritesheet_filepath, &texWidth, &texHeight, &nrChannels, 0);
    if (!data) // TODO: Find a better way to handle errors like this
        std::cout << "Texture load failed." << '\n';

    // Set the texture's data to the loaded image's data, generate bitmap and free memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Initialize vbo, vao, ebo and the shader
    
    setAnimationStep(anim_step);
    init_vbo();
    update_transformation();
}


/**
 *  Draws the sprite onto the screen
 */
void Sprite::draw() {
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}









/**
 *  Gets the size of a vector and its contents in bytes
 */
template <typename T>
int Sprite::sizeof_v(std::vector<T> v) { return sizeof(std::vector<T>) + sizeof(T) * v.size(); }


/**
 *  Rotates a point around an origin
 */
void Sprite::rotate_point(float& px, float& py, float angle, float ox, float oy) {
    px -= ox; py -= oy;

    float   s = sin(angle),
            c = cos(angle),
            xn = px * c - py * s,
            yn = px * s + py * c;

    px = xn + ox;
    py = yn + oy;
}
