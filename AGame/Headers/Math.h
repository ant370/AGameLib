#ifndef ANT_MATH_H
#define ANT_MATH_H

#include <stdio.h>
#include "Types.h"


////Vector Functions
Vec4 Vec4Add(Vec4 a, Vec4 b);
Vec4 Vec4Cross(Vec4 a, Vec4 b);
Vec4 Vec4Sub(Vec4 a, Vec4 b);
Vec4 Vec4Normal(Vec4 a);
VecFloat Vec4Dot(const Vec4 a, const Vec4 b); 
VecFloat VecLenSq(Vec4 a);  
Matrix MatrixId();

#define MACRO_FUNCTION_ROW_i(i) a.a_##i##_0, a.a_##i##_1, a.a_##i##_2, a.a_##i##_3
#define MACRO_FUNCTION_COL_i(i) a.a_0_##i , a.a_1_##i , a.a_2_##i , a.a_3_##i  
#define MACRO_FUNCTION_ROW_GET(i) inline Vec4 MatrixRow##i(Matrix a) { Vec4 t = { MACRO_FUNCTION_ROW_i(i) }; return t; }
#define MACRO_FUNCTION_COL_GET(i) inline Vec4 MatrixCol##i(Matrix a) { Vec4 t = { MACRO_FUNCTION_COL_i(i) }; return t; }

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
inline Matrix Matrix_##i##j##_set(const Matrix m, const VecFloat f)		\
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

Vec4 MatrixRow(Matrix m, int i);

VecFloat MatrixGetij(Matrix m, int i, int j);

//Matrix Creation
Matrix MatrixTranslation(float x, float y, float z);
Matrix MatrixScale(float x, float y, float z);
Matrix MatrixRotation(Vec4 axis, float angle);
Matrix MatrixLookAt(Vec4 lookat, Vec4 from, Vec4 up);
Matrix MatrixProj(float near, float far, float aspect, float fov);
Matrix MatrixOrtho(float l, float r, float t, float b, float n, float f);

//Matrix Arithmatic
Matrix MatrixMul(Matrix a, Matrix b);
Matrix MatrixMulScalar(Matrix a, float b);
Matrix MatrixAdd(Matrix a, Matrix b);

// Matrix Inverse
const int MathPermutations4[24 * 5];  
int MathPerumtationParity(int xs[], int n);
float MathDeterminant(const Matrix m);
float MatrixTrace(const Matrix m); 
Matrix MathMatrixInv(Matrix a); 
//Helper Functions
void MatrixStringFormated(Matrix m);
 
#endif