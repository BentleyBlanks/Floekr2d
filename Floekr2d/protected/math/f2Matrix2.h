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
            // ���ַ�ʽ���ʾ���
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
            // ���ƹ���
            f2Matrix2(const f2Matrix2<Type>& matrix) :m00(matrix.m00), m01(matrix.m01), m10(matrix.m10), m11(matrix.m11){}

            inline void clone(const f2Matrix2<Type>& matrix) { m00 = matrix.m00; m01 = matrix.m01; m10 = matrix.m10; m11 = matrix.m11; }
            
            // ת�ñ���
            inline void transpose()
            {
                //ʵ��Ϊ�Խ��߶Ի�
                Type temp = m00;
                m00 = m11;
                m11 = temp;
            }

            // �õ�����ת�þ���
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

            // ��ά��ƽ�ƾ���
            // ��ת����
            inline void makeRotationMatrixa(float32 angle)
            {
                float32 radians = angle * DEGREE_TO_RADIAN;

                Type c = std::cos(radians);
                Type s = std::sin(radians);

                // ����
                zero();

                m00 = c; m01 = -s;
                m10 = s; m11 = c;
            }

            inline void makeRotationMatrixr(float32 radians)
            {
                Type c = std::cos(radians);
                Type s = std::sin(radians);

                // ����
                zero();

                m00 = c; m01 = -s;
                m10 = s; m11 = c;
            }

            // ���ž���
            inline void makeScaleMatrix(float32 sx, float32 sy)
            {
                // ����
                zero();

                m00 = sx;
                m11 = sy;
            }


            // ���������
            // ���� =
            f2Matrix2<Type>& operator=(const f2Matrix2<Type>& matrix);
            // ���� == !=
            bool operator==(const f2Matrix2<Type>& matrix) const;
            bool operator!=(const f2Matrix2<Type>& matrix) const;

            // ����һԪ - +
            f2Matrix2<Type>& operator-();
            f2Matrix2<Type>& operator+();

            // ���� ��Ԫ + -
            // --!float32���͵�Scale��Ϊ�˱���Scaleģ��������Martix���ͳ�ͻ���±������޷��ҵ���������
            // template<class Scale, class Type>
            f2Matrix2<Type> operator+(const f2Matrix2<Type>& matrix) const;
            f2Matrix2<Type> operator-(const f2Matrix2<Type>& matrix) const;
            f2Matrix2<Type> operator*(const float32 scale) const;

            // �ҳ������� ���inline
            template <class vectorType>
            f2Vector2<vectorType> operator*(const f2Vector2<vectorType>& vector) const;

            // ����˷�
            f2Matrix2<Type> operator*(const f2Matrix2<Type>& matrix) const;
           
            // ���� += -= *=
            f2Matrix2<Type>& operator+=(const f2Matrix2<Type>& matrix);
            f2Matrix2<Type>& operator-=(const f2Matrix2<Type>& matrix);
            f2Matrix2<Type>& operator*=(const float32 scale);
            f2Matrix2<Type>& operator*=(const f2Matrix2<Type>& matrix);


            // ����ʹ�õ�Console Log���
            inline void print(std::string name) const
            {
#ifdef _DEBUG
                // Ϊ��
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


        // ���� =
        template <class Type>
        f2Matrix2<Type>& f2Matrix2<Type>::operator=(const f2Matrix2<Type>& matrix)
        {
            m00 = matrix.m00;
            m01 = matrix.m01;
            m10 = matrix.m10;
            m11 = matrix.m11;

            return *this;
        }

        // ���� == !=
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

        // ����һԪ - +
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

        // ���� ��Ԫ + -
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

        // �ҳ�������
        // --!�ο��� http://www.cppblog.com/fwxjj/archive/2014/03/10/73139.html ����ģ������ģ�庯����д��
        template <class Type>
        template <class vectorType>
        f2Vector2<vectorType> f2Matrix2<Type>::operator*(const f2Vector2<vectorType>& vector) const
        {
            return f2Vector2<vectorType>(m00 * vector.x + m01 * vector.y, m10 * vector.x + m11 * vector.y);
        }

        // ����˷�
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

        // ���� += -= *=
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

        // �ػ�
        typedef f2Matrix2<int32> f2Matrix2i;
        typedef f2Matrix2<float32> f2Matrix2f;

        // �����
        static f2Matrix2f f2Matrix2fZero;
        static f2Matrix2i f2Matrix2iZero;
    }
}


// �ǳ�Ա���������������ռ�������
// ��˱���
template<class Type>
inline Floekr2d::math::f2Matrix2<Type> operator*(const float32 scale, const Floekr2d::math::f2Matrix2<Type>& matrix)
{
    return Floekr2d::math::f2Matrix2<Type>(scale * matrix.m00, scale * matrix.m01, scale * matrix.m10, scale * matrix.m11);
}

// ���������
template<class Type, class vectorType>
inline Floekr2d::math::f2Vector2<vectorType> operator*(const Floekr2d::math::f2Vector2<vectorType>& vector, const Floekr2d::math::f2Matrix2<Type>& matrix)
{
    return Floekr2d::math::f2Vector2<vectorType>(matrix.m00 * vector.x + matrix.m10 * vector.y, matrix.m01 * vector.x + matrix.m11 * vector.y);
}

#endif