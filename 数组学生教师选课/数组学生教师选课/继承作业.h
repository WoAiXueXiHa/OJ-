#pragma once
#include <iostream>
using namespace std;
#define pai 3.14;
/*
* 写一个几何体类给出计算面积的纯虚函数；
 三角形类继承几何体类重写计算面积的函数，
 矩形继承几何体类计算面积的函数

 写一个几何体为底的柱体类，给出计算体积的函数
 写main测试自己的类和函数
*/

class Geometry {
public:
	// 计算面积
	virtual double area() = 0;
	virtual ~Geometry() {}
};

class Triangle : public Geometry {
private:
	double _height = 0.0;
	double _base = 0.0;
public:
	Triangle(double height, double base)
		:_height(height)
		, _base(base) 
	{
		cout << "Triangle(double height, double base)" << endl;
	}
	double area() {
		return (_height * _base) / 2.0;
	}

};

class Rectangle : public Geometry {
private:
	double _width = 0.0;
	double _length = 0.0;
public:
	Rectangle(double width, double length)
		:_width(width)
		, _length(length)
	{
		cout << "Rectangle(double width, double length)" << endl;
	}

	double area() {
		return _width * _length;
	}
};

class Cylinder {
public:
	Cylinder(Geometry* geo, double height) : _area(geo), _height(height) {}
		
	double V() {
		return _height * _area->area();
	}
	~Cylinder() {}
private:
	Geometry* _area;
	double _height;
};

class Cycle : public Geometry {
private: 
	double _r = 0.0;
public:
	Cycle(double r) : _r(r) { cout << "Cycle(double r)" << endl; }
	double area() {
		return _r * _r * pai;
	}
};

void test() {
	
	Triangle tri(3, 4);    
	Rectangle rect(2, 3);  
	Cycle circle(2);       

	
	Cylinder cy1(&tri, 5);  
	Cylinder cy2(&rect, 4); 
	Cylinder cy3(&circle, 3); 

	
	cout << "三角形面积：" << tri.area() << endl;
	cout << "三角形柱体体积：" << cy1.V() << endl << endl;

	cout << "矩形面积：" << rect.area() << endl;
	cout << "矩形柱体体积：" << cy2.V() << endl << endl;

	cout << "圆形面积：" << circle.area() << endl;
	cout << "圆形柱体体积：" << cy3.V() << endl;
}

