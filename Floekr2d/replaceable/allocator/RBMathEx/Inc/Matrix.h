#pragma once

#include "Axis.h"
#include "Vector3.h"
/*with:
 *	   Vector4.h
 *	   RBMathUtilities.h
 *			stdlib.h/
 *			RBMathBase.h/math.h
 */
//#include "WindowsPlatformTemp.h"
#include <iostream>

class RBMatrix
{
public:
	//UE4 use a union to surround the data
	//not sure the reason
	ALIGN(16) f32 m[4][4];

	ALIGN(16) static RB_API const RBMatrix identity;

	FORCEINLINE RBMatrix();

	FORCEINLINE RBMatrix(const RBVector3& tx,const RBVector3& ty,const RBVector3& tz,const RBVector3& tw);
	
	FORCEINLINE RBMatrix(const RBVector4& tx,const RBVector4& ty,const RBVector4& tz,const RBVector4& tw);

	FORCEINLINE RBMatrix(f32 n00,f32 n01,f32 n02,f32 n03,f32 n10,f32 n11,f32 n12,f32 n13,f32 n20,f32 n21,f32 n22,f32 n23,f32 n30,f32 n31,f32 n32,f32 n33);

	INLINE void set_identity();

	FORCEINLINE RBMatrix operator* (const RBMatrix& other) const;

	FORCEINLINE void operator*=(const RBMatrix& other);

	FORCEINLINE RBMatrix operator+(const RBMatrix& other) const;

	FORCEINLINE void operator+=(const RBMatrix& other);

	FORCEINLINE RBMatrix operator*(f32 a) const;

	FORCEINLINE void operator*=(f32 a);

	INLINE bool operator==(const RBMatrix& other) const;

	INLINE bool equals(const RBMatrix& other, f32 tolerance=SMALL_F) const;

	INLINE bool operator!=(const RBMatrix& other) const;

	//Homogeneous transform 齐次
	FORCEINLINE RBVector4 transform_vector4(const RBVector4& v) const;

	FORCEINLINE RBVector4 transform_position(const RBVector3 &V) const;

	FORCEINLINE RBVector3 inv_transform_position(const RBVector3 &V) const;

	FORCEINLINE RBVector4 transform_vector3(const RBVector3& V) const;

	FORCEINLINE RBVector3 inv_transform_vector3(const RBVector3 &V) const;

	// Transpose.转置

	FORCEINLINE RBMatrix get_transposed() const;

	// @return determinant of this matrix.返回

	INLINE f32 get_determinant() const;

	/** @return the determinant of rotation 3x3 matrix */
	INLINE f32 rotation_determinant() const;

	/** Fast path, doesn't check for nil matrices in final release builds */
	INLINE RBMatrix get_inverse() const;
	/** Fast path, and handles nil matrices. */
	INLINE RBMatrix get_inverse_safe() const;
	/** Slow and safe path */
	INLINE RBMatrix get_inverse_slow() const;

#ifdef _DEBUG
	/**output console**/
	INLINE void out() const
	{
		//std::cout<<std::endl;
		std::cout<<"|"<<m[0][0]<<","<<m[0][1]<<","<<m[0][2]<<","<<m[0][3]<<"|"<<std::endl;
		std::cout<<"|"<<m[1][0]<<","<<m[1][1]<<","<<m[1][2]<<","<<m[1][3]<<"|"<<std::endl;
		std::cout<<"|"<<m[2][0]<<","<<m[2][1]<<","<<m[2][2]<<","<<m[2][3]<<"|"<<std::endl;
		std::cout<<"|"<<m[3][0]<<","<<m[3][1]<<","<<m[3][2]<<","<<m[3][3]<<"|"<<std::endl;
	}
#endif

	/**quickly set valves to the matrix**/
	INLINE void set_quickly(f32 a0,f32 a1,f32 a2,f32 a3,
							f32 b0,f32 b1,f32 b2,f32 b3,
							f32 c0,f32 c1,f32 c2,f32 c3,
							f32 d0,f32 d1,f32 d2,f32 d3)
	{

		m[0][0] = a0; m[0][1] = a1; m[0][2] = a2; m[0][3] = a3;
		m[1][0] = b0; m[1][1] = b1; m[1][2] = b2; m[1][3] = b3;
		m[2][0] = c0; m[2][1] = c1; m[2][2] = c2; m[2][3] = c3;
		m[3][0] = d0; m[3][1] = d1; m[3][2] = d2; m[3][3] = d3;

	}
};



FORCEINLINE  RBMatrix::RBMatrix()
{
}

