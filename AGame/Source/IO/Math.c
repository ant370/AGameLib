#ifndef ANT_MATH_H
#define ANT_MATH_H
 
#include "Math.h"


typedef float VecFloat;

typedef const struct Vec4
{
	const VecFloat x;
	const VecFloat y;
	const VecFloat z;
	const VecFloat w;

} Vec4; 
typedef struct Matrix 
{
	VecFloat a_0_0;
	VecFloat a_0_1;
	VecFloat a_0_2;
	VecFloat a_0_3;
	VecFloat a_1_0;
	VecFloat a_1_1;
	VecFloat a_1_2;
	VecFloat a_1_3;
	VecFloat a_2_0;
	VecFloat a_2_1;
	VecFloat a_2_2;
	VecFloat a_2_3;
	VecFloat a_3_0;
	VecFloat a_3_1;
	VecFloat a_3_2;
	VecFloat a_3_3;

} Matrix;
 
 
VecFloat Vec4Dot(const Vec4 a, const Vec4 b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z + a.w * b.w);
}

VecFloat VecLenSq(Vec4 a)
{
	return (a.x *a.x + a.y*a.y + a.z*a.z);
}
  

Matrix MatrixId()
{ 
	Matrix m = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
	return m;
}

#define MACRO_FUNCTION_ROW_i(i) a.a_##i##_0, a.a_##i##_1, a.a_##i##_2, a.a_##i##_3
#define MACRO_FUNCTION_COL_i(i) a.a_0_##i , a.a_1_##i , a.a_2_##i , a.a_3_##i 

#define MACRO_FUNCTION_ROW_GET(i) Vec4 MatrixRow##i(Matrix a) { Vec4 t = { MACRO_FUNCTION_ROW_i(i) }; return t; }
#define MACRO_FUNCTION_COL_GET(i) Vec4 MatrixCol##i(Matrix a) { Vec4 t = { MACRO_FUNCTION_COL_i(i) }; return t; }
 
MACRO_FUNCTION_COL_GET(0)
MACRO_FUNCTION_COL_GET(1)
MACRO_FUNCTION_COL_GET(2)
MACRO_FUNCTION_COL_GET(3)
MACRO_FUNCTION_ROW_GET(0)
MACRO_FUNCTION_ROW_GET(1)
MACRO_FUNCTION_ROW_GET(2)
MACRO_FUNCTION_ROW_GET(3)


#define MACRO_Matrix_Set_i_j(i,j) .a_##i##_##j = m.a_##i##_##j
#define MACRO_Matrix_Set_i_j_val(i,j,val) .a_##i##_##j = val
#define MACRO_Matric_Set(i,j,val)		\
	MACRO_Matrix_Set_i_j(0,0)	,	\
	MACRO_Matrix_Set_i_j(0,1)	,	\
	MACRO_Matrix_Set_i_j(0,2)	,	\
	MACRO_Matrix_Set_i_j(0,3)	,	\
	MACRO_Matrix_Set_i_j(1,0)	,	\
	MACRO_Matrix_Set_i_j(1,1)	,	\
	MACRO_Matrix_Set_i_j(1,2)	,	\
	MACRO_Matrix_Set_i_j(1,3)	,	\
	MACRO_Matrix_Set_i_j(2,0)	,	\
	MACRO_Matrix_Set_i_j(2,1)	,	\
	MACRO_Matrix_Set_i_j(2,2)	,	\
	MACRO_Matrix_Set_i_j(2,3)	,	\
	MACRO_Matrix_Set_i_j(3,0)	,	\
	MACRO_Matrix_Set_i_j(3,1)	,	\
	MACRO_Matrix_Set_i_j(3,2)	,	\
	MACRO_Matrix_Set_i_j(3,3)		\
 												\


#define MACRO_FUNCTION_Matrix_Set(i,j)							\
Matrix Matrix_##i##j##_set(const Matrix m, const VecFloat f)		\
{																	\
	Matrix n = {													\
			MACRO_Matric_Set(i,j, f) ,							\
			.a_##i##_##j = f										\
	};																\
	return n;														\
}																	\
 
