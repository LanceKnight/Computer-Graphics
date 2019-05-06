#ifndef MAIN_HH
#define MAIN_HH

#include <string>
#include <vector>
#include <GL/glut.h>

/*	Create checkerboard image	*/
#define	checkImageWidth 1024
#define	checkImageHeight 1024
//static GLubyte checkImage[checkImageHeight][checkImageWidth][3];
extern GLubyte checkImage[checkImageHeight][checkImageWidth][3];
void makeCheckImage(void);
void init(void);
extern void display(void);
void reshape(int w, int h);
void motion(int x, int y);
void mouse(int button, int press, int x, int y);
void read_next_command(unsigned char key, int x, int y);

void main_loop(char line[]);

#endif
