#include <iostream>
#include <string>
#include "exprtk.hpp"
using namespace std;
double derivative(exprtk::expression<double>& expr, double& x, exprtk::symbol_table<double>& symbol_table) {
    double h = 1e-7;
    x = x + h;
    double fxh1 = expr.value();
    x = x - 2 * h;
    double fxh2 = expr.value();
    x = x + h;
    return (fxh1 - fxh2) / (2 * h);
}

int main() {
    string expression;
    double x0;

    cout << "Nhap bieu thuc ham so: ";
    getline(std::cin, expression);

    exprtk::symbol_table<double> symbol_table;
    double x;
    symbol_table.add_variable("x", x);

    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    if (!parser.compile(expression, expr)) {
        std::cerr << "Error: " << parser.error() << endl;
        return 1;
    }

    cout << "Nhap tiep diem x0: ";
    cin >> x0;
    x = x0;
    double y0 = expr.value();
    double f_prime = derivative(expr, x, symbol_table);
    cout << "f'(x) tai diem " << x0 << " : " << f_prime << endl;
    if (f_prime == 0)
    {
        cout << "Phuong trinh tiep tuyen tai diem (" << x0 << ", " << y0 << "): y = "
            << y0 << endl;
        cout << "Phuong trinh phap tuyen tai diem (" << x0 << ", " << y0 << "): x = " << x0 << endl;
    }
    else
    {

        cout << "Phuong trinh tiep tuyen tai diem (" << x0 << ", " << y0 << "): y = "
            << f_prime << " * (x - " << x0 << ") + " << y0 << endl;

        cout << "Phuong trinh phap tuyen tai diem (" << x0 << ", " << y0 << "): y = "
            << -1 / f_prime << " * (x - " << x0 << ") + " << y0 << endl;
    }
    return 0;
}
