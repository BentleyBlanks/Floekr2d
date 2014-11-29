#ifndef F2_VECTOR2_H
#define F2_VECTOR2_H

#include <Floekr2d/protected/f2Settings.h>
#include <Floekr2d/protected/math/f2Math.h>

namespace Floekr2d
{
    namespace math
    {
        template <class Type>
        class f2Vector2
        {
        public:
            Type x, y;

            f2Vector2() :x(0), y(0) {}
            f2Vector2(Type x, Type y) :x(x), y(y) {}
            // ���ƹ���
            f2Vector2(const f2Vector2<Type>& vector) :x(vector.x), y(vector.y) {}

            void clone(const f2Vector2<Type>& vector) { x = vector.x; y = vector.y; }

            void set(const Type x, const Type y) { this->x = x; this->y = y; }

            // ���� =
            f2Vector2<Type>& operator=(const f2Vector2<Type>& vector);
            // ���� == !=
            bool operator==(const f2Vector2<Type>& vector) const;
            bool operator!=(const f2Vector2<Type>& vector) const;

            // ����һԪ - +
            f2Vector2<Type> operator-();
            f2Vector2<Type> operator+();

            // ���� ��Ԫ + -
            f2Vector2<Type> operator+(const f2Vector2<Type>& vector) const;
            f2Vector2<Type> operator-(const f2Vector2<Type>& vector) const;

            // ���� �˳��� * / scale
            f2Vector2<Type> operator*(const Type scale) const;
            // ���
            f2Vector2<Type> operator/(const Type scale) const;

            // ���� += -= *= /=
            f2Vector2<Type>& operator+=(const f2Vector2<Type>& vector);
            f2Vector2<Type>& operator-=(const f2Vector2<Type>& vector);
            f2Vector2<Type>& operator*=(const Type scale);
            f2Vector2<Type>& operator/=(const Type scale);

            // ���� * vector(����inlineһ���ҳ� ��Ϊ������ͬ���Ҳ���)
            Type operator*(const f2Vector2<Type>& vector) const;

            // ��Ϊ������
            inline void zero()
            {
                // --!֮���Դ˴���(Type)ǿ������ת�� ����Ϊ���岻�� ����ȴ���ıȽϴ���ٶ�
                x = 0;
                y = 0;
            }

            // ����2D������׼��
            inline bool normalize()
            {
                Type length = getLength();
                if(length == 0)
                    return false;

                Type inverseLength = 1.0f / length;
                x *= inverseLength;
                y *= inverseLength;

                return true;
            }

            // ��ģ��
            inline Type getLength() const
            {
                return sqrtf(x*x + y*y);
            }

            inline Type getLengthSquare() const
            {
                return (x*x + y*y);
            }

            // ���
            inline Type cross(const f2Vector2<Type>& vector) const
            {
                // |i j k|
                // |a b c|
                // |x y z|
                // |axb|�ĳ��ȴ�������a��bΪ�ڱߵ�ƽ���ı��ε����
                return x*vector.y - y*vector.x;
            }

            // ��һ��������Ľ��Ϊ ����õ�ǰ�����Ĵ�ֱ����(-y, x)������scalar��
            // ���
            template <class Type>
            inline f2Vector2<Type> cross(const float32& scalar) const
            {
                return f2Vector2<Type>(-this->y*scalar, scalar*this->x);
            }

            inline Type dot(const f2Vector2<Type>& vector) const
            {
                return this->x*vector.x + this->y*vector.y;
            }

            // ��������
            inline Type getDistance(const f2Vector2<Type>& end) const
            {
                return sqrt((end.x - x)*(end.x - x) + (end.y - y)*(end.y - y));
            }

            inline Type getDistanceSquare(const f2Vector2<Type>& end) const
            {
                return (end.x - x)*(end.x - x) + (end.y - y)*(end.y - y);
            }

