#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: catalan_assimilate <result_file>" << endl;
        return 1;
    }
    
    ifstream infile(argv[1]);
    if (!infile) {
        cout << "Cannot open file" << endl;
        return 1;
    }
    
    string line;
    double series_val = 0.0;
    double integral_val = 0.0;
    double error_val = 0.0;
    
    while (getline(infile, line)) {
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
        if (line.find("<error>") != string::npos) {
            size_t start = line.find(">") + 1;
            size_t end = line.find("<", start);
            error_val = stod(line.substr(start, end - start));
        }
    }
    
    infile.close();
    
    // Constanta Catalan teoretică
    double catalan_true = 0.915965594177219015054603514;
    
    // Scriem rezultatul final într-un fișier de ieșire
    ofstream outfile("catalan_result.txt");
    outfile << fixed << setprecision(15);
    outfile << "Catalan Constant Calculation Results:" << endl;
    outfile << "=====================================" << endl;
    outfile << "Series method: " << series_val << endl;
    outfile << "Integral method: " << integral_val << endl;
    outfile << "Average: " << (series_val + integral_val) / 2.0 << endl;
    outfile << "Theoretical value: " << catalan_true << endl;
    outfile << "Error (vs theoretical): " << scientific << abs((series_val + integral_val)/2.0 - catalan_true) << endl;
    outfile << "Internal error estimate: " << error_val << endl;
    
    outfile.close();
    
    return 0;
}