#Notes:
##*libEGL.lib*&*libGLESv2.lib* is from Mali OpenGL ES Emulator v1.3.2
##other version lib may be wrong.
##Some dll is necessary.
##Better get  Mali OpenGL ES Emulator.
##http://malideveloper.arm.com/cn/develop-for-mali/tools/opengl-es-3-0-emulator/
---

#约定

---
**行向量**
**左手系**
**矩阵右乘**
**存在f32向量，和int向量**
#顺时针顶点方案，使用左手规则来确定向外的方向：手掌环绕方向与顶点编号递增
的方向相同时，拇指指向的方向就是向外的。

#数学库可用性主题
##此数学库若要用于外部，注意将PlatformExpand中的RBBasedata.h和WindowsPlatformnTemp.h
一起拷贝走
这是一个可用性问题，数学库中的所有数据全部都是自定义数据类型，在每个平台上都应该更改
RBBasedata.h来实现自定义数据类型


*三角面按照顺时针顶点顺序排列，使用左右螺旋,拇指方向所指即为正面*

---
#NOTE
##项目GLESCommon是一个ugly architecture只为了实现绘图，日后修改。


#NOTE:
**RBMath的文件包含是被精简过的，如果出现错误请检查**

/////////////////////////////////////////////////////
TO DO:
ADD THIS CONTENT TO MATRIX.H
/////////////////////////////////////////////////////
CONTENTS
/////////////////////////////////////////////////////
INLINE RBMatrix get_transpose_adjoint() const;

	// NOTE: There is some compiler optimization issues with WIN64 that cause FORCEINLINE to cause a crash
	// Remove any scaling from this matrix (ie magnitude of each row is 1) with error Tolerance
	INLINE void remove_scaling(f32 tolerance=SMALLER_F);

	// Returns matrix after RemoveScaling with error Tolerance
	INLINE RBMatrix get_matrix_without_scale(f32 Tolerance=SMALLER_F) const;

	/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present with error Tolerance */
	INLINE RBVector3 extract_scaling(f32 tolerance=SMALLER_F);

	/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector) with error Tolerance. */
	INLINE RBVector3 get_scale_vector(f32 tolerance=SMALLER_F) const;

	// Remove any translation from this matrix
	INLINE RBMatrix remove_translation() const;

	/** Returns a matrix with an additional translation concatenated. */
	INLINE RBMatrix ConcatTranslation(const RBVector3& Translation) const;

	/** Returns true if any element of this matrix is NaN */
	INLINE bool is_containsNaN() const;

	/** Scale the translation part of the matrix by the supplied vector. */
	INLINE void scale_translation(const RBVector3& Scale3D);

	/** @return the maximum magnitude of any row of the matrix. */
	INLINE f32 get_maximum_axis_scale() const;

	/** Apply Scale to this matrix **/
	INLINE RBMatrix apply_scale(f32 Scale);

	// @return the origin of the co-ordinate system
	INLINE RBVector3 get_origin() const;

	/**
	 * get axis of this matrix scaled by the scale of the matrix
	 *
	 * @param i index into the axis of the matrix
	 * @ return vector of the axis
	 */
	INLINE RBVector3 GetScaledAxis(Axiss Axis) const;

	/**
	 * get axes of this matrix scaled by the scale of the matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	INLINE void GetScaledAxes(RBVector3 &X, RBVector3 &Y, RBVector3 &Z) const;

	/**
	 * get unit length axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @ return vector of the axis
	 */
	INLINE RBVector3 GetUnitAxis(Axiss Axis) const;

	/**
	 * get unit length axes of this matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	INLINE void GetUnitAxes(RBVector3 &X, RBVector3 &Y, RBVector3 &Z) const;

	/**
	 * set an axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @param Axis vector of the axis
	 */
	INLINE void SetAxis( int32 i, const RBVector3& Axis );

	// Set the origin of the coordinate system to the given vector
	INLINE void SetOrigin( const RBVector3& NewOrigin );

	/**
	 * Update the axes of the matrix if any value is NULL do not update that axis
	 *
	 * @param Axis0 set matrix row 0
	 * @param Axis1 set matrix row 1 
	 * @param Axis2 set matrix row 2
	 * @param Origin set matrix row 3
	 */
	INLINE void SetAxes(RBVector3* Axis0 = NULL, RBVector3* Axis1 = NULL, RBVector3* Axis2 = NULL, RBVector3* Origin = NULL);

	
	/**
	 * get a column of this matrix
	 *
	 * @param i index into the column of the matrix
	 * @return vector of the column 
	 */
	INLINE RBVector3 GetColumn(int32 i) const;

	/** @return rotator representation of this matrix */
	RB_API FRotator Rotator() const;

	/** 
	 * Transform a rotation matrix into a quaternion.
	 * @warning rotation part will need to be unit length for this to be right!
	 */
	RB_API FQuat ToQuat() const;

	// Frustum plane extraction.
	/** @param OutPlane the near plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumNearPlane(FPlane& OutPlane) const;

	/** @param OutPlane the far plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumFarPlane(FPlane& OutPlane) const;

	/** @param OutPlane the left plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumLeftPlane(FPlane& OutPlane) const;

	/** @param OutPlane the right plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumRightPlane(FPlane& OutPlane) const;

	/** @param OutPlane the top plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumTopPlane(FPlane& OutPlane) const;

	/** @param OutPlane the bottom plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumBottomPlane(FPlane& OutPlane) const;

	/**
	 * Utility for mirroring this transform across a certain plane,
	 * and flipping one of the axis as well.
	 */
	INLINE void Mirror(Axiss MirrorAxis, Axiss FlipAxis);

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	RB_API FString ToString() const;

	/** Output ToString */
	void DebugPrint() const;

	/**
	 * Serializes the Matrix.
	 *
	 * @param Ar Reference to the serialization archive.
	 * @param M Reference to the matrix being serialized.
	 *
	 * @return Reference to the Archive after serialization.
	 */
	friend RB_API FArchive& operator<<(FArchive& Ar,RBMatrix& M);

	/**
	* Convert this Atom to the 3x4 transpose of the transformation matrix.
	*/
	void To3x4MatrixTranspose( f32* Out ) const
	{
		const f32 * RESTRICT Src = &(m[0][0]);
		f32 * RESTRICT Dest = Out;

		Dest[0] = Src[0];   // [0][0]
		Dest[1] = Src[4];   // [1][0]
		Dest[2] = Src[8];   // [2][0]
		Dest[3] = Src[12];  // [3][0]

		Dest[4] = Src[1];   // [0][1]
		Dest[5] = Src[5];   // [1][1]
		Dest[6] = Src[9];   // [2][1]
		Dest[7] = Src[13];  // [3][1]

		Dest[8] = Src[2];   // [0][2]
		Dest[9] = Src[6];   // [1][2]
		Dest[10] = Src[10]; // [2][2]
		Dest[11] = Src[14]; // [3][2]
	}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////