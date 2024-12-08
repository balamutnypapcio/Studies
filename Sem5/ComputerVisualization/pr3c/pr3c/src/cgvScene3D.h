#pragma once

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include "iostream"
#endif


#include <vector>
#include "cgvBox.h"

using namespace std;

/**
 * cgvScene3D contains the basic functionality to render a scene.
 */
class cgvScene3D {
private:
    vector<cgvBox> boxes; ///< List of boxes in the scene

    // TODO: Section B: Add the required attributes to be able to transform the selected box.

    // Additional attributes
    bool isAnyBoxSelected = false; // Whether any box is selected
    GLfloat rotation[3][2];
    bool axes = true; ///< It indicates whether the axes are rendered or not


public:
    // Default constructor and destructor
    cgvScene3D();

    ~cgvScene3D() = default;

    // Methods
    // method with the OpenGL calls to render the scene
    void render(RenderMode mode);

    void assignSelection(GLubyte _c[3]);

    bool get_axes() { return axes; };
    void set_axes(bool _axes) { axes = _axes; };
    void updateRotation(GLint x, GLint y);

    bool return_isAnyBoxSelected() { return isAnyBoxSelected; };

private:
    void draw_axes();
};
