#include <iostream>  // Thư viện để sử dụng các hàm nhập/xuất chuẩn
#include <string>    // Thư viện để sử dụng kiểu dữ liệu chuỗi
#include "exprtk.hpp" // Thư viện để phân tích và tính toán các biểu thức toán học

using namespace std;  // Sử dụng không gian tên chuẩn để tiết kiệm cú pháp

// Hàm tính đạo hàm tại một điểm x
double derivative(exprtk::expression<double>& expr, double x, exprtk::symbol_table<double>& symbol_table) {
    double h = 1e-7;  // Bước nhảy rất nhỏ
    double x_original = x;  // Lưu giá trị gốc của x

    // Tính giá trị hàm số tại x + h
    x = x + h;
    double fxh1 = expr.value();

    // Tính giá trị hàm số tại x - h
    x = x_original - h;
    double fxh2 = expr.value();

    // Khôi phục giá trị ban đầu của x
    x = x_original;

    // Tính đạo hàm theo công thức sai phân trung tâm
    return (fxh1 - fxh2) / (2 * h);
}

// Hàm kiểm tra sự tồn tại của đạo hàm tại một điểm x
bool is_derivative_defined(exprtk::expression<double>& expr, double x, exprtk::symbol_table<double>& symbol_table) {
    double h = 1e-7;  // Bước nhảy rất nhỏ
    double x_original = x;  // Lưu giá trị gốc của x

    // Tính đạo hàm bên phải tại x + h
    x = x + h;
    double fxh1 = expr.value();
    x = x_original;
    double f_right = (fxh1 - expr.value()) / h;

    // Tính đạo hàm bên trái tại x - h
    x = x_original - h;
    double fxh2 = expr.value();
    x = x_original;
    double f_left = (expr.value() - fxh2) / h;

    // Kiểm tra sự chênh lệch giữa đạo hàm bên trái và bên phải
    return abs(f_right - f_left) < 1e-5;
}

int main() {
    string expression;  // Biến lưu trữ biểu thức hàm số
    double x0;  // Biến lưu trữ điểm x0

    // Yêu cầu người dùng nhập biểu thức hàm số
    cout << "Nhap bieu thuc ham so: ";
    getline(cin, expression);

    // Khởi tạo bảng ký hiệu và thêm biến x vào bảng ký hiệu
    exprtk::symbol_table<double> symbol_table;
    double x;
    symbol_table.add_variable("x", x);

    // Khởi tạo biểu thức và đăng ký bảng ký hiệu
    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    // Khởi tạo parser và biên dịch biểu thức
    exprtk::parser<double> parser;
    if (!parser.compile(expression, expr)) {
        cerr << "Error: " << parser.error() << endl;
        return 1;  // Thoát chương trình nếu có lỗi biên dịch biểu thức
    }

    // Yêu cầu người dùng nhập giá trị của x0
    cout << "Nhap tiep diem x0: ";
    cin >> x0;
    x = x0;  // Gán giá trị x0 cho x
    double y0 = expr.value();  // Tính giá trị hàm số tại x0

    // Kiểm tra sự tồn tại của đạo hàm tại x0
    if (!is_derivative_defined(expr, x0, symbol_table)) {
        cout << "Dao ham khong ton tai tai diem x0 = " << x0 << endl;
    }
    else {
        double f_prime = derivative(expr, x0, symbol_table);  // Tính đạo hàm tại x0
        cout << "f'(x) tai diem " << x0 << " : " << f_prime << endl;

        if (f_prime == 0) {  // Trường hợp đạo hàm bằng 0
            cout << "Phuong trinh tiep tuyen tai diem (" << x0 << ", " << y0 << "): y = " << y0 << endl;
            cout << "Phuong trinh phap tuyen tai diem (" << x0 << ", " << y0 << "): x = " << x0 << endl;
        }
        else {  // Trường hợp đạo hàm khác 0
            cout << "Phuong trinh tiep tuyen tai diem (" << x0 << ", " << y0 << "): y = "
                << f_prime << " * (x - " << x0 << ") + " << y0 << endl;

            cout << "Phuong trinh phap tuyen tai diem (" << x0 << ", " << y0 << "): y = "
                << -1 / f_prime << " * (x - " << x0 << ") + " << y0 << endl;
        }
    }
    return 0;
}
