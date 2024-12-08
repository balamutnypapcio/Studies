#include <cstdlib>
#include <stdio.h>

#include "cgvScene3D.h"


// Constructor methods -----------------------------------

/**
 * Constructor method. Initially, it defines the vector of boxes with three of them. 
 */
cgvScene3D::cgvScene3D() {
    // TODO: Section A: Modify the constructor to assign adequate color to the boxes to be able to use the color buffer technique.
    axes = true;
    isAnyBoxSelected = false;

    GLubyte c[3][3] = {
        {255, 0, 0}, // Red
        {0, 255, 0}, // Green
        {0, 0, 255} // Blue
    };

    for (int i = 0; i < 3; ++i) {
        boxes.push_back(cgvBox(c[i]));
    }
}


// Public methods ----------------------------------------


/**
 * This method is called to render the scene
 * @param mode Identifier of the scene to be rendered
 * @pre It is assumed that the value of the parameter is valid
 * @post Render the scene normally (CGV_DISPLAY) or for selection using the color buffer technique (CGV_SELECT)
 */
void cgvScene3D::render(RenderMode mode) {
    // TODO: Section B: Add the required code to be able to transform the selected box.


    // lights
    GLfloat light0[4] = {5.0, 5.0, 5.0, 1}; // point light source
    glLightfv(GL_LIGHT0,GL_POSITION, light0); // this light is placed here and it remains still
    glEnable(GL_LIGHT0);

    // create the model
    glPushMatrix(); // store the model matrices

    // draw the axes
    if ((axes) && (mode == CGV_DISPLAY)) draw_axes();

    //

    for (int i = 0; i < boxes.size(); ++i) {
        glPushMatrix();

        // Apply transformation: translate and rotate
        glTranslatef(0, i, 0); // Stack boxes along the Y-axis
        glRotatef(rotation[i][0], 0, 1, 0);


        // Render the box
        boxes[i].render(mode);
        glPopMatrix();
    }

    glPopMatrix(); // restore the modelview matrix
}

/**
 * Select a box from the vector of boxes if needed
 * @param _c RBG color
 * @pre It is assumed that the parameters are valid
 * @post The boxes that correspond to the color _c as identifier are marked as selected, the rest as not selected.
 */
void cgvScene3D::assignSelection(GLubyte _c[3]) {
    // TODO: Section A. Add the required code to select the corresponding box if any of them can be selected.
    bool selectCheck = false;
    for (auto &box: boxes) {
        box.select(_c); // This will check if the color matches
        if (box.isSelected()) {
            selectCheck = true;
        }
    }
    isAnyBoxSelected = selectCheck;
    std::cout<<isAnyBoxSelected<<std::endl;
}


/**
 * Method to render the axes
 */
void cgvScene3D::draw_axes(void) {
    GLfloat red[] = {1, 0, 0, 1.0};
    GLfloat green[] = {0, 1, 0, 1.0};
    GLfloat blue[] = {0, 0, 1, 1.0};

    glBegin(GL_LINES);
    glMaterialfv(GL_FRONT,GL_EMISSION, red);
    glVertex3f(1000, 0, 0);
    glVertex3f(-1000, 0, 0);

    glMaterialfv(GL_FRONT,GL_EMISSION, green);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, -1000, 0);

    glMaterialfv(GL_FRONT,GL_EMISSION, blue);
    glVertex3f(0, 0, 1000);
    glVertex3f(0, 0, -1000);
    glEnd();
}

void cgvScene3D::updateRotation(GLint x, GLint y) {
    for (int i = 0; i < 3; ++i) {
        if (boxes[i].isSelected()) {
            rotation[i][0] += x;
            rotation[i][1] += y;
        }
    }
}
