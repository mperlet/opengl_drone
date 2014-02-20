#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

//import the required libraries (note: only works for visual studio)
//#pragma comment(lib, "glut32.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

// Winkel für Rotor-Blätter
float angle = 0;

// Tasten für dynamische Kamera Bewegung
GLfloat vert_key = 0.0f;
GLfloat hori_key = 0.0f;

// Geschwindigkeit für Rotor-Blätter und vertikaler Dronen-Bewegung
GLfloat speed = 1.0f;

// Dronen Bewegung - horizontal + vertikal
GLfloat drone_move_hori = 0.0f;
GLfloat drone_move_verti = 0.0f;

// Material Material-Tabelle, siehe README.md
void set_black_mat(){ 
    GLfloat mat_ambient[]={0, 0, 0, 1};
    GLfloat mat_diffuse[]={0.1, 0.1, 0.1, 1};
    GLfloat mat_specular[]={0.5, 0.5, 0.5, 1};
    GLfloat mat_shininess=32;

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

// Material Material-Tabelle, siehe README.md
void set_copper_mat() { // Kupfer-Material
    GLfloat mat_ambient[]={0.19125, 0.0735, 0.0225, 1};
    GLfloat mat_diffuse[]={0.7038, 0.27048, 0.0828, 1};
    GLfloat mat_specular[]={0.256777, 0.137622, 0.086014, 1};
    GLfloat mat_shininess=0.1;

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

// Material Material-Tabelle, siehe README.md
void set_red_mat() { // Kupfer-Material
    GLfloat mat_ambient[]={0.0, 0.0, 0.0, 1};
    GLfloat mat_diffuse[]={0.5, 0.0, 0.0, 1};
    GLfloat mat_specular[]={0.7, 0.6, 0.6, 1};
    GLfloat mat_shininess=0.25;

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

// quader Funktion aus Beispiel Quelltext
void quader(double l, double b, double t) {
	GLdouble f[8][3]={
			{-l/2,-b/2,-t/2},{ l/2,-b/2,-t/2},
			{ l/2, b/2,-t/2},{-l/2, b/2,-t/2},
			{-l/2,-b/2, t/2},{ l/2,-b/2, t/2},
			{ l/2, b/2, t/2},{-l/2, b/2, t/2}
	};
	GLint in[18]={0,4,1,5,
			    2,6,3,7,
			    0,3,4,7,5,6,1,2,0,3
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, f);
	glDrawElements(GL_QUAD_STRIP,4,GL_UNSIGNED_INT,&in[0]);
	glDrawElements(GL_QUAD_STRIP,4,GL_UNSIGNED_INT,&in[4]);
	glDrawElements(GL_QUAD_STRIP,10,GL_UNSIGNED_INT,&in[8]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// U-Quader Funktion für Aufgabe 2.3
void uquader(double l, double b, double t, double lb, double tb) {
	double laenge = l;
	double breite = b;
	double tiefe = t;
	double fussbreite = (laenge - lb) / 2;
	double sitzhoehe = (breite - tb);
	
	// Äußere Hülle des U-Quaders
	glBegin(GL_QUAD_STRIP);
        
		glVertex3f(0,0,0); // a'
		glVertex3f(0,0,tiefe); // a
		
		glVertex3f(fussbreite,0,0); // b'
		glVertex3f(fussbreite,0,tiefe); // b
		
		glVertex3f(fussbreite,sitzhoehe,0); // c' 
		glVertex3f(fussbreite,sitzhoehe,tiefe); // c
		
		glVertex3f(fussbreite + lb,sitzhoehe,0); // d'
		glVertex3f(fussbreite + lb,sitzhoehe,tiefe); // d
		
		glVertex3f(fussbreite + lb,0,0); // e'
		glVertex3f(fussbreite + lb,0,tiefe); // e 
		
		glVertex3f(laenge,0,0); // f'
		glVertex3f(laenge,0,tiefe); // f 
		
		glVertex3f(laenge,breite,0); // g'
		glVertex3f(laenge,breite,tiefe); // g
		
		glVertex3f(0,breite,0); // h'
		glVertex3f(0,breite,tiefe); // h 
		
		glVertex3f(0,0,0); // a'
		glVertex3f(0,0,tiefe); // a
				
	glEnd();
  
	// Vorderseite links
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0,0,tiefe); // a'
		glVertex3f(fussbreite,0,tiefe); // a
		
		glVertex3f(0,breite,tiefe); // a'
		glVertex3f(fussbreite,breite,tiefe); // a
	glEnd();
  
	// Vorderseite oben
	glBegin(GL_QUAD_STRIP);
		glVertex3f(fussbreite,sitzhoehe,tiefe); // a'
		glVertex3f(fussbreite,breite,tiefe); // a
		
		glVertex3f(laenge,sitzhoehe,tiefe); // a'
		glVertex3f(laenge,breite,tiefe); // a
	glEnd();

	// Vorderseite rechts
	glBegin(GL_QUAD_STRIP);
		glVertex3f(fussbreite + lb,sitzhoehe,tiefe); // d
		glVertex3f(laenge,sitzhoehe,tiefe); // d
		
		glVertex3f(fussbreite + lb,0,tiefe); // e 
		glVertex3f(laenge,0,tiefe); // f 
	glEnd();
  
	// Hinterseite links
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0,0,0); // a'
		glVertex3f(fussbreite,0,0); // a
		
		glVertex3f(0,breite,0); // a'
		glVertex3f(fussbreite,breite,0); // a
	glEnd();
	  
	// Hinterseite oben
	glBegin(GL_QUAD_STRIP);
		glVertex3f(fussbreite,sitzhoehe,0); // a'
		glVertex3f(fussbreite,breite,0); // a
		
		glVertex3f(laenge,sitzhoehe,0); // a'
		glVertex3f(laenge,breite,0); // a
	glEnd();

	// Hinterseite rechts
	glBegin(GL_QUAD_STRIP);
		glVertex3f(fussbreite + lb,sitzhoehe,0); // d
		glVertex3f(laenge,sitzhoehe,0); // d
		
		glVertex3f(fussbreite + lb,0,0); // e 
		glVertex3f(laenge,0,0); // f  
	glEnd();
}

// Beispiel 1 Materialien, kann genutzt oder veraendert werden
void set_mat1() {

	GLfloat mat_ambient[]={0.21, 0.13, 0.05, 1.0};
	GLfloat mat_diffuse[]={0.71, 0.43, 0.18, 1.0};
	GLfloat mat_specular[]={0.39, 0.27, 0.17, 1.0};
	GLfloat mat_shininess=25.6;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
}

// Beispiel 2 Materialien, kann genutzt oder veraendert werden
void set_mat2() {
	
    GLfloat mat_ambient[]={0.25, 0.21, 0.21, 0.9};
	GLfloat mat_diffuse[]={0.99, 0.83, 0.83, 0.9};
	GLfloat mat_specular[]={0.3, 0.3, 0.3, 0.9};
	GLfloat mat_shininess=11.3;

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

// Bank aus grossem Element und 3 kleine Sitzflächen
void set_bank() { 
	// Anschauungsunterricht von Konrad-Zuse-Haus
	glPushMatrix(); // jede Bank neu erstellen		 
		//glRotated(-90.0, 1.0, 0.0, 0.0); // um x-Achse drehen
		set_mat1(); // Matrialien setzen
		uquader(1.9, 0.45, 0.55, 0.9, 0.15); 
		//quader(1.9, 0.45, 0.55);
	glPopMatrix();
		
	glPushMatrix();
		glTranslated(0.95, 0.575, 0.1); // neue Koordinaten fuer Sitzflaeche 3 lange Quader
		set_mat2();
		quader(1.9, 0.05, 0.05);
		glTranslated(0.0, 0.0, 0.175);// nur relative Verschiebung
		quader(1.9, 0.05, 0.05);
		glTranslated(0.0, 0.0, 0.175);// nur relative Verschiebung
		quader(1.9, 0.05, 0.05);
	glPopMatrix();
}

// Modelliert den Rotor für die Drone
void set_rotor() {
	// Aus Torus, Kegel und Quadern Rotor in X-Y-Ebene erzeugen 
	
	// Rotor-Ring aus Kupfer
	glPushMatrix();
		set_copper_mat();		
		glTranslated(0.0,0.3,0); 
		glRotated(90, 1.0, 0.0, 0.0);
		glutSolidTorus(0.1,0.9,9, 20);
	glPopMatrix();	

	// Rotor-Achse aus rotem Plastik
	glPushMatrix();
		set_red_mat();
        glTranslated(0,0.3,0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		glutSolidCone(0.3, 0.3, 50, 50);
	glPopMatrix();
	
	glPushMatrix();
		set_red_mat();
        glTranslated(0,0.3,0.0);
		glRotated(90, 1.0, 0.0, 0.0);
		glutSolidCone(0.3, 0.3, 50, 50);
	glPopMatrix();	
 	
 	// Drehung der Rotor-Blätter
 	angle = glutGet(GLUT_ELAPSED_TIME)/1000.0f *15;
	glRotated(angle*10*speed, 0.0, 1.0, 0.0);
	
	// Rotor-Blätter
	glPushMatrix();
		set_black_mat();
		glRotated(0, 0.0, 1.0, 0.0);
		glTranslated(0.3,0.3,0);
		quader(0.6,0.1,0.2); 
	glPopMatrix();
	glPushMatrix();
		set_black_mat();
		glRotated(120, 0.0, 1.0, 0.0);  
		glTranslated(0.3,0.3,0);
		quader(0.6,0.1,0.2); 
	glPopMatrix();
	glPushMatrix();
		set_black_mat();
		glRotated(240, 0.0, 1.0, 0.0); 
		glTranslated(0.3,0.3,0);
		quader(0.6,0.1,0.2); 
	glPopMatrix();
}

// Setzt die Drone zusammen
void set_drone() {
	
	// Dronen Körper/Gerüst 
	glPushMatrix(); 
		glTranslated(0.0, 0.0, 0.0);
		quader(4, 0.10, 0.10);
		quader(0.10, 0.10, 4);
	glPopMatrix();
	
	// Setzt 4 Rotoren an die Spitzen des Gerüsts
	glPushMatrix();
		glTranslated(-2.0,0,0);
		set_rotor();
	glPopMatrix();

	glPushMatrix();
		glTranslated(2.0,0,0);
		set_rotor();
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0,0,-2.0);
		set_rotor();
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0,0,2.0);
		set_rotor();
	glPopMatrix();
}

// Setzt initiale Beleuchtung - aus Beispiel Quellcode
int initApplication() {
	GLfloat model_ambient[]={ 0.2, 0.2, 0.2, 1.0};
	GLfloat light_position[]={ 1.0, 1.0, 1.0, 0.0};
	GLfloat spot_direction[]={ 0.0, 0.0, -1.0, 1.0};
	GLfloat light_intensity[]={ 1.0, 1.0, 1.0, 1.0};
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, light_intensity);
	glLightfv(GL_LIGHT0,GL_SPECULAR, light_intensity);
	glLightfv(GL_LIGHT1,GL_DIFFUSE, light_intensity);
	glLightfv(GL_LIGHT1,GL_SPECULAR, light_intensity);
	glLightfv(GL_LIGHT1,GL_POSITION, light_position);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, spot_direction);
	glEnable(GL_LIGHT1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);

	glEnable(GL_AUTO_NORMAL);

	return 1;
}

// Funktion für die Änderung der Fenstergröße
// Wurde angepasst, sodass auch eine Vollbild-Darstellung möglich ist
void reshape(int breite, int hoehe) { 
	float abmessung = (float) breite / (float) hoehe; 

    glViewport(0, 0, breite, hoehe);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-abmessung, abmessung, -1.0, 1.0, 2.0, 100.0); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

// Tastatur Funktion für ASCII-Tasten
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		
		// Escape-Taste beendet das Programm
		case 27:
		exit(EXIT_SUCCESS);break;
		
		// 'a' Taste bewegt Drone nach links
		case 97: 					
			drone_move_hori+=0.3;
		break;
		
		// 'd' Taste bewegt Drone nach rechts
		case 100: 					
			drone_move_hori-=0.3;
		break;	
		
		// 'w' Taste bewegt Drone  nach oben. 
		case 119: 					
			drone_move_verti+=0.2*speed;
		break;	
		
		// 's' Taste bewegt Drone nach unten
		case 115: 					
			drone_move_verti-=0.2*speed;
		break;
		
		// 'Leertaste' setzt Geschwindigkeit
		// für vertikale Dronen Bewegung und Rotor-Rotation
		case 32	:
			speed += 1.5;
		break;
	}
	// Setzt Geschwindigkeit zurück, 3 Stufen Schalter
	if (speed > 4) speed = 1.0;
	glutPostRedisplay(); 
}

