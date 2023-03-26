#include <fstream>
#include <iostream>
#include <random>

using namespace std;

double a0, b0, g1, g2;
double epsilon = 1e-6, delta = 5e-7;

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

double MainSquareFunc(double& x)
{
	double t1 = a0 + x * g1, t2 = b0 + x * g2;

	return SquareFunc(t1, t2);
}

double MainModulFunc(double& x)
{
	double t1 = a0 + x * g1, t2 = b0 + x * g2;

	return ModulFunc(t1, t2);
}

void Dichotomy(ofstream& file_out, double(*Func)(double&))
{
	double a = -10, b = 10, c, left, right;
	int i = 1;

	file_out << "n" << ";" << "a" << ";" << "b" << ";" << "(b - a) / 2" << ";"
			 << "c" << ";" << "d" << ";" << "F(c)" << ";" << "F(d)" << endl;

	while (b - a > epsilon)
	{
		c = (a + b) / 2;

		left = c - delta;
		right = c + delta;

		file_out << i << "; " << a << "; " << b << "; " << (b - a) / 2 << "; "
				 << left << "; " << right << "; " << Func(left) << "; " << Func(right) << endl;

		if (Func(right) > Func(left)) b = left;
		else a = right;

		++i;
	}

	file_out << endl;
	file_out << "Минимум = " << c << endl;
	file_out << endl;
}

void GoldenRatio(ofstream& file_out, double(*Func)(double&))
{
	double a = -10, b = 10, left, right, f1, f2;
	int i = 1;

	left = a + (b - a) * (3 - sqrt(5)) / 2;
	right = a + (b - a) * (sqrt(5) - 1) / 2;
	f1 = Func(left);
	f2 = Func(right);

	file_out << "n" << ";" << "a" << ";" << "b" << ";" << "(b - a) / 2" << ";"
			 << "c" << ";" << "d" << ";" << "F(c)" << ";" << "F(d)" << endl;

	while (b - a > epsilon)
	{
		file_out << i << "; " << a << "; " << b << "; " << (b - a) / 2 << "; "
				 << right << "; " << left << "; " << Func(right) << "; " << Func(left) << endl;

		if (f1 < f2)
		{
			b = right;
			right = left;
			f2 = f1;
			left = a + (b - a) * (3 - sqrt(5)) / 2;
			f1 = Func(left);
		}
		else
		{
			a = left;
			left = right;
			f1 = f2;
			right = a + (b - a) * (sqrt(5) - 1) / 2;
			f2 = Func(right);
		}

		++i;
	}

	file_out << endl;
	file_out << "Минимум = " << (a + b) / 2 << endl;
	file_out << endl;
}

int main()
{
	ofstream file_out;

	file_out.open("Excel_table.csv");

	random_device rd;
	uniform_real_distribution<double> dist1(0, 1);
	uniform_real_distribution<double> dist5(-5, 5);

	g1 = dist1(rd);
	g2 = sqrt(1 - pow(g1, 2));
	a0 = dist5(rd);
	b0 = dist5(rd);

	file_out << "Метод дихотомии для первой функции" << endl;
	Dichotomy(file_out, MainSquareFunc);
	file_out << "Метод золотого сечения для первой функции" << endl;
	GoldenRatio(file_out, MainSquareFunc);
	file_out << "Метод дихотомии для второй функции" << endl;
	Dichotomy(file_out, MainModulFunc);
	file_out << "Метод золотого сечения для второй функции" << endl;
	GoldenRatio(file_out, MainModulFunc);

	file_out.close();

	system("Excel_table.csv");

	return 0;
}
