#ifndef F2_MATRIX2_H
#define F2_MATRIX2_H

#include <Floekr2d/protected/math/f2Vector2.h>

namespace Floekr2d
{
    namespace math
    {
        template <class Type>
        class f2Matrix2
        {
        public:
            // 多种方式访问矩阵
            union
            {
                struct 
                {
                    Type m00, m01;
                    Type m10, m11;
                };

                Type m[2][2];
                Type v[4];
            };

            f2Matrix2() { m00 = m01 = m10 = m11 = 0; }
            f2Matrix2(Type m00, Type m01, Type m10, Type m11)
                :m00(m00), m01(m01), m10(m10), m11(m11) {}
            // 复制构造
            f2Matrix2(const f2Matrix2<Type>& matrix) :m00(matrix.m00), m01(matrix.m01), m10(matrix.m10), m11(matrix.m11){}

            inline void clone(const f2Matrix2<Type>& matrix) { m00 = matrix.m00; m01 = matrix.m01; m10 = matrix.m10; m11 = matrix.m11; }
            
            // 转置本身
            inline void transpose()
            {
                //实质为对角线对换
                Type temp = m00;
                m00 = m11;
                m11 = temp;
            }

            // 得到自身转置矩阵
            inline f2Matrix2<Type> getTranspose() const 
            {
                return f2Matrix2<Type>(m00, m10, m01, m11);
            }

            inline void zero()
            {
                m00 = m01 = m10 = m11 = 0;
            }

            inline void set(Type m00, Type m01, Type m10, Type m11)
            {
                m00 = m00;
                m01 = m01;
                m10 = m10;
                m11 = m11;
            }

            inline void setIdentity()
            {
                m00 = 1;  m01 = 0;
                m10 = 0;  m11 = 1;
            }

            // 二维无平移矩阵
            // 旋转矩阵
            inline void makeRotationMatrixa(float32 angle)
            {
                float32 radians = angle * DEGREE_TO_RADIAN;

                Type c = std::cos(radians);
                Type s = std::sin(radians);

                // 置零
                zero();

                m00 = c; m01 = -s;
                m10 = s; m11 = c;
            }

            inline void makeRotationMatrixr(float32 radians)
            {
                Type c = std::cos(radians);
                Type s = std::sin(radians);

                // 置零
                zero();

                m00 = c; m01 = -s;
                m10 = s; m11 = c;
            }

            // 缩放矩阵
            inline void makeScaleMatrix(float32 sx, float32 sy)
            {
                // 置零
                zero();

                m00 = sx;
                m11 = sy;
            }


            // 运算符重载
            // 重载 =
            f2Matrix2<Type>& operator=(const f2Matrix2<Type>& matrix);
            // 重载 == !=
            bool operator==(const f2Matrix2<Type>& matrix) const;
            bool operator!=(const f2Matrix2<Type>& matrix) const;

            // 重载一元 - +
            f2Matrix2<Type>& operator-();
            f2Matrix2<Type>& operator+();

            // 重载 二元 + -
            // --!float32类型的Scale是为了避免Scale模板类型与Martix类型冲突导致编译器无法找到合理类型
            // template<class Scale, class Type>
            f2Matrix2<Type> operator+(const f2Matrix2<Type>& matrix) const;
            f2Matrix2<Type> operator-(const f2Matrix2<Type>& matrix) const;
            f2Matrix2<Type> operator*(const float32 scale) const;

            // 右乘列向量 左乘inline
            template <class vectorType>
            f2Vector2<vectorType> operator*(const f2Vector2<vectorType>& vector) const;

            // 矩阵乘法
            f2Matrix2<Type> operator*(const f2Matrix2<Type>& matrix) const;
           
            // 重载 += -= *=
            f2Matrix2<Type>& operator+=(const f2Matrix2<Type>& matrix);
            f2Matrix2<Type>& operator-=(const f2Matrix2<Type>& matrix);
            f2Matrix2<Type>& operator*=(const float32 scale);
            f2Matrix2<Type>& operator*=(const f2Matrix2<Type>& matrix);


            // 测试使用的Console Log输出
            inline void print(std::string name) const
            {
#ifdef _DEBUG
                // 为空
                if(name.empty())
                    name = "";
                cout << "f2Matrix2 " << endl;
                cout << name << "[ " << m00 << " " << m01 << " ]" << endl;
                cout << name << "[ " << m10 << " " << m11 << " ]" << endl << endl;
#endif
            }

            inline void print() const
            {
#ifdef _DEBUG
                cout << "f2Matrix2 " << endl;
                cout << "[ " << m00 << " " << m01 << " ]" << endl;
                cout << "[ " << m10 << " " << m11 << " ]" << endl << endl;
#endif
            }
        };