// Anzeige Funktion
void display() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Setzt Kamera, Variablen für dynamische Kamera-Bewegung
	gluLookAt(4.0+hori_key, 0.0+vert_key, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	 
	// Grundflaeche
	GLfloat ebene[4][3]={ 
		{-30.0f, 0.0f,-15.0f}, { 30.0f,0.0f,-15.0f},
		{ -30.0f, 0.0f,15.0f},{ 30.0f,0.0f, 15.0f}};
	
	// Sonnen Position
	GLfloat light_position1[]= { 0.0f, 20.0f, -20.0f, 1.0f}; 
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glLightfv(GL_LIGHT0,GL_POSITION, light_position1);
	glDisable(GL_LIGHTING);
	
	glPushMatrix();
		glTranslatef(0.0f, -3.0f, 0.0f); // nach unten verschoben
		glColor4f(0.0f, 0.8f, 0.8f, 1.0f);
		// Setzt die Ebene
		glBegin(GL_QUAD_STRIP);
			glVertex3fv(ebene[0]);
			glVertex3fv(ebene[1]);
			glVertex3fv(ebene[2]);
			glVertex3fv(ebene[3]);
		glEnd();
		
		glEnable(GL_LIGHTING);
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// Setzt den Teepot für Aufgabe 2.6
		glPushMatrix();
			set_black_mat(); 
			glTranslated(-2.50, 1.3, -5.0); 
			glutSolidTeapot(2.0);
		glPopMatrix();
		
		// 3 Bänke setzen
		glPushMatrix();// Bank ausgeben
			glRotated(432 % 360, 0.0, 1.0, 0.0);
			glTranslated(0, 0, 0); 
			set_bank(); 
		glPopMatrix();
		
		glPushMatrix();// Bank ausgeben
			glRotated(432 % 360, 0.0, 1.0, 0.0);
			glTranslated(2.5, 0, 0); 
			set_bank();
		glPopMatrix();
		
		glPushMatrix();// Bank ausgeben
			glRotated(432 % 360, 0.0, 1.0, 0.0);
			glTranslated(-2.5, 0, 0);
			set_bank();
		glPopMatrix();
				
		// Lampe setzen
		glPushMatrix(); // Lampe
			set_black_mat();
			glTranslated(4.0,0.0,3.5);
			quader(0.1, 8.0, 0.1);
		glPopMatrix();

		glPushMatrix(); // Lampenkopf
			set_copper_mat();
			glTranslated(3.50,4.0,3.5);
			quader(1.0, 0.2, 0.60);
		glPopMatrix();
			
		// Drohne setzen und bewegen
		glPushMatrix(); 
			glTranslated(-drone_move_hori,4.0+drone_move_verti,0);
			set_drone();
		glPopMatrix(); 

    glPopMatrix(); // Gesamtszene 

	glutSwapBuffers();
	glutPostRedisplay();
}

