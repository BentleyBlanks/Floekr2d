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
            // 复制构造
            f2Vector2(const f2Vector2<Type>& vector) :x(vector.x), y(vector.y) {}

            void clone(const f2Vector2<Type>& vector) { x = vector.x; y = vector.y; }

            void set(const Type x, const Type y) { this->x = x; this->y = y; }

            // 重载 =
            f2Vector2<Type>& operator=(const f2Vector2<Type>& vector);
            // 重载 == !=
            bool operator==(const f2Vector2<Type>& vector) const;
            bool operator!=(const f2Vector2<Type>& vector) const;

            // 重载一元 - +
            f2Vector2<Type> operator-();
            f2Vector2<Type> operator+();

            // 重载 二元 + -
            f2Vector2<Type> operator+(const f2Vector2<Type>& vector) const;
            f2Vector2<Type> operator-(const f2Vector2<Type>& vector) const;

            // 标量 乘除法 * / scale
            f2Vector2<Type> operator*(const Type scale) const;
            // 会对
            f2Vector2<Type> operator/(const Type scale) const;

            // 重载 += -= *= /=
            f2Vector2<Type>& operator+=(const f2Vector2<Type>& vector);
            f2Vector2<Type>& operator-=(const f2Vector2<Type>& vector);
            f2Vector2<Type>& operator*=(const Type scale);
            f2Vector2<Type>& operator/=(const Type scale);

            // 向量 * vector(无需inline一个右乘 因为类型相同左右不分)
            Type operator*(const f2Vector2<Type>& vector) const;

            // 置为零向量
            inline void zero()
            {
                // --!之所以此处不(Type)强制类型转换 是因为意义不大 但是却消耗比较大的速度
                x = 0;
                y = 0;
            }

            // 将此2D向量标准化
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

            // 求模长
            inline Type getLength() const
            {
                return sqrtf(x*x + y*y);
            }

            inline Type getLengthSquare() const
            {
                return (x*x + y*y);
            }

            // 叉积
            inline Type cross(const f2Vector2<Type>& vector) const
            {
                // |i j k|
                // |a b c|
                // |x y z|
                // |axb|的长度代表了以a和b为邻边的平行四边形的面积
                return x*vector.y - y*vector.x;
            }

            // 与一标量叉积的结果为 先求得当前向量的垂直向量(-y, x)再缩放scalar倍
            // 叉积
            template <class Type>
            inline f2Vector2<Type> cross(const float32& scalar) const
            {
                return f2Vector2<Type>(-this->y*scalar, scalar*this->x);
            }

            inline Type dot(const f2Vector2<Type>& vector) const
            {
                return this->x*vector.x + this->y*vector.y;
            }

            // 两点间距离
            inline Type getDistance(const f2Vector2<Type>& end) const
            {
                return sqrt((end.x - x)*(end.x - x) + (end.y - y)*(end.y - y));
            }

            inline Type getDistanceSquare(const f2Vector2<Type>& end) const
            {
                return (end.x - x)*(end.x - x) + (end.y - y)*(end.y - y);
            }

            // 测试使用的Console Log输出
            inline void print(std::string name)
            {
#ifdef _DEBUG
                // 为空
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

        // 成员函数
        // 重载 =
        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator=(const f2Vector2<Type>& vector)
        {
            this->x = vector.x;
            this->y = vector.y;

            return *this;
        }

        // 重载 == !=
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

        // 重载一元 - +
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

        // 重载 二元 + -
        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator+(const f2Vector2<Type>& vector) const
        {
            //--!加减号和左右乘顺序相同
            return f2Vector2<Type>(x + vector.x, y + vector.y);
        }

        template <class Type>
        f2Vector2<Type> f2Vector2<Type>::operator-(const f2Vector2<Type>& vector) const
        {
            return f2Vector2<Type>(x - vector.x, y - vector.y);
        }

        // 标量 乘除法 * / scale
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
                // --!未来替换为log
                std::cout << "除数不能为零!" << std::endl;
#endif
            return f2Vector2<Type>(x / scale, y / scale);
        }

        // 重载 += -= *= /=
        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator+=(const f2Vector2<Type>& vector)
        {
            //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
            x += vector.x;
            y += vector.y;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator-=(const f2Vector2<Type>& vector)
        {
            //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
            x -= vector.x;
            y -= vector.y;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator*=(const Type scale)
        {
            //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
            x *= scale;
            y *= scale;
            return *this;
        }

        template <class Type>
        f2Vector2<Type>& f2Vector2<Type>::operator/=(const Type scale)
        {
            //--!之所以不使用重载后的+-*/是希望少调用一次函数 减少开销
            x /= scale;
            y /= scale;
            return *this;
        }

        // 向量 * vector(无需inline一个右乘 因为类型相同左右不分)
        template <class Type>
        Type f2Vector2<Type>::operator*(const f2Vector2<Type>& vector) const
        {
            return (x*vector.x + y*vector.y);
        }


        // 非成员函数
        // 左乘标量
        // --!float32类型的Scale是为了避免Scale模板类型与Martix类型冲突导致编译器无法找到合理类型
        // template<class Scale, class Type>
        template<class Type>
        f2Vector2<Type> operator*(const float32 scale, const f2Vector2<Type>& vector)
        {
            return f2Vector2<Type>(scale * vector.x, scale * vector.y);
        }


        // vector左叉积一标量
        // 实质为将vector左旋90度得到tempV 之后再缩放scalar倍
        template<class Type>
        inline f2Vector2<Type> f2Cross(const Type scalar, const f2Vector2<Type>& vector)
        {
            return f2Vector2<Type>(-scalar*vector.y, scalar*vector.x);
        }

        // 特化
        typedef f2Vector2<float32> f2Vector2f;
        typedef f2Vector2<int32> f2Vector2i;

        // 零向量
        static f2Vector2f f2Vector2fZero;
        static f2Vector2i f2Vector2iZero;
    }
}

#endif