//行向量
FORCEINLINE RBMatrix::RBMatrix(const RBVector3& tx,const RBVector3& ty,const RBVector3& tz,const RBVector3& tw)
{
	m[0][0] = tx.x;m[0][1] = tx.y;m[0][2] = tx.z;m[0][3] = 0.f;
	m[1][0] = ty.x;m[1][1] = ty.y;m[1][2] = ty.z;m[1][3] = 0.f;
	m[2][0] = tz.x;m[2][1] = tz.y;m[2][2] = tz.z;m[2][3] = 0.f;
	m[3][0] = tw.x;m[3][1] = tw.y;m[3][2] = tw.z;m[3][3] = 1.f;
}

FORCEINLINE RBMatrix::RBMatrix(const RBVector4& tx,const RBVector4& ty,const RBVector4& tz,const RBVector4& tw)
{
	m[0][0] = tx.x;m[0][1] = tx.y;m[0][2] = tx.z;m[0][3] = tx.w;
	m[1][0] = ty.x;m[1][1] = ty.y;m[1][2] = ty.z;m[1][3] = ty.w;
	m[2][0] = tz.x;m[2][1] = tz.y;m[2][2] = tz.z;m[2][3] = tz.w;
	m[3][0] = tw.x;m[3][1] = tw.y;m[3][2] = tw.z;m[3][3] = tw.w;
}

FORCEINLINE RBMatrix::RBMatrix(f32 n00,f32 n01,f32 n02,f32 n03,f32 n10,f32 n11,f32 n12,f32 n13,f32 n20,f32 n21,f32 n22,f32 n23,f32 n30,f32 n31,f32 n32,f32 n33)
{
	
	m[0][0] = n00;m[0][1] = n01;m[0][2] = n02;m[0][3] = n03;
	m[1][0] = n10;m[1][1] = n11;m[1][2] = n12;m[1][3] = n13;
	m[2][0] = n20;m[2][1] = n21;m[2][2] = n22;m[2][3] = n23;
	m[3][0] = n30;m[3][1] = n31;m[3][2] = n32;m[3][3] = n33;

}

INLINE void RBMatrix::set_identity()
{
	m[0][0] = 1;m[0][1] = 0;m[0][2] = 0;m[0][3] = 0;
	m[1][0] = 0;m[1][1] = 1;m[1][2] = 0;m[1][3] = 0;
	m[2][0] = 0;m[2][1] = 0;m[2][2] = 1;m[2][3] = 0;
	m[3][0] = 0;m[3][1] = 0;m[3][2] = 0;m[3][3] = 1;
}


//Res = Mat1.operator*(Mat2) means Res = Mat1 * Mat2
FORCEINLINE RBMatrix RBMatrix::operator*(const RBMatrix& other) const
{
	//UE4 optimize it with SSE，refer to "UnrealMathSSE.h".
	RBMatrix _m;

	_m.m[0][0] = other.m[0][0]*m[0][0]+other.m[1][0]*m[0][1]+other.m[2][0]*m[0][2]+other.m[3][0]*m[0][3];
	_m.m[0][1] = other.m[0][1]*m[0][0]+other.m[1][1]*m[0][1]+other.m[2][1]*m[0][2]+other.m[3][1]*m[0][3];
	_m.m[0][2] = other.m[0][2]*m[0][0]+other.m[1][2]*m[0][1]+other.m[2][2]*m[0][2]+other.m[3][2]*m[0][3];
	_m.m[0][3] = other.m[0][3]*m[0][0]+other.m[1][3]*m[0][1]+other.m[2][3]*m[0][2]+other.m[3][3]*m[0][3];

	_m.m[1][0] = other.m[0][0]*m[1][0]+other.m[1][0]*m[1][1]+other.m[2][0]*m[1][2]+other.m[3][0]*m[1][3];
	_m.m[1][1] = other.m[0][1]*m[1][0]+other.m[1][1]*m[1][1]+other.m[2][1]*m[1][2]+other.m[3][1]*m[1][3];
	_m.m[1][2] = other.m[0][2]*m[1][0]+other.m[1][2]*m[1][1]+other.m[2][2]*m[1][2]+other.m[3][2]*m[1][3];
	_m.m[1][3] = other.m[0][3]*m[1][0]+other.m[1][3]*m[1][1]+other.m[2][3]*m[1][2]+other.m[3][3]*m[1][3];

	_m.m[2][0] = other.m[0][0]*m[2][0]+other.m[1][0]*m[2][1]+other.m[2][0]*m[2][2]+other.m[3][0]*m[2][3];
	_m.m[2][1] = other.m[0][1]*m[2][0]+other.m[1][1]*m[2][1]+other.m[2][1]*m[2][2]+other.m[3][1]*m[2][3];
	_m.m[2][2] = other.m[0][2]*m[2][0]+other.m[1][2]*m[2][1]+other.m[2][2]*m[2][2]+other.m[3][2]*m[2][3];
	_m.m[2][3] = other.m[0][3]*m[2][0]+other.m[1][3]*m[2][1]+other.m[2][3]*m[2][2]+other.m[3][3]*m[2][3];

	_m.m[3][0] = other.m[0][0]*m[3][0]+other.m[1][0]*m[3][1]+other.m[2][0]*m[3][2]+other.m[3][0]*m[3][3];
	_m.m[3][1] = other.m[0][1]*m[3][0]+other.m[1][1]*m[3][1]+other.m[2][1]*m[3][2]+other.m[3][1]*m[3][3];
	_m.m[3][2] = other.m[0][2]*m[3][0]+other.m[1][2]*m[3][1]+other.m[2][2]*m[3][2]+other.m[3][2]*m[3][3];
	_m.m[3][3] = other.m[0][3]*m[3][0]+other.m[1][3]*m[3][1]+other.m[2][3]*m[3][2]+other.m[3][3]*m[3][3];

	return _m;

}

