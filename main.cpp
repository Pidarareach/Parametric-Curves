#include <GL/glut.h>   // GLUT lib
#include <stdlib.h>    
#include <math.h>     
#include <iostream>
using namespace std;    

// Struct to define a point (x, y) coordinates
struct screenPt {
    GLint x;
    GLint y;
};

// Enum to label the different curve types
typedef enum { limacon = 1, cardioid, threeLeaf, fourLeaf, spiral } curveName;

GLsizei winWidth = 600, winHeight = 500; // Initial display window size

// Initialization function for OpenGL
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set background color to white
    glMatrixMode(GL_PROJECTION);     // Set projection parameters
    gluOrtho2D(0.0, 200.0, 0.0, 150.0); // Define a 2D orthographic projection
}

// Function to draw a line between two points
void lineSegment(screenPt pt1, screenPt pt2) {
    glBegin(GL_LINES);
    glVertex2i(pt1.x, pt1.y);
    glVertex2i(pt2.x, pt2.y);
    glEnd();
}

// Function to draw the selected curve
void drawCurve(GLint curveNum) {
    /* The limacon of Pascal is a modification of the circle equation
    * with the radius varying as r = a * cos (theta) + b, where a
    * and b are constants. A cardioid is a limacon with a = b.
    * Three-leaf and four-leaf curves are generated when
    * r = a * cos (n * theta), with n = 3 and n = 2, respectively.
    * A spiral is displayed when r is a multiple of theta.
    */
    const GLdouble twoPi = 6.283185;       // 2pi for full circle
    const GLint a = 175, b = 60;           // Constants for curve equations
    GLfloat r, theta, dtheta = 1.0 / float(a); // Step size for theta
    GLint x0 = 200, y0 = 250;              // Set an initial screen position
    screenPt curvePt[2];                   // Store two points for drawing lines

    glColor3f(0.0, 0.0, 0.0);             // Set curve color to black
    curvePt[0].x = x0;                    // Initialize curve position
    curvePt[0].y = y0;

    // Adjust initial x-offset based on the curve type
    switch (curveNum) {
    case limacon:   curvePt[0].x += a + b; break;
    case cardioid:  curvePt[0].x += a + a; break;
    case threeLeaf: curvePt[0].x += a; break;
    case fourLeaf:  curvePt[0].x += a; break;
    case spiral:    break;
    default: break;
    }

    theta = dtheta;

    // Loop to calculate and draw curve by connecting points
    while (theta < twoPi) {
        switch (curveNum) {
        case limacon:
            r = a * cos(theta) + b; break;
        case cardioid:
            r = a * (1 + cos(theta)); break;
        case threeLeaf:
            r = a * cos(3 * theta); break;
        case fourLeaf:
            r = a * cos(2 * theta); break;
        case spiral:
            r = (a / 4.0) * theta; break;
        default: break;
        }
        // Convert Polar to Cartesian coordinates
        curvePt[1].x = x0 + r * cos(theta);
        curvePt[1].y = y0 + r * sin(theta);
        lineSegment(curvePt[0], curvePt[1]); // Draw line segment
        curvePt[0] = curvePt[1]; // Move to next point
        theta += dtheta; // Increment theta
    }
}

// Display callback
void displayFcn(void) {
    GLint curveNum;
    glClear(GL_COLOR_BUFFER_BIT); // Clear display window

    // Prompt user for curve choice
    cout << "\nEnter the integer value corresponding to\n";
    cout << "one of the following curve names.\n";
    cout << "Press any other key to exit.\n";
    cout << "\n1-limacon, 2-cardioid, 3-threeLeaf, 4-fourLeaf, 5-spiral: ";
    cin >> curveNum;

    // Draw the selected curve, else exit
    if (curveNum >= 1 && curveNum <= 5)
        drawCurve(curveNum);
    else
        exit(0);

    glFlush();
}

// Callback for window resize
void winReshapeFcn(GLint newWidth, GLint newHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);
    glClear(GL_COLOR_BUFFER_BIT); // Clear display window
}

// Main function
void main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer and RGB color
    glutInitWindowPosition(100, 100); // Initialize window position
    glutInitWindowSize(winWidth, winHeight); // Initialize window size
    glutCreateWindow("Draw Curves"); // Create window with title
    init(); // Set up OpenGL state
    glutDisplayFunc(displayFcn); // Register display callback
    glutReshapeFunc(winReshapeFcn); // Register window resize callback
    glutMainLoop(); // Enter loop
}
