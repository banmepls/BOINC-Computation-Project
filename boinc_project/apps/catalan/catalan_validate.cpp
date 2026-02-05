#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

bool validate(vector<string> results) {
    vector<double> series_values;
    vector<double> integral_values;
    
    for (string result : results) {
        istringstream iss(result);
        string line;
        double series_val = 0.0;
        double integral_val = 0.0;
        
        while (getline(iss, line)) {
            if (line.find("<catalan_series>") != string::npos) {
                size_t start = line.find(">") + 1;
                size_t end = line.find("<", start);
                series_val = stod(line.substr(start, end - start));
            }
            if (line.find("<catalan_integral>") != string::npos) {
                size_t start = line.find(">") + 1;
                size_t end = line.find("<", start);
                integral_val = stod(line.substr(start, end - start));
            }
        }
        
        series_values.push_back(series_val);
        integral_values.push_back(integral_val);
    }
    
    // Verificăm dacă valorile sunt similare
    double avg_series = 0.0;
    double avg_integral = 0.0;
    
    for (double val : series_values) {
        avg_series += val;
    }
    avg_series /= series_values.size();
    
    for (double val : integral_values) {
        avg_integral += val;
    }
    avg_integral /= integral_values.size();
    
    // Dacă media diferă prea mult, rezultatele nu sunt valide
    if (abs(avg_series - avg_integral) > 0.001) {
        return false;
    }
    
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    
    vector<string> results;
    for (int i = 1; i < argc; i++) {
        results.push_back(argv[i]);
    }
    
    if (validate(results)) {
        cout << "VALID" << endl;
    } else {
        cout << "INVALID" << endl;
    }
    
    return 0;
}