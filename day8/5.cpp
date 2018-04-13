#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;
template <class T>
class CArray3D
{
  private:
	T **data;
	int x, y, z;
	int deep;

  public:
	CArray3D(int a, int b, int c, int d = 3, void *tmp = NULL) : x(a), y(b), z(c), deep(d)
	{
		if (d == 3 && tmp == NULL)
		{
			data = new T *[x];
			for (int i = 0; i < x; ++i)
				data[i] = new T[y * z];
		}
		else if (deep <= 2)
		{
			data = (T **)tmp;
		}
	}
	CArray3D operator[](int a)
	{
		if (deep == 3)
		{
			if (a >= x)
				a = x - 1;
			return CArray3D(x, y, z, 2, data[a]);
		}
		else if (deep == 2)
		{
			if (a >= y)
				a = y - 1;
			return CArray3D(x, y, z, 1, (T *)data + a * z);
		}
		else if (deep == 1)
		{
			if (a >= z)
				a = z - 1;
			return CArray3D(x, y, z, 0, (T *)data + a);
		}
	}
	CArray3D<T> &operator=(T x)
	{
		*((T *)data) = x;
		return *this;
	}
	operator T()
	{
		return *((T *)data);
	}
	operator T *()
	{
		if (deep == 2)
			return ((T *)data);
		if (deep == 1)
			return ((T *)data);
	}
	friend ostream &operator<<(ostream &out, CArray3D<T> x)
	{
		out << (T)x;
		return out;
	}
};

CArray3D<int> a(3, 4, 5);
CArray3D<double> b(3, 2, 2);
void PrintA()
{
	for (int i = 0; i < 3; ++i)
	{
		cout << "layer " << i << ":" << endl;
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 5; ++k)
				cout << a[i][j][k] << ",";
			cout << endl;
		}
	}
}
void PrintB()
{
	for (int i = 0; i < 3; ++i)
	{
		cout << "layer " << i << ":" << endl;
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
				cout << b[i][j][k] << ",";
			cout << endl;
		}
	}
}

int main()
{

	int No = 0;
	for (int i = 0; i < 3; ++i)
	{
		a[i];
		for (int j = 0; j < 4; ++j)
		{
			a[j][i];
			for (int k = 0; k < 5; ++k)
				a[i][j][k] = No++;
			a[j][i][i];
		}
	}
	PrintA();
	memset(a[1], -1, 20 * sizeof(int));
	memset(a[1], -1, 20 * sizeof(int));
	PrintA();
	memset(a[1][1], 0, 5 * sizeof(int));
	PrintA();

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k)
				b[i][j][k] = 10.0 / (i + j + k + 1);
	PrintB();
	int n = a[0][1][2];
	double f = b[0][1][1];
	cout << "****" << endl;
	cout << n << "," << f << endl;
	system("pause");
	return 0;
}