MACRO_FUNCTION_Matrix_Set(0, 0) 
MACRO_FUNCTION_Matrix_Set(0, 1) 
MACRO_FUNCTION_Matrix_Set(0, 2) 
MACRO_FUNCTION_Matrix_Set(0, 3) 
MACRO_FUNCTION_Matrix_Set(1, 0) 
MACRO_FUNCTION_Matrix_Set(1, 1) 
MACRO_FUNCTION_Matrix_Set(1, 2) 
MACRO_FUNCTION_Matrix_Set(1, 3) 
MACRO_FUNCTION_Matrix_Set(2, 0) 
MACRO_FUNCTION_Matrix_Set(2, 1) 
MACRO_FUNCTION_Matrix_Set(2, 2) 
MACRO_FUNCTION_Matrix_Set(2, 3) 
MACRO_FUNCTION_Matrix_Set(3, 0) 
MACRO_FUNCTION_Matrix_Set(3, 1) 
MACRO_FUNCTION_Matrix_Set(3, 2) 
MACRO_FUNCTION_Matrix_Set(3, 3)		

Vec4 MatrixRow(Matrix m, int i)
{
	switch (i)
	{
		case 0: return MatrixRow0(m); break;
		case 1: return  MatrixRow1(m); break;
		case 2: return MatrixRow2(m); break;
		case 3: return MatrixRow3(m); break;
	}
}

VecFloat MatrixGetij(Matrix m, int i, int j)
{
	switch (j)
	{
		case 0: return MatrixRow(m, i).x; break;
		case 1:	return MatrixRow(m, i).y; break;
		case 2:	return MatrixRow(m, i).z; break;
		case 3:	return MatrixRow(m, i).w; break;
	}
}

#define MACRO_MATRIX_MUL(i,j) Vec4Dot( MatrixRow##i(a) , MatrixCol##j(b) )

Matrix MatrixMul(Matrix a, Matrix b)
{
	Matrix out = {
	  MACRO_MATRIX_MUL(0, 0) ,  MACRO_MATRIX_MUL(0, 1) ,  MACRO_MATRIX_MUL(0, 2) ,  MACRO_MATRIX_MUL(0, 3) ,
	  MACRO_MATRIX_MUL(1, 0) ,  MACRO_MATRIX_MUL(1, 1) ,  MACRO_MATRIX_MUL(1, 2) ,  MACRO_MATRIX_MUL(1, 3) ,
	  MACRO_MATRIX_MUL(2, 0) ,  MACRO_MATRIX_MUL(2, 1) ,  MACRO_MATRIX_MUL(2, 2) ,  MACRO_MATRIX_MUL(2, 3) ,
	  MACRO_MATRIX_MUL(3, 0) ,  MACRO_MATRIX_MUL(3, 1) ,  MACRO_MATRIX_MUL(3, 2) ,  MACRO_MATRIX_MUL(3, 3) 
	};

	return out;
}


Vec4 Vec4Add(Vec4 a, Vec4 b)
{
	Vec4 c = { a.x + b.x , a.y + b.y, a.z + b.z, a.w + b.w };
	return c;
}

Vec4 Vec4Cross(Vec4 a, Vec4 b)
{
	Vec4 c = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
	return c;
}


Vec4 Vec4Sub(Vec4 a, Vec4 b)
{
	Vec4 c = {
		a.x - b.x , a.y - b.y , a.z - b.z, a.w - b.w
	};
	return c;
}

Vec4 Vec4Normal(Vec4 a)
{
	float sum = sqrt(a.x*a.x + a.y*a.y + a.z*a.z); 

	Vec4 c = {
		a.x / sum , a.y / sum, a.z /sum , a.w
	};
	return c;
}

Matrix MatrixProj(float near, float far, float aspect, float fov)
{

	float top = near * tan ( (3.142f/ 180.0f) * fov * 0.5f); 
	float bottom = - top;
	float right = top * aspect;
	float left = -right;
	Matrix m = { 
		(2.0f * near) / (right - left) , 0.0f, (right + left)/(right - left), 0.0f,
		0.0f, (2.0f * near) / (top -bottom), (top + bottom)/(top -bottom), 0.0f,
		0.0f, 0.0f, - (far + near)/(far - near) , - (2.0f) * (far * near) / (far - near),
		0.0f, 0.0f, -1.0f, 0.0f
	}; 
	return m;
}

