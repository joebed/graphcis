#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vecmath.h>
#include <vector>
using namespace std;

// Globals
unsigned short color_counter;
bool rotate = false;
float theta = 0.0f;
float speed = 1.0f;
// Light position
GLfloat Lt0pos[] = {1.0f, 1.0f, 5.0f, 1.0f};

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned>> vecf;

// You will need more global variables to implement color and position changes

// These are convenience functions which allow us to call OpenGL
// methods on Vec3d objects
// inline void glVertex(const Vector3f &a) { glVertex3fv(a); }

// inline void glNormal(const Vector3f &a) { glNormal3fv(a); }

template <typename T>
void print_vec(vector<T> vec) {
    cout << "(";
    for (auto val : vec) cout << val << ", ";
    cout << ")";
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        color_counter = (color_counter + 1) % 4;
        break;
    case 'r':
        rotate = !rotate;
        if (rotate) cout << "I'm going to be sick XI\n";
        break;
    case 'u':
        if (speed < 100) speed *= 2;
        else if (rotate) cout << "HURFHSHSHRHRH\n";
        break;
    case 'i':
        if (speed > .25) speed /= 2;
        else if (rotate) cout << "Now this is more like it :)\n";
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;
    }

    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        // add code to change light position
        Lt0pos[1] += 0.5f;
        break;
    case GLUT_KEY_DOWN:
        // add code to change light position
        Lt0pos[1] -= 0.5f;
        break;
    case GLUT_KEY_LEFT:
        // add code to change light position
        Lt0pos[0] -= 0.5f;
        break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
        Lt0pos[0] += 0.5f;
        break;
    }

    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void) {
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
    glLoadIdentity();           // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

    // Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = {{0.5, 0.5, 0.9, 1.0},
                                {0.9, 0.5, 0.5, 1.0},
                                {0.5, 0.9, 0.3, 1.0},
                                {0.3, 0.8, 0.9, 1.0}};

    // Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[color_counter]);

    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
    if (rotate) {
        theta += speed;
        if (theta > 360) theta -= 360;
    }
    glRotatef(theta, 0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    for (const auto& face : vecf) {
        glNormal3d(vecn[face[1]][0], vecn[face[1]][1], vecn[face[1]][2]);
        glVertex3d(vecv[face[0]][0], vecv[face[0]][1], vecv[face[0]][2]);
        glNormal3d(vecn[face[3]][0], vecn[face[3]][1], vecn[face[3]][2]);
        glVertex3d(vecv[face[2]][0], vecv[face[2]][1], vecv[face[2]][2]);
        glNormal3d(vecn[face[5]][0], vecn[face[5]][1], vecn[face[5]][2]);
        glVertex3d(vecv[face[4]][0], vecv[face[4]][1], vecv[face[4]][2]);
    }
    glEnd();

    // Dump the image to the screen.
    glutSwapBuffers();
}

void rotateFunc(int state) {
    if (rotate) {
        drawScene();
    }
    glutTimerFunc(1, rotateFunc, 1);
}

// Initialize OpenGL's rendering modes
void initRendering() {
    glEnable(GL_DEPTH_TEST); // Depth testing must be turned on
    glEnable(GL_LIGHTING);   // Enable lighting calculations
    glEnable(GL_LIGHT0);     // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h) {
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

Vector3f get_vec3f_from_ss(stringstream &ss) {
    string point1, point2, point3;
    ss >> point1 >> point2 >> point3;
    return Vector3f(stof(point1), stof(point2), stof(point3));
}

vector<unsigned int> get_indices_from_ss(stringstream &ss) {
    vector<unsigned int> res;
    res.reserve(6);
    string word, num;
    for (int i = 0; i < 3; ++i) {
        ss >> word;
        stringstream line(word);
        getline(line, num, '/');
        res.push_back(stoul(num) - 1);
        getline(line, num, '/');
        line >> num;
        res.push_back(stoul(num) - 1);
    }
    return res;
}


void loadInput() {
    // load the OBJ file here
    string line;
    string word;
    while (getline(cin, line)) {
        stringstream ss(line);
        ss >> word;
        if (word == "v") {
            vecv.push_back(get_vec3f_from_ss(ss));
        } else if (word == "vn") {
            vecn.push_back(move(get_vec3f_from_ss(ss)));
        } else if (word == "f") {
            vecf.push_back(get_indices_from_ss(ss));
        }
    }
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char **argv) {
    cout << "Press c to change color\n";
    cout << "Press arrow keys to move light position\n";
    cout << "Press r to toggle rotation\n";
    cout << "Press u and i to speed up/slow down rotation\n";
    loadInput();

    glutInit(&argc, argv);

    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(360, 360);
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up the callback function for resizing windows
    glutReshapeFunc(reshapeFunc);

    // Call back
    glutTimerFunc(1, rotateFunc, 1);

    // Call this whenever window needs redrawing
    glutDisplayFunc(drawScene);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0; // This line is never reached.
}
