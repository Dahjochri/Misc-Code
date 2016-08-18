#include <iostream.h>



//
// Inheritance
//
class CPolygon
{
    protected:
        int width, height;
    public:
        CPolygon(){};
        CPolygon(int a, int b){width = a; height = b;}
        void set_values(int a, int b){width = a; height = b;};
        int GetWidth(){return width;};
        int GetHeight(){return height;};
};

class COutput
{
    public:
        void output(int i){cout << i << endl;};
        void coords(CPolygon poly){cout << poly.GetWidth() << "," << poly.GetHeight() << endl;};
};

class CRectangle: public CPolygon, public COutput
{
    public:
        CRectangle(){};
        CRectangle(int a, int b) : CPolygon(a,b) {};
        int area(){return (width * height);};
};

class CTriangle: public CPolygon, public COutput
{
    public:
        CTriangle(){};
        CTriangle(int a, int b) : CPolygon(a,b){};
        int area(){return (width*height)/2;};
};



//
// Templates
//

template <class T>
T GetMax(T a, T b)
{
    T result;
    result = (a>b) ? a : b;
    return result;
}
template <class T>
T GetMin(T a, T b)
{
    T result;
    result = (a>b) ? b : a;
    return result;
}

//Template class
template <class O>
class mypair
{
    O a,b;
    public:
    mypair(O first, O second)
        {a=first; b=second;}
    O getmax();
    O getmin();
};
template <class O>
O mypair<O>::getmax()
{
    O retval;
    retval = a>b ? a : b;
    return retval;
}
template <class O>
O mypair<O>::getmin()
{
    O retval;
    retval = a>b ? b : a;
    return retval;
}

//Template class
template <class G>
class container
{
    G element;

    public:
        container (G arg){element = arg;}
        G increase() { return ++element;}
};
//Template specialization
template <>
class container <char>
{
    char element;

    public:
        container(char arg){element=arg;}
        char uppercase()
        {
            if((element>'a')&&(element<'z'))
            {
                element += 'A' - 'a';
                return element;
            }
        }
};


int main(int argc, char **argv)
{
    cout << "Showing off inheritance" << endl;
    CRectangle rect(5,5);
    CTriangle tri;
    tri.set_values(5,5);
    tri.output(tri.area());
    tri.coords(tri);
    rect.output(rect.area());
    rect.coords(rect);

    cout << "Showing off templates" << endl;
    int i1 = 5, i2 = 24;
    float f1 = 3.14, f2 = 1.962;
    cout << GetMin<int>(f1,i1) << endl; //Need <> brackets if vars isn't same type,
                                        //and with <> bracks. it will typecast all to the bracketed type
    cout << GetMax(f1,f2) << endl;


    cout << "Showing off template classes" << endl;
    mypair <float> whatever (3.1415926535, 3.00212);
    cout << whatever.getmax() << endl;
    cout << whatever.getmin() << endl;


    cout << "Showing off template specialization" << endl;
    container<int> myint(7);
    container<char> mychar('x');
    cout << myint.increase() << endl;
    cout << mychar.uppercase() << endl;

    return 0;
}
