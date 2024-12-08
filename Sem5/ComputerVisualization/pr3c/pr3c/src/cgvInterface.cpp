#include <cstdlib>
#include <stdio.h>

#include "cgvInterface.h"


// Singleton pattern
cgvInterface *cgvInterface::instance = nullptr;


// Public methods ----------------------------------------
/**
 * Method to access the unique instance of the class. Singleton pattern
 * @return A reference to the unique instance of the class
 */
cgvInterface &cgvInterface::getInstance() {
    if (!instance) {
        instance = new cgvInterface;
    }

    return *instance;
}

/**
 * Create a new empty world with a camera
 */
void cgvInterface::create_world(void) {
    camera = cgvCamera(cgvPoint3D(6.0, 4.0, 8), cgvPoint3D(0, 0, 0), cgvPoint3D(0, 1.0, 0));
    camera.setParallelParameters(1 * 5, 1 * 5, 0.1, 200);
}

/**
 * Initialize the required parameters to create a window
 * @param argc Parameter from the main function of the program to know the number of input parameters from the command line
 * @param argv Parámeter from the command line to run the whole program
 * @param _width_window Initial width of the window
 * @param _height_window Initial height of the window
 * @param _pos_X Initial position of the window (X coordinate)
 * @param _pos_Y Initial position of the window (Y coordinate)
 * @param _title Title of the window
 * @pre It is assumed that the parameters have valid values
 * @post Change the height and weight of the window stored in the object
 */
void cgvInterface::configure_environment(int argc, char **argv,
                                         int _width_window, int _height_window,
                                         int _pos_X, int _pos_Y,
                                         string _title) {
    // initialization of the interface variables
    width_window = _width_window;
    height_window = _height_window;

    // initialization of the display window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_width_window, _height_window);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_title.c_str());

    glEnable(GL_DEPTH_TEST); // enable the removal of hidden surfaces by using the z-buffer
    glClearColor(1.0, 1.0, 1.0, 0.0); // define the background color of the window


    glEnable(GL_LIGHTING); // enable the lighting of the scene
    glEnable(GL_NORMALIZE); // normalize the normal vectors required by the lighting computation.

    create_world(); // create the world (scene) to be rendered in the window
}

/**
 * Infinite loop to render the scene and wait for new events in the interface
 */
void cgvInterface::init_rendering_loop() {
    glutMainLoop(); // initialize the visualization loop of OpenGL
}

/**
 * Method to control the keyboard events
 * @param key Pressed key code
 * @param x Mouse position (x coordinate) when the key was pressed
 * @param y Mouse position (y coordinate) when the key was pressed
 * @pre It is assumed that the parameters have valid values
 * @post The attribute that indicate whether the axes are rendered or not can be updated.
 */
void cgvInterface::set_glutKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': // enable/disable the visualization of the axes
            cgvInterface::getInstance().scene.set_axes(cgvInterface::getInstance().scene.get_axes() ? false : true);

            break;
        case 27: // Escape key to exit
            exit(1);
            break;
    }
    glutPostRedisplay(); // renew the content of the window
}

/**
 * Method that is called when the window changes its size. It defines the camera, the projection and the viewport
 * @param w Width of the window
 * @param h Height of the window
 * @pre It is assumed that the parameters have valid values
 */
void cgvInterface::set_glutReshapeFunc(int w, int h) {
    // dimension of the viewport with a new width and a new height of the display window


    // store the new values of the viewport and the display window.
    cgvInterface::getInstance().set_width_window(w);
    cgvInterface::getInstance().set_height_window(h);

    // Set up the kind of projection to be used
    cgvInterface::getInstance().camera.apply();
}

/**
 * Method to render the scene
 */
void cgvInterface::set_glutDisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the z-buffer

    // set up the viewport
    glViewport(0, 0, cgvInterface::getInstance().get_width_window(), cgvInterface::getInstance().get_height_window());

    // Section A: check the mode before applying the camera and projection transformations,
    if (cgvInterface::getInstance().mode == CGV_SELECT) {
        cgvInterface::getInstance().init_selection();
    }
    // Apply the camera and projection transformations according to its parameters and to the mode (selection or visualization)
    cgvInterface::getInstance().camera.apply();

    // Render the scene
    cgvInterface::getInstance().scene.render(cgvInterface::getInstance().mode);

    if (cgvInterface::getInstance().mode == CGV_SELECT) {
        cgvInterface::getInstance().finish_selection();
        glutPostRedisplay();
    } else {
        // refresh the window
        glutSwapBuffers(); // it is used instead of glFlush(), to avoid flickering
    }
}


/**
 * Mouse buttom detection function
 * @param button The button parameter is one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
 * @param state The state parameter is either GLUT_UP or GLUT_DOWN
 * @param x X position of the mouse at the time the buttom is pressed or released
 * @param y Y position of the mouse at the time the buttom is pressed or released
 */
void cgvInterface::set_glutMouseFunc(GLint button, GLint state, GLint x, GLint y) {
    // TODO: Section A: check if the left button of the mouse has been clicked
    // Review the documentation of glutMouseFunc in
    // Check if the state is GLUT_DOWN or not
    // If the state is GLUT_DOWN then, change to select mode, store the position of the mouse and modify the state of pressed_button
    // if the state is not GLUT_DOWN change to display mode and modify the state of pressed_button accordingly.
    if (button == GLUT_LEFT_BUTTON) {
        getInstance().cursorX = x;
        getInstance().cursorY = y;
        getInstance().pressed_button = state;

        if (state == GLUT_DOWN) {
            getInstance().mode = CGV_SELECT; // Enable selection mode
        } else {
            getInstance().mode = CGV_DISPLAY; // Return to display mode
        }
        glutPostRedisplay(); // Trigger a redraw
    }
}

/**
 * Mouse buttom movement detection method
 * @param x X position of the mouse
 * @param y Y position of the mouse
 */
void cgvInterface::set_glutMotionFunc(GLint x, GLint y) {
    if (getInstance().pressed_button == GLUT_DOWN) {
        if (getInstance().scene.return_isAnyBoxSelected()) {
            GLint deltaX = x - getInstance().cursorX; // Change in x position
            GLint deltaY = y - getInstance().cursorY; // Change in y position

            getInstance().scene.updateRotation(deltaX, deltaY); // Example: Updating rotation

            getInstance().cursorX = x;
            getInstance().cursorY = y;
            glutPostRedisplay();
        }
    }
}


/**
 * Method to indicate the callback functions
 */
void cgvInterface::init_callbacks() {
    glutKeyboardFunc(set_glutKeyboardFunc);
    glutReshapeFunc(set_glutReshapeFunc);
    glutDisplayFunc(set_glutDisplayFunc);

    glutMouseFunc(set_glutMouseFunc);
    glutMotionFunc(set_glutMotionFunc);
}


/**
 * Function to do the required operations when the selection begins
 */
void cgvInterface::init_selection() {
    // Section A: Disable lighting.
    glDisable(GL_LIGHTING);
}

/**
 * Function to do the required operations when the selection ends
 */
void cgvInterface::finish_selection() {
    glReadBuffer(GL_BACK);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);


    GLubyte pixels[3] = {0, 0, 0};

    // TODO: Section A. Use the function glReadPixels to read the value of the pixel in the position of the mouse
    // TODO: Section A. Once the color below the mouse is stored, then look for the corresponding box and select it.
    // Use the function assignSelection from Scene

    glReadPixels(getInstance().cursorX, getInstance().height_window - getInstance().cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    getInstance().scene.assignSelection(pixels);

    glutPostRedisplay();


    glEnable(GL_LIGHTING);
}
