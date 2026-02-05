#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double compute_catalan_series(int n) {
    double G = 0.0;
    for (int k = 0; k <= n; k++) {
        G += pow(-1, k) / pow(2 * k + 1, 2);
    }
    return G;
}

double compute_catalan_integral(int n) {
    double G = 0.0;
    double step = 1.0 / n;
    
    for (int i = 0; i < n; i++) {
        double x = i * step;
        double atan_term = atan(x) / x;
        if (x > 0) {
            G += atan_term * step;
        }
    }
    
    return G;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: catalan_compute <n>" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if (n <= 0) {
        cout << "n must be positive" << endl;
        return 1;
    }
    
    // Metoda 1: Seria alternantă
    double G_series = compute_catalan_series(n);
    
    // Metoda 2: Integrala
    double G_integral = compute_catalan_integral(n);
    
    // Calculăm eroarea (diferența dintre cele două metode)
    double error = abs(G_series - G_integral);
    
    cout << fixed << setprecision(15);
    cout << "<catalan_series>" << G_series << "</catalan_series>" << endl;
    cout << "<catalan_integral>" << G_integral << "</catalan_integral>" << endl;
    cout << "<error>" << scientific << error << "</error>" << endl;
    
    return 0;
}