Matrix MatrixLookAt(Vec4 lookat, Vec4 eye, Vec4 up)
{ 
	Vec4 zaxis = Vec4Normal(Vec4Sub(eye, lookat));
	Vec4 xaxis = Vec4Normal(Vec4Cross(zaxis, up));
	Vec4 yaxis = Vec4Normal(Vec4Cross(xaxis, zaxis));
 

	Matrix m = {
			xaxis.x, xaxis.y, xaxis.z, -(Vec4Dot(xaxis, eye)),
			yaxis.x, yaxis.y, yaxis.z, -(Vec4Dot(yaxis, eye)),
			zaxis.x,  zaxis.y,  zaxis.z, -(Vec4Dot(zaxis, eye)),
			0.0f, 0.0f, 0.0f, 1.0f 
	};

	return m; 
}

Matrix MatrixTranslation(float x, float y, float z)
{
	
	Matrix m = { 	1.0f, 0.0f, 0.0f, x,
					0.0f, 1.0, 	0.0f, y,
					0.0f, 0.0f, 1.0f, z,
					0.0f, 0.0f, 0.0f, 1.0f };
	return m;
}

//Matrix Creation
Matrix MatrixScale(float x, float y, float z)
{
	Matrix m = { x, 0.0f, 0.0f, 0.0f,
					0.0f, y, 0.0f, 0.0f,
					0.0f, 0.0f, z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
	return m;
}

Matrix MatrixMulScalar(Matrix a, float b)
{
	Matrix m = {
		b * a.a_0_0, b * a.a_0_1, b * a.a_0_2, b * a.a_0_3,
		b * a.a_1_0, b * a.a_1_1, b * a.a_1_2, b * a.a_1_3,
		b * a.a_2_0, b * a.a_2_1, b * a.a_2_2, b * a.a_2_3,
		b * a.a_3_0, b * a.a_3_1, b * a.a_3_2, b * a.a_3_3
	};
	return m;
}

void MatrixStringFormated(Matrix m)
{
	char* outputFormat = " | %10.4f %10.4f %10.4f %10.4f | \n | %10.4f %10.4f %10.4f %10.4f | \n | %10.4f %10.4f %10.4f %10.4f | \n | %10.4f %10.4f %10.4f %10.4f | \n";
	printf(outputFormat,
		m.a_0_0, m.a_0_1, m.a_0_2, m.a_0_3,
		m.a_1_0, m.a_1_1, m.a_1_2, m.a_1_3,
		m.a_2_0, m.a_2_1, m.a_2_2, m.a_2_3,
		m.a_3_0, m.a_3_1, m.a_3_2, m.a_3_3); 


}


Matrix MatrixAdd(Matrix a, Matrix b)
{
	Matrix m = {
		b.a_0_0 + a.a_0_0,
		b.a_0_1 + a.a_0_1,
		b.a_0_2 + a.a_0_2,
		b.a_0_3 + a.a_0_3,

		b.a_1_0 + a.a_1_0,
		b.a_1_1 + a.a_1_1,
		b.a_1_2 + a.a_1_2,
		b.a_1_3 + a.a_1_3,

		b.a_2_0 + a.a_2_0,
		b.a_2_1 + a.a_2_1,
		b.a_2_2 + a.a_2_2,
		b.a_2_3 + a.a_2_3,

		b.a_3_0 + a.a_3_0,
		b.a_3_1 + a.a_3_1,
		b.a_3_2 + a.a_3_2,
		b.a_3_3 + a.a_3_3,
	};
	return m;
}
 
///Fairly confident that this works
int MathPerumtationParity(int xs[], int n)
{ 
	int parity = 1;

	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (xs[i] > xs[j])
				parity = parity * (-1);
		}
	}

	return parity;
}

