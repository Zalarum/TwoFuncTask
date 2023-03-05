#include <fstream>
#include <iostream>
#include <random>

using namespace std;

double SquareFunc(double& a, double& b)
{
	int data[5] = { -3, 0, -3, -2, -1 };

	double result = 0;

	for (int i = 0; i < 5; ++i)
	{
		result += pow(a * i + b - data[i], 2);
	}

	return result;
}

double ModulFunc(double& a, double& b)
{
	int data[5] = { -3, 0, -3, -2, -1 };

	double result = 0;

	for (int i = 0; i < 5; ++i)
	{
		result += abs(a * i + b - data[i]);
	}

	return result;
}

double MainSquareFunc(double& x, double& a, double& b, double& g1, double& g2)
{
	double t1 = a + x * g1, t2 = b + x * g2;

	return SquareFunc(t1, t2);
}

double MainModulFunc(double& x, double& a, double& b, double& g1, double& g2)
{
	double t1 = a + x * g1, t2 = b + x * g2;

	return ModulFunc(t1, t2);
}

void Dichotomy(ofstream& file_out, double& a0, double& b0, double& g1, double& g2, double(*Func)(double&, double&, double&, double&, double&))
{
	double epsilon = 1e-6, a = -10, b = 10, c, t1, t2;
	int i = 1;

	file_out << "n" << ";" << "a" << "; " << "b" << "; " << "(b - a) / 2" << "; " << "c" << "; " << "d" << "; " << "F(c)" << "; " << "F(d)" << endl;

	while (b - a > epsilon)
	{
		c = (a + b) / 2;
		
		t1 = c + epsilon;
		t2 = c - epsilon;

		file_out << i << ";" << a << ";" << b << ";" << (b - a) / 2 << ";" << t2 << ";" << t1 << ";" << Func(t2, a0, b0, g1, g2) << ";" << Func(t1, a0, b0, g1, g2) << endl;

		if (Func(t1, a0, b0, g1, g2) > Func(t2, a0, b0, g1, g2)) b = c;
		else a = c;

		++i;
	}

	file_out << endl;
}

void GoldenRatio(ofstream& file_out, double& a0, double& b0, double& g1, double& g2, double(*Func)(double&, double&, double&, double&, double&))
{
	double epsilon = 1e-6, a = -10, b = 10, g, h, fu1, fu2;
	int i = 1;

	g = a + (b - a) * (3 - sqrt(5)) / 2;
	h = a + (b - a) * (sqrt(5) - 1) / 2;
	fu1 = Func(g, a0, b0, g1, g2);
	fu2 = Func(h, a0, b0, g1, g2);

	file_out << "n" << ";" << "a" << "; " << "b" << "; " << "(b - a) / 2" << "; " << "c" << "; " << "d" << "; " << "F(c)" << "; " << "F(d)" << endl;

	while (b - a > epsilon)
	{
		file_out << i << ";" << a << ";" << b << ";" << (b - a) / 2 << ";" << h << ";" << g << ";" << Func(h, a0, b0, g1, g2) << ";" << Func(g, a0, b0, g1, g2) << endl;

		if (fu1 < fu2)
		{
			b = h;
			h = g;
			fu2 = fu1;
			g = a + (b - a) * (3 - sqrt(5)) / 2;
			fu1 = Func(g, a0, b0, g1, g2);
		}
		else
		{
			a = g;
			g = h;
			fu1 = fu2;
			h = a + (b - a) * (sqrt(5) - 1) / 2;
			fu2 = Func(h, a0, b0, g1, g2);
		}

		++i;
	}

	file_out << endl;

}

int main()
{
	ofstream file_out;

	file_out.open("Excel_table.csv");

	random_device rd;
	uniform_real_distribution<double> dist1(0, 1);
	uniform_real_distribution<double> dist5(-5, 5);

	double g1, g2, a, b;

	g1 = dist1(rd);
	g2 = sqrt(1 - pow(g1, 2));
	a = dist5(rd);
	b = dist5(rd);

	file_out << "Метод дихотомии для первой функции" << endl;
	Dichotomy(file_out, a, b, g1, g2, MainSquareFunc);
	file_out << "Метод золотого сечения для первой функции" << endl;
	GoldenRatio(file_out, a, b, g1, g2, MainSquareFunc);
	file_out << "Метод дихотомии для второй функции" << endl;
	Dichotomy(file_out, a, b, g1, g2, MainModulFunc);
	file_out << "Метод золотого сечения для второй функции" << endl;
	GoldenRatio(file_out, a, b, g1, g2, MainModulFunc);

	file_out.close();

	system("Excel_table.csv");

	return 0;
}