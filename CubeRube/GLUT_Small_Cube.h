#ifndef _SMALL_CUBE_H
#define _SMALL_CUBE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>


// класс частей кубика-рубика, большой кубик будет состоять из 27 маленьких.
struct GLUT_Small_Cube
{
    // шесть граней куба - шесть цветов
    unsigned int color[6]; // (верх, низ, впереди, сзади, лево, право)
    // размер ребра
    double size;

    GLUT_Small_Cube()
    {
        // по умолчанию черный цвет
        memset(color, 0, sizeof(color));
        size = 0.0;
    }

    // поворот на плоскости X0Y
    void rotateZ()
    {
        unsigned int tmp = color[5];
        color[5] = color[3];
        color[3] = color[4];
        color[4] = color[2];
        color[2] = tmp;
    }

    // поворот на плоскости X0Z
    void rotateY()
    {
        unsigned int tmp = color[2];
        color[2] = color[1];
        color[1] = color[3];
        color[3] = color[0];
        color[0] = tmp;
    }

    // поворот на плоскости Y0Z
    void rotateX()
    {
        unsigned int tmp = color[0];
        color[0] = color[4];
        color[4] = color[1];
        color[1] = color[5];
        color[5] = tmp;
    }

    void setColor(int i, int color)
    {
        this->color[i] = color;
    }

    unsigned char *at(int i)
    {
        // разбиваем color[i] на 3 составляющих
        // например для 0xFF0000 RGB(FF, 0, 00) - красный цвет;
        _color[0] = color[i] >> 16;
        _color[1] = color[i] >>  8;
        _color[2] = color[i]      ;
        return _color;
    }

    // отрисовка куба:
    // устанавливаем цвет и нормали
    void draw()
    {
        glPushMatrix();
        glBegin(GL_QUADS);

        // верх
        glColor3ubv(at(0));
        glNormal3f(0, 0, 1);
        glVertex3f(size, size, size);          
        glVertex3f(0, size, size);          
        glVertex3f(0, 0, size);          
        glVertex3f(size, 0, size);      
        
        // низ
        glColor3ubv(at(1));
        glNormal3f(0, 0, -1);
        glVertex3f(size, 0, 0);
        glVertex3f(0, 0, 0);   
        glVertex3f(0, size, 0);  
        glVertex3f(size, size, 0);

        // спереди
        glColor3ubv(at(2));
        glNormal3f(0, -1, 0);
        glVertex3f(size, 0, size);
        glVertex3f(0, 0, size);   
        glVertex3f(0, 0, 0);      
        glVertex3f(size, 0, 0); 

        // сзади
        glColor3ubv(at(3));
        glNormal3f(0, 1, 0);
        glVertex3f(size, size, 0);   
        glVertex3f(0, size, 0);      
        glVertex3f(0, size, size);   
        glVertex3f(size, size, size);

        // слева
        glColor3ubv(at(4));
        glNormal3f(-1, 0, 0);
        glVertex3f(0, size, size);   
        glVertex3f(0, size, 0);      
        glVertex3f(0, 0, 0);         
        glVertex3f(0, 0, size);      

        // справа
        glColor3ubv(at(5));
        glNormal3f(1, 0, 0);
        glVertex3f(size, size, 0);
        glVertex3f(size, size, size);
        glVertex3f(size, 0, size);   
        glVertex3f(size, 0, 0);      

        glEnd();
        glPopMatrix();
    }

    // отрисовка куба со смещением (x, y, z)
    void draw(double x, double y, double z)
    {
        glPushMatrix();
        glTranslated(x, y, z);
        draw();
        glPopMatrix();
    }

private:
    unsigned char _color[4];
};


#endif