FORCEINLINE void RBMatrix::operator*=(const RBMatrix& other)
{
	*this = *this*other;
	/*
	RBMatrix _m;
	_m.m[0][0] = other.m[0][0]*m[0][0]+other.m[1][0]*m[0][1]+other.m[2][0]*m[0][2]+other.m[3][0]*m[0][3];
	_m.m[0][1] = other.m[0][1]*m[0][0]+other.m[1][1]*m[0][1]+other.m[2][1]*m[0][2]+other.m[3][1]*m[0][3];
	_m.m[0][2] = other.m[0][2]*m[0][0]+other.m[1][2]*m[0][1]+other.m[2][2]*m[0][2]+other.m[3][2]*m[0][3];
	_m.m[0][3] = other.m[0][3]*m[0][0]+other.m[1][3]*m[0][1]+other.m[2][3]*m[0][2]+other.m[3][3]*m[0][3];

	_m.m[1][0] = other.m[0][0]*m[1][0]+other.m[1][0]*m[1][1]+other.m[2][0]*m[1][2]+other.m[3][0]*m[1][3];
	_m.m[1][1] = other.m[0][1]*m[1][0]+other.m[1][1]*m[1][1]+other.m[2][1]*m[1][2]+other.m[3][1]*m[1][3];
	_m.m[1][2] = other.m[0][2]*m[1][0]+other.m[1][2]*m[1][1]+other.m[2][2]*m[1][2]+other.m[3][2]*m[1][3];
	_m.m[1][3] = other.m[0][3]*m[1][0]+other.m[1][3]*m[1][1]+other.m[2][3]*m[1][2]+other.m[3][3]*m[1][3];

	_m.m[2][0] = other.m[0][0]*m[2][0]+other.m[1][0]*m[2][1]+other.m[2][0]*m[2][2]+other.m[3][0]*m[2][3];
	_m.m[2][1] = other.m[0][1]*m[2][0]+other.m[1][1]*m[2][1]+other.m[2][1]*m[2][2]+other.m[3][1]*m[2][3];
	_m.m[2][2] = other.m[0][2]*m[2][0]+other.m[1][2]*m[2][1]+other.m[2][2]*m[2][2]+other.m[3][2]*m[2][3];
	_m.m[2][3] = other.m[0][3]*m[2][0]+other.m[1][3]*m[2][1]+other.m[2][3]*m[2][2]+other.m[3][3]*m[2][3];

	_m.m[3][0] = other.m[0][0]*m[3][0]+other.m[1][0]*m[3][1]+other.m[2][0]*m[3][2]+other.m[3][0]*m[3][3];
	_m.m[3][1] = other.m[0][1]*m[3][0]+other.m[1][1]*m[3][1]+other.m[2][1]*m[3][2]+other.m[3][1]*m[3][3];
	_m.m[3][2] = other.m[0][2]*m[3][0]+other.m[1][2]*m[3][1]+other.m[2][2]*m[3][2]+other.m[3][2]*m[3][3];
	_m.m[3][3] = other.m[0][3]*m[3][0]+other.m[1][3]*m[3][1]+other.m[2][3]*m[3][2]+other.m[3][3]*m[3][3];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			m[i][j] = _m.m[i][j];
		}
	*/
}



FORCEINLINE RBMatrix RBMatrix::operator+(const RBMatrix& other) const
{
	RBMatrix _m;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			_m.m[i][j] = m[i][j] + other.m[i][j];
		}

	return _m;

}

FORCEINLINE void RBMatrix::operator+=(const RBMatrix& other)
{
	*this = *this + other;
}

INLINE bool RBMatrix::operator==(const RBMatrix& other) const
{
	for(i32 i=0;i<4;i++)
	{
		for(i32 j=0;j<4;j++)
		{
			if(m[i][j]!=other.m[i][j])
				return false;

		}
	}
	return true;
}

