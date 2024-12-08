#pragma once

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

/**
 * Modes of rendering
 */
typedef enum {
    CGV_DISPLAY, ///< The scene is rendered as usual
    CGV_SELECT ///< The scene is rendered in selection mode to calculate the selected object with the color buffer technique
} RenderMode;


/**
 * The instances of this class are 3D boxes. They are prepared to use the color buffer technique. 
 */
class cgvBox {

	GLubyte color_as_ID[3]={0,0,0}; ///< RGB color used as an identifier
	bool selected=false; ///< Indicate whether the box is selected or not

public:
	cgvBox() = default; 
	cgvBox(GLubyte _r, GLubyte _g, GLubyte _b);
	cgvBox(GLubyte _color_as_ID[3]);
	~cgvBox() = default; 

	void render(RenderMode mode);

	void select(GLubyte c[3]); 

	bool isSelected() const { return selected; }

};