const int MathPermutations4[24 * 5] =
{ 1,2,3,4, 1,
2,1,3,4, -1,
3,1,2,4, 1,
1,3,2,4, -1,
2,3,1,4, 1,
3,2,1,4, -1,
3,2,4,1, 1,
2,3,4,1, -1,
4,3,2,1, 1,
3,4,2,1, -1,
2,4,3,1, 1,
4,2,3,1, -1,
4,1,3,2, 1,
1,4,3,2, -1,
3,4,1,2, 1,
4,3,1,2, -1,
1,3,4,2, 1,
3,1,4,2, -1,
2,1,4,3, 1,
1,2,4,3, -1,
4,2,1,3, 1,
2,4,1,3, -1,
1,4,2,3, 1,
4,1,2,3, -1 };

float MathDeterminant(const Matrix m)
{
	const int N = 24; // 4! due to 4x4 matrix
	float summation = 0.0f;

	for (int i = 0; i < N; i++)
	{
		float parity = MathPermutations4[5 * i + 4]; 

		int *perm = MathPermutations4 + (5 * i);  

		float term = parity * MatrixGetij(m, 0, perm[0]-1)
			* MatrixGetij(m, 1, perm[1]-1)
			* MatrixGetij(m, 2, perm[2]-1)
			* MatrixGetij(m, 3, perm[3]-1);

		summation += term;
	}

	return summation;
}

float MatrixTrace(const Matrix m)
{
	return (m.a_0_0 + m.a_1_1 + m.a_2_2 + m.a_3_3);
}

 
Matrix MathMatrixInv(Matrix a)
{
	Matrix A_squared = MatrixMul(a, a);
	Matrix A_cubed = MatrixMul(a, A_squared);

	float trace_A = MatrixTrace(a);
	float trace_A_sq = MatrixTrace(A_squared);
	float trace_A_cubed = MatrixTrace(A_cubed);

	float det_A = MathDeterminant(a);

	// ===========  https://en.wikipedia.org/wiki/Invertible_matrix#Cayley%E2%80%93Hamilton_method
	//////

	float I_term = (1.0f / 6.0f) * (trace_A * trace_A * trace_A - 3 * trace_A * trace_A_sq + 2 * trace_A_cubed);
	Matrix L = MatrixMulScalar(MatrixId(), I_term);

	float A_term = (-1.0f / 2.0f) * (trace_A*trace_A - trace_A_sq);
	Matrix L2 = MatrixMulScalar(a, A_term);

	Matrix L3 = MatrixMulScalar(A_squared, trace_A);
	Matrix L4 = MatrixMulScalar(A_cubed, -1.0f);

	Matrix sum = MatrixAdd(MatrixAdd(L, L2), MatrixAdd(L3, L4));
	Matrix result = MatrixMulScalar(sum, (1.0f / det_A));


	return result;
}


Matrix MatrixRotation(Vec4 axis, float angle)
{
	float sint = sin(angle);
	float cost = cos(angle);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	Matrix rotMax = {
		cost + x*x*(1.0f - cost), 		x*y*(1-cost) - z*sint, 		x*y*(1-cost) + y*sint,		0.0f,
		y*x* (1-cost) + z*sint,			cost + y*y*(1-cost),		y*z*(1-cost) - x*sint,		0.0f,
		z*x*(1-cost) - y*sint,			z*y*(1-cost) + x*sint,		cost + z*z*(1-cost),		0.0f,
		0.0f,							0.0f,						0.0f,						1.0f
	};

	return rotMax;
}

Matrix MatrixTranspose(Matrix a)
{ 
	Matrix m = {
		a.a_0_0, a.a_1_0, a.a_2_0, a.a_3_0,
		a.a_0_1, a.a_1_1, a.a_2_1, a.a_3_1,
		a.a_0_2, a.a_1_2, a.a_2_2, a.a_3_2,
		a.a_0_3, a.a_1_3, a.a_2_3, a.a_3_3,
	};
	return MatrixTranspose(m);
	return a;
}

Matrix MatrixOrtho(float l, float r, float t, float b, float n, float f)
{
	Matrix m = {
		2.0f / (r-l), 		0.0f,		0.0f,		-((r+l)/(r-l)),
		0.0f,				2.0f/(t-b),	 0.0f,		-((t+b)/(t-b)),
		0.0f,				0.0f,		-2.0f/(f-n),	-((f+n)/(f-n)),
		0.0f,				0.0f,		0.0f,			1.0f
	};
 
	return m;
	
}

#endif