INLINE bool RBMatrix::equals(const RBMatrix& other, f32 tolerance/*=SMALL_F*/) const
{
	for(i32 i=0;i<4;i++)
	{
		for(i32 j=0;j<4;j++)
		{
			if(RBMath::abs(m[i][j] - other.m[i][j]) > tolerance)
				return false;

		}
	}
	return true;
}

INLINE bool RBMatrix::operator!=(const RBMatrix& other) const
{
	return !(*this==other);
}

FORCEINLINE RBMatrix RBMatrix::get_transposed() const
{
	RBMatrix _r;
	_r.m[0][0] = m[0][0];
	_r.m[0][1] = m[1][0];
	_r.m[0][2] = m[2][0];
	_r.m[0][3] = m[3][0];

	_r.m[1][0] = m[0][1];
	_r.m[1][1] = m[1][1];
	_r.m[1][2] = m[2][1];
	_r.m[1][3] = m[3][1];

	_r.m[2][0] = m[0][2];
	_r.m[2][1] = m[1][2];
	_r.m[2][2] = m[2][2];
	_r.m[2][3] = m[3][2];

	_r.m[3][0] = m[0][3];
	_r.m[3][1] = m[1][3];
	_r.m[3][2] = m[2][3];
	_r.m[3][3] = m[3][3];

	return _r;
}

INLINE f32 RBMatrix::get_determinant() const
{
	return 
		m[0][0] * (
				m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
				m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
				m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
				) -
			m[1][0] * (
				m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
				m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
				m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
				) +
			m[2][0] * (
				m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
				m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
				m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
				) -
			m[3][0] * (
				m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
				m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
				m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
				);
}

INLINE f32 RBMatrix::rotation_determinant() const
{
	return	
		m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
		m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
}

INLINE RBMatrix RBMatrix::get_inverse_slow() const
{
	RBMatrix _r;
	const float	Det = get_determinant();

	if(Det == 0.0f)
		return RBMatrix::identity;

	const float	RDet = 1.0f / Det;

	_r.m[0][0] = RDet * (
			m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			);
	_r.m[0][1] = -RDet * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			);
	_r.m[0][2] = RDet * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
	_r.m[0][3] = -RDet * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);

	_r.m[1][0] = -RDet * (
			m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			);
	_r.m[1][1] = RDet * (
			m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][0] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][0] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			);
	_r.m[1][2] = -RDet * (
			m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][0] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][0] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
	_r.m[1][3] = RDet * (
			m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][0] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][0] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);

	_r.m[2][0] = RDet * (
			m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
			m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
			m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
			);
	_r.m[2][1] = -RDet * (
			m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
			m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
			m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
			);
	_r.m[2][2] = RDet * (
			m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
			m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
			m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
			);
	_r.m[2][3] = -RDet * (
			m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
			m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
			m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
			);

	_r.m[3][0] = -RDet * (
			m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
			m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
			m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
			);
	_r.m[3][1] = RDet * (
			m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
			m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
			m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
			);
	_r.m[3][2] = -RDet * (
			m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
			m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
			m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
			);
	_r.m[3][3] = RDet * (
			m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
			m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
			m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
			);

	return _r;
}



//Below..Implementation for class RBVector4

//Apply a matrix to this vector with side use,implemented in Matrix.h
FORCEINLINE void RBVector4::apply_matrix(const RBMatrix& tm)
{
	RBVector4 _r;

	_r.x = tm.m[0][0]*x+tm.m[1][0]*y+tm.m[2][0]*z+tm.m[3][0]*w;
	_r.y = tm.m[0][1]*x+tm.m[1][1]*y+tm.m[2][1]*z+tm.m[3][1]*w;
	_r.z = tm.m[0][2]*x+tm.m[1][2]*y+tm.m[2][2]*z+tm.m[3][2]*w;
	_r.w = tm.m[0][3]*x+tm.m[1][3]*y+tm.m[2][3]*z+tm.m[3][3]*w;

	*this = _r;
}

//Matrix multiple to right with a return value,implemented in Matrix.h
FORCEINLINE RBVector4 RBVector4::operator*(const RBMatrix& tm) const
{
	RBVector4 _r;

	_r.x = tm.m[0][0]*x+tm.m[1][0]*y+tm.m[2][0]*z+tm.m[3][0]*w;
	_r.y = tm.m[0][1]*x+tm.m[1][1]*y+tm.m[2][1]*z+tm.m[3][1]*w;
	_r.z = tm.m[0][2]*x+tm.m[1][2]*y+tm.m[2][2]*z+tm.m[3][2]*w;
	_r.w = tm.m[0][3]*x+tm.m[1][3]*y+tm.m[2][3]*z+tm.m[3][3]*w;

	return _r;
}