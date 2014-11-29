#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix.h"

//RBVector4 definetion
RBVector4::RBVector4(RBVector2 axy,RBVector2 azw)
	:x(axy.x),y(axy.y),z(azw.x),w(azw.y){}
const RBVector4 RBVector4::zero_vector(0.f,0.f,0.f,1.f);


//RBVector3 definetion
RBVector3::RBVector3(const RBVector2 v,f32 az):x(v.x),y(v.y),z(az){}
RBVector2 RBVector3::unit_cartesian_to_spherical() const
{
	const f32 t = RBMath::acos(z/size());
	const f32 p = RBMath::atant2(y,x);
	return RBVector2(t,p);
}

const RBVector3 RBVector3::zero_vector(0.f,0.f,0.f);
const RBVector3 RBVector3::up_vector(0.f,0.f,1.f);


//RBVector2 definetion
const RBVector2 RBVector2::zero_vector(0.0f, 0.0f);
const RBVector2 RBVector2::unit_vector(1.0f, 1.0f);


//RBMatrix definetion
const RBMatrix RBMatrix::identity(RBVector4(1,0,0,0),RBVector4(0,1,0,0),RBVector4(0,0,1,0),RBVector4(0,0,0,1));