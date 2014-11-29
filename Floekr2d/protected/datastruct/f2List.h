#ifndef F2_LIST_H
#define F2_LIST_H

// --!�ɸ���ʽ
// �ײ�ʵ��Ϊvector
#include <vector>
#include <Floekr2d/protected/f2Settings.h>

using namespace std;

namespace Floekr2d
{
    template <class Type>
    class f2List
    {
    public:
        f2List() {};

        void pushback(Type& object);

        void popback();

        void remove(uint32 index);

        void clear();

        bool isEmpty() const;

        Type& operator[](uint32 index);

        int size() const;

    private:
        std::vector<Type> list;
    };

    template <class Type>
    void f2List<Type>::pushback(Type& object)
    {
        list.push_back(object);
    }

    template <class Type>
    void f2List<Type>::popback()
    {
        list.pop_back();
    }

    template <class Type>
    void f2List<Type>::remove(uint32 index)
    {
        std::vector<Type>::iterator it;

        if(index < list.size())
        {
            it = list.begin() + index;
            list.erase(it);
        }
        else
        {
            cout << "�޷�ɾ�����Ϊ:" << index << "�ŵ�Ԫ�� �ñ����Խ��" << endl;
            cout << "��������Σ�ղ���" << endl;
        }
        
        //for(it = list.begin(); it != list.end();)
        //{
        //    if(*it == object)
        //    {
        //        it = list.erase(it); //����д��arr.erase(it);
        //    }
        //    else
        //        ++it;
        //}
    }

    template <class Type>
    void f2List<Type>::clear()
    {
        list.clear();
    }

    template <class Type>
    bool f2List<Type>::isEmpty() const
    {
        return list.empty();
    }

    template <class Type>
    int f2List<Type>::size() const
    {
        return list.size();
    }

    template <class Type>
    Type& f2List<Type>::operator[](uint32 index)
    {
        return list[index];
    }
}



#endif