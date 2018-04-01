#include <cstdlib>
#include <iostream>
using namespace std;
int strlen(const char * s) 
{	int i = 0;
	for(; s[i]; ++i);
	return i;
}
void strcpy(char * d,const char * s)
{
	int i = 0;
	for( i = 0; s[i]; ++i)
		d[i] = s[i];
	d[i] = 0;
		
}
int strcmp(const char * s1,const char * s2)
{
	for(int i = 0; s1[i] && s2[i] ; ++i) {
		if( s1[i] < s2[i] )
			return -1;
		else if( s1[i] > s2[i])
			return 1;
	}
	return 0;
}
void strcat(char * d,const char * s)
{
	int len = strlen(d);
	strcpy(d+len,s);
}
class MyString
{
  private:
    char *p;
    int length;

  public:
    MyString() : p(NULL), length(0){};
    MyString(const char *s)
    {
        if (s)
        {
            length = strlen(s);
            p = new char[length + 1];
            strcpy(p, s);
        }
        else
            p = NULL, length = 0;
    }
    MyString(const char *s, const char *t)
    {
        if (t - s == 0)
        {
            length = 0;
            p = NULL;
        }
        else
        {
            length = 0;
            p = new char[t - s + 1];
            while (s != t)
            {
                p[length++] = *(s++);
            }
            p[length] = '\0';
        }
        return;
    }
    MyString(const MyString &x)
    {
        length = x.length;
        if (x.p)
        {
            p = new char[length + 1];
            strcpy(p, x.p);
        }
        else
            p = NULL;
    }
    void append(const char *s)
    {
        if (s == NULL)
            return;
        char *tmp;
        tmp = new char[length + strlen(s) + 1];
        if (length)
        {
            strcpy(tmp, p);
            delete[] p;
        }
        strcpy(tmp + length, s);
        p = tmp;
        length += strlen(s);
        return;
    }
    void append(const char *s, const char *t)
    {
        if (t - s == 0)
            return;
        char *tmp;
        tmp = new char[length + t - s + 1];
        if (length)
        {
            strcpy(tmp, p);
            delete[] p;
        }
        p = tmp;
        while (s != t)
        {
            p[length++] = *(s++);
        }
        p[length] = '\0';
        return;
    }
    friend ostream &operator<<(ostream &out, const MyString &x)
    {
        if (x.p)
            out << x.p;
        return out;
    }
    MyString &operator=(const MyString &x)
    {
        if (p)
            delete[] p;
        length = x.length;
        if (x.p)
        {
            p = new char[x.length + 1];
            strcpy(p, x.p);
        }
        else
            p = NULL;
        return *this;
    }
    MyString operator+(const MyString &x) const
    {
        MyString ret(*this);
        ret.append(x.p);
        return ret;
    }
    char &operator[](const int a)
    {
        if (a >= 0 && a < length)
            return p[a];
        else
        {
            /*

            */
        }
    }
    bool operator<(const MyString &x) const
    {
        for (int i = 0; i < length && i < x.length; ++i)
            if (p[i] < x.p[i])
                return true;
            else if (p[i] > x.p[i])
                return false;
        return false;
    }
    bool operator>(const MyString &x) const
    {
        return x < *this;
    }
    bool operator==(const MyString &x) const
    {
        return !(*this < x) && !(x < *this);
    }
    MyString &operator+=(const MyString &x)
    {
        append(x.p);
        return *this;
    }
    MyString operator()(const int a, const int b)
    {
        MyString ret(p + a, p + a + b);
        return ret;
    }
    friend MyString operator + (const char *p, const MyString& x){
        MyString ret(p);
        ret+=x;
        return ret;
    }
};

int CompareString(const void *e1, const void *e2)
{
    MyString *s1 = (MyString *)e1;
    MyString *s2 = (MyString *)e2;
    if (*s1 < *s2)
        return -1;
    else if (*s1 == *s2)
        return 0;
    else if (*s1 > *s2)
        return 1;
}
int main()
{
    MyString s1("abcd-"), s2, s3("efgh-"), s4(s1);
    MyString SArray[4] = {"big", "me", "about", "take"};
    cout << "1. " << s1 << s2 << s3 << s4 << endl;
    s4 = s3;
    s3 = s1 + s3;
    cout << "2. " << s1 << endl;
    cout << "3. " << s2 << endl;
    cout << "4. " << s3 << endl;
    cout << "5. " << s4 << endl;
    cout << "6. " << s1[2] << endl;
    s2 = s1;
    s1 = "ijkl-";
    s1[2] = 'A';
    cout << "7. " << s2 << endl;
    cout << "8. " << s1 << endl;
    s1 += "mnop";
    cout << "9. " << s1 << endl;
    s4 = "qrst-" + s2;
    cout << "10. " << s4 << endl;
    s1 = s2 + s4 + " uvw " + "xyz";
    cout << "11. " << s1 << endl;
    qsort(SArray, 4, sizeof(MyString), CompareString);
    for (int i = 0; i < 4; i++)
        cout << SArray[i] << endl;
    //s1的从下标0开始长度为4的子串
    cout << s1(0, 4) << endl;
    //s1的从下标5开始长度为10的子串
    cout << s1(5, 10) << endl;
    return 0;
}