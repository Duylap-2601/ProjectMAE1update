#include <iostream>
#include <string>
#include <cmath>
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

double partial_derivative(exprtk::expression<double>& expr, exprtk::symbol_table<double>& symbol_table, double& var) {
    double h = 1e-7;
    var = var + h;
    double fph = expr.value();
    var = var - 2 * h;
    double fmh = expr.value();
    var = var + h;
    return (fph - fmh) / (2 * h);
}

int main() {
    string expression;
    double x0, y0;

    // Nhập biểu thức
    cout << "Nhap bieu thuc ham so: ";
    getline(cin, expression);

    exprtk::symbol_table<double> symbol_table;
    double x, y;
    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);

    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    if (!parser.compile(expression, expr)) {
        cerr << "Lỗi: " << parser.error() << endl;
        return 1;
    }

    // Kiểm tra xem biểu thức có chứa biến 'y' hay không
    bool is_explicit = (expression.find('y') == string::npos);

    if (is_explicit) {
        // Hàm tường minh f(x)
        cout << "Nhap toa do x0: ";
        cin >> x0;
        x = x0;

        double y0 = expr.value();
        double f_prime = derivative(expr, x, symbol_table);

        cout << "Phuong trinh tiep tuyen tai diem (" << x0 << ", " << y0 << "): y = "
            << f_prime << " * (x - " << x0 << ") + " << y0 << endl;

        cout << "Phuong trinh phap tuyen tai diem (" << x0 << ", " << y0 << "): y = "
            << -1 / f_prime << " * (x - " << x0 << ") + " << y0 << endl;
    }
    else {
        // Hàm ẩn F(x, y)
        cout << "Nhap toa do diem x0: ";
        cin >> x0;
        cout << "Nhap toa do diem y0: ";
        cin >> y0;
        x = x0;
        y = y0;

        double Fxy = expr.value();
        if (abs(Fxy) > 1e-7) {
            cerr << "Loi. Vui long nhap mot diem hop le." << endl;
            return 1;
        }

        double Fx = partial_derivative(expr, symbol_table, x);
        double Fy = partial_derivative(expr, symbol_table, y);

        double slope_tangent = -Fx / Fy;

        cout << "Phuong trinh tiep tuyen tai (" << x0 << ", " << y0 << "): y = "
            << slope_tangent << " * (x - " << x0 << ") + " << y0 << endl;

        cout << "Phuong trinh phap tuyen tai (" << x0 << ", " << y0 << "): y = "
            << -1 / slope_tangent << " * (x - " << x0 << ") + " << y0 << endl;
    }

    return 0;
}