// Tastatur Funktion für Steuerungs-Tasten (Pfeiltasten) 
void processSpecialKeys(int key, int x, int y) {

	switch(key) {
		// Pfeil-Hoch Taste für Kamera-Bewegung
		case GLUT_KEY_UP :
			vert_key+=0.5;break;
		
		// Pfeil-Runter Taste für Kamera-Bewegung
		case GLUT_KEY_DOWN :
			vert_key-=0.5;break;
		
		// Pfeil-Links Taste für Kamera-Bewegung
		case GLUT_KEY_LEFT :
			hori_key-=0.5;break;
			
		// Pfeil-Links Taste für Kamera-Bewegung
		case GLUT_KEY_RIGHT :
			hori_key+=0.5;break;
	}
	// Setzt ein Limit für die Kamera-Bewegung (untere Grenze)
	if (vert_key <= -2.0) vert_key = -2.0; 
}


int main(int argc, char **argv) {
	
	//initialize glut and parse command line arguments @ see glut documentation for more details
	glutInit(&argc, argv);
	
	//setting up the window display mode
	glutInitDisplayMode(
	GLUT_RGBA |   //use a rgba target framebuffer
	GLUT_DEPTH |  //create a depth (z-)buffer
	GLUT_DOUBLE |  //create a front and back-buffer
	GLUT_MULTISAMPLE); //create a multi-sample framebuffer (use for fullscreen antialiasing)
	
	//setting up window pos and size
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 480);
	
	//create the window
	//from this point we can use the opengl api, because
	//glut has created a valid opengl context for us
	glutCreateWindow( "Hausaufgabe 2");
	
	//init the application and all required resources
	if (!initApplication())
	  return EXIT_FAILURE;
	
	//register the required callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);

	//show the window
	glutShowWindow();
	
	//enter mainloop which handle all window events
	glutMainLoop();
	return EXIT_SUCCESS;
}

