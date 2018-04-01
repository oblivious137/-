#include <iostream> 
using namespace std;
class Point { 
	private: 
		int x; 
		int y; 
	public: 
		Point() { };

        friend istream& operator >> (istream& in, Point& x){
            in>>x.x>>x.y;
            return in;
        }
        friend ostream& operator << (ostream& out, const Point& x){
            out<<x.x<<','<<x.y;
            return out;
        }
}; 
int main() 
{ 
 	Point p;
 	while(cin >> p) {
 		cout << p << endl;
	 }
	return 0;
}