        // 重载 =
        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator=(const f2Matrix2<Type>& matrix)
        {
            m00 = matrix.m00;
            m01 = matrix.m01;
            m10 = matrix.m10;
            m11 = matrix.m11;

            return *this;
        }

        // 重载 == !=
        template <class Type>
        bool f2Matrix2<Type>::operator==(const f2Matrix2<Type>& matrix) const
        {
            return equals(this->m00, matrix.m00) && equals(this->m01, matrix.m01) 
                && equals(this->m10, matrix.m10) && equals(this->m11, matrix.m11);
        }

        template <class Type>
        bool f2Matrix2<Type>::operator!=(const f2Matrix2<Type>& matrix) const
        {
            return !(equals(this->m00, matrix.m00) && equals(this->m01, matrix.m01)
                && equals(this->m10, matrix.m10) && equals(this->m11, matrix.m11));
        }

        // 重载一元 - +
        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator-()
        {
            m00 = -m00;
            m01 = -m01;
            m10 = -m10;
            m11 = -m11;
        }

        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator+()
        {
            m00 = +m00;
            m01 = +m01;
            m10 = +m10;
            m11 = +m11;
        }

        // 重载 二元 + -
        template <class Type>
        f2Matrix2<Type> f2Matrix2<Type>::operator+(const f2Matrix2<Type>& matrix) const
        {
            return f2Matrix2<Type>(m00 + matrix.m00, m01 + matrix.m01, m10 + matrix.m10, m11 + matrix.m11);
        }

        template <class Type>
        f2Matrix2<Type> f2Matrix2<Type>::operator-(const f2Matrix2<Type>& matrix) const
        {
            return f2Matrix2<Type>(m00 - matrix.m00, m01 - matrix.m01, m10 - matrix.m10, m11 - matrix.m11);
        }

        template <class Type>
        f2Matrix2<Type> f2Matrix2<Type>::operator*(const float32 scale) const
        {
            return f2Matrix2<Type>(m00 * scale, m01 * scale, m10 * scale, m11 * scale);
        }

        // 右乘列向量
        // --!参考于 http://www.cppblog.com/fwxjj/archive/2014/03/10/73139.html 关于模板类中模板函数的写法
        template <class Type>
        template <class vectorType>
        f2Vector2<vectorType> f2Matrix2<Type>::operator*(const f2Vector2<vectorType>& vector) const
        {
            return f2Vector2<vectorType>(m00 * vector.x + m01 * vector.y, m10 * vector.x + m11 * vector.y);
        }

        // 矩阵乘法
        template <class Type>
        f2Matrix2<Type> f2Matrix2<Type>::operator*(const f2Matrix2<Type>& matrix) const
        {
            // [m00 m01]  [m00 m01] 
            // [m10 m11]  [m10 m11] 

            return f2Matrix2<Type>(
                m00 * matrix.m00 + m01 * matrix.m10, m00 * matrix.m01 + m01 * matrix.m11,
                m10 * matrix.m00 + m11 * matrix.m10, m10 * matrix.m01 + m11 * matrix.m11
                );
        }

        // 重载 += -= *=
        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator+=(const f2Matrix2<Type>& matrix)
        {
            this = this + matrix;
        }

        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator-=(const f2Matrix2<Type>& matrix)
        {
            this = this - matrix;
        }

        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator*=(const float32 scale)
        {
            this = this * scale;
        }

        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator*=(const f2Matrix2<Type>& matrix)
        {
            this = this * matrix;
        }

        // 特化
        typedef f2Matrix2<int32> f2Matrix2i;
        typedef f2Matrix2<float32> f2Matrix2f;

        // 零矩阵
        static f2Matrix2f f2Matrix2fZero;
        static f2Matrix2i f2Matrix2iZero;
    }
}


// 非成员函数不能在命名空间中重载
// 左乘标量
template<class Type>
inline Floekr2d::math::f2Matrix2<Type> operator*(const float32 scale, const Floekr2d::math::f2Matrix2<Type>& matrix)
{
    return Floekr2d::math::f2Matrix2<Type>(scale * matrix.m00, scale * matrix.m01, scale * matrix.m10, scale * matrix.m11);
}

// 左乘行向量
template<class Type, class vectorType>
inline Floekr2d::math::f2Vector2<vectorType> operator*(const Floekr2d::math::f2Vector2<vectorType>& vector, const Floekr2d::math::f2Matrix2<Type>& matrix)
{
    return Floekr2d::math::f2Vector2<vectorType>(matrix.m00 * vector.x + matrix.m10 * vector.y, matrix.m01 * vector.x + matrix.m11 * vector.y);
}

#endif