            // ����ʹ�õ�Console Log���
            inline void print(std::string name)
            {
#ifdef _DEBUG
                // Ϊ��
                if(name.empty())
                    name = "";
                std::cout << "f2Vector2 ";
                std::cout << name << "(x: " << x << ",y: " << y << ")" << std::endl << std::endl;
#endif
            }

            inline void print()
            {
#ifdef _DEBUG
                std::cout << "f2Vector2 ";
                std::cout << "(x: " << x << ",y: " << y << ")" << std::endl << std::endl;
#endif
            }

        };

        // ��Ա����
        // ���� =
        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator=(const f2Vector2<Type>& vector)
        {
            this->x = vector.x;
            this->y = vector.y;

            return *this;
        }

        // ���� == !=
        template <class Type>
        bool f2Vector2<Type>::operator==(const f2Vector2<Type>& vector) const
        {
            return equals(this->x, vector.x) && equals(this->y, vector.y);
        }

        template <class Type>
        bool f2Vector2<Type>::operator!=(const f2Vector2<Type>& vector) const
        {
            return !(equals(this->x, vector.x) && equals(this->y, vector.y));
        }

        // ����һԪ - +
        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator-()
        {
            f2Vector2<Type> v;
            v.set(-x, -y); 
            return v;
        }

        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator+()
        {
            return *this;
        }

        // ���� ��Ԫ + -
        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator+(const f2Vector2<Type>& vector) const
        {
            //--!�Ӽ��ź����ҳ�˳����ͬ
            return f2Vector2<Type>(x + vector.x, y + vector.y);
        }

        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator-(const f2Vector2<Type>& vector) const
        {
            return f2Vector2<Type>(x - vector.x, y - vector.y);
        }

        // ���� �˳��� * / scale
        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator*(const Type scale) const
        {
            return f2Vector2<Type>(x * scale, y * scale);
        }

        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator/(const Type scale) const
        {
            if(scale == 0)
#ifdef _DEBUG
                // --!δ���滻Ϊlog
                std::cout << "��������Ϊ��!" << std::endl;
#endif
            return f2Vector2<Type>(x / scale, y / scale);
        }

        // ���� += -= *= /=
        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator+=(const f2Vector2<Type>& vector)
        {
            //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
            x += vector.x;
            y += vector.y;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator-=(const f2Vector2<Type>& vector)
        {
            //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
            x -= vector.x;
            y -= vector.y;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator*=(const Type scale)
        {
            //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
            x *= scale;
            y *= scale;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator/=(const Type scale)
        {
            //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
            x /= scale;
            y /= scale;
            return *this;
        }

        // ���� * vector(����inlineһ���ҳ� ��Ϊ������ͬ���Ҳ���)
        template <class Type>
        Type f2Vector2<Type>::operator*(const f2Vector2<Type>& vector) const
        {
            return (x*vector.x + y*vector.y);
        }


        // �ǳ�Ա����
        // ��˱���
        // --!float32���͵�Scale��Ϊ�˱���Scaleģ��������Martix���ͳ�ͻ���±������޷��ҵ���������
        // template<class Scale, class Type>
        template<class Type>
        f2Vector2<Type> operator*(const float32 scale, const f2Vector2<Type>& vector)
        {
            return f2Vector2<Type>(scale * vector.x, scale * vector.y);
        }


        // vector����һ����
        // ʵ��Ϊ��vector����90�ȵõ�tempV ֮��������scalar��
        template<class Type>
        inline f2Vector2<Type> f2Cross(const Type scalar, const f2Vector2<Type>& vector)
        {
            return f2Vector2<Type>(-scalar*vector.y, scalar*vector.x);
        }

        // �ػ�
        typedef f2Vector2<float32> f2Vector2f;
        typedef f2Vector2<int32> f2Vector2i;

        // ������
        static f2Vector2f f2Vector2fZero;
        static f2Vector2i f2Vector2iZero;
    }
}

#endif