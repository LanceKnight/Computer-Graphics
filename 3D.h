#ifdef __cplusplus
extern "C" {
#endif

#ifndef ThreeD_H
#define ThreeD_H

#define ROW 4
#define COL 4


typedef struct                  /* structure definitions */
{
   float  mat[ROW][COL];
}  matrix_unit;

typedef struct
{
   float i;
   float j;
   float k;
} Vector;

typedef struct
{
   float x;
   float y;
   float z;
   float r;
   float g;
   float b;
} Vertex_unit;

typedef struct
{
  float mat41[ROW];
} matrix41;

extern const matrix_unit I;

//extern matrix_unit ini;

extern matrix_unit *stack[50];    /* array of pointers to act as a stack */

extern float Near, Far;

extern int width, height;

extern int top;                   /* points to top of the stack */

extern int perspflag;

extern matrix_unit orth;       /* global ortho and perspective matrices */
                               /* to be used in Vertex3f */
extern matrix_unit perspect;

void draw_line(float, float, float, float);
int near_far_clip(float, float, float *, float *, float *, float *,
                  float *, float *);

void gtLookAt( float fx, float fy, float fz, float atx, float aty,
	       float atz, float upx, float upy, float upz);
void gtVertex3f(float x, float y, float z);


void Cross(Vector *x, Vector *y, Vector *z);
void Unitvec(float x, float y, float z, Vector *vec);
int Mult_end(matrix_unit *M, matrix41 *V, matrix41 *result);
int Mult_mat(matrix_unit *left, matrix_unit *right, matrix_unit *result);
int Copy_mat(const matrix_unit *from, matrix_unit *to);


#endif

#ifdef __cplusplus
}
#endif




