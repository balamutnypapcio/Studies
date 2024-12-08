#include "cgvBox.h"



/**
 * Parametrized constructor
 * @param _r Red component [0, 255]
 * @param _g Red component [0, 255]
 * @param _b Red component [0, 255] 
 * @pre It is assumed that the parameters are valid
 * @post Create a new instance of the box with an RGB color as identifier
 */
cgvBox::cgvBox(GLubyte _r, GLubyte _g, GLubyte _b) {
	color_as_ID[0] = _r; 
	color_as_ID[1] = _g; 
	color_as_ID[2] = _b; 	
}

/**
 * Parametrized constructor
 * @param _color_as_ID It is a vector with the three component of a color Valid range [0, 255] per component
 * @pre It is assumed that the parameters are valid
 * @post Create a new instance of the box with an RGB color as identifier
 */
cgvBox::cgvBox(GLubyte _color_as_ID[3]) {
	for (int i = 0; i < 3; ++i) {
		color_as_ID[i] = _color_as_ID[i];
	}
}

/**
 * Method to render the box
 * @param mode It can be CGV_DISPLAY (normal rendering) or CGV_SELECT and render with the color_as_ID to use the color buffer technique
 * @pre It is assumed that the parameters are valid
 * @post If mode=CGV_DISPLAY->(normal rendering) if CGV_SELECT render with the color_as_ID to use the color buffer technique
 */
void cgvBox::render(RenderMode mode) {

	GLfloat color_piece[] = { 0,0.25,0 };
	GLfloat color_piece_top[] = { 0,0.3,0 };

	GLfloat selected_color[] = { 1,1,0,1.0 };


	// TODO: Section A. Add the required code to render in selection mode. Use glColor instead of glMaterial.
	// TODO: Section A. Add the required code to render the selected box as yellow (selected_color).


	if (mode == CGV_SELECT) {
		// Render with the unique color ID for selection
		glColor3ubv(color_as_ID);
	} else {
		// Normal rendering
		if (selected) {
			glMaterialfv(GL_FRONT, GL_EMISSION, selected_color);
			glColor4f(selected_color[0],selected_color[1],selected_color[2],selected_color[3]); // Yellow color for selected box
		} else {
			glMaterialfv(GL_FRONT, GL_EMISSION, color_piece);
			glColor3f(color_piece[0],color_piece[1],color_piece[2]); // Green for unselected box
		}
	}

	glPushMatrix();

	glScalef(1.1, 1, 2);
	glutSolidCube(1);

	glPopMatrix();

	//glMaterialfv(GL_FRONT, GL_EMISSION, color_piece_top);

	if (mode == CGV_SELECT) {
		// Render with the unique color ID for selection
		glColor3ubv(color_as_ID);
	} else {
		// Normal rendering
		if (selected) {
			glMaterialfv(GL_FRONT, GL_EMISSION, selected_color);
			glColor4f(selected_color[0],selected_color[1],selected_color[2],selected_color[3]); // Yellow color for selected box
		} else {
			glMaterialfv(GL_FRONT, GL_EMISSION, color_piece_top);
			glColor3f(color_piece_top[0],color_piece_top[1],color_piece_top[2]); // Green for unselected box
		}
	}

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glScalef(1.15, 0.2, 2.05);

	glutSolidCube(1);

	glPopMatrix();

}

/**
 * Method to identify the box as selected
 * @param c RGB color with three components
 * @pre It is assumed that the parameters are valid
 * @post The instance is marked as selected if c==color_as_ID
 */
void cgvBox::select(GLubyte c[3]) {
	selected = (color_as_ID[0] == c[0] && color_as_ID[1] == c[1] && color_as_ID[2] == c[2]);
}

