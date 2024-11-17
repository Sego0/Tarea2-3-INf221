#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <chrono>

using namespace std;

// Tablas de costos
vector<vector<int>> costReplace(26, vector<int>(26, 0));
vector<int> costInsert(26, 0);
vector<int> costDelete(26, 0);
vector<vector<int>> costTranspose(26, vector<int>(26, 0));

// Función para cargar las tablas de costos desde archivos
void cargarCostos() {
    ifstream file;

    // Cargar costos de inserción
    file.open("cost_insert.txt");
    for (int i = 0; i < 26; ++i) {
        file >> costInsert[i];
    }
    file.close();

    // Cargar costos de eliminación
    file.open("cost_delete.txt");
    for (int i = 0; i < 26; ++i) {
        file >> costDelete[i];
    }
    file.close();

    // Cargar costos de sustitución
    file.open("cost_replace.txt");
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> costReplace[i][j];
        }
    }
    file.close();

    // Cargar costos de transposición
    file.open("cost_transpose.txt");
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> costTranspose[i][j];
        }
    }
    file.close();
}

// Calcula el costo de sustituir el carácter 'a' por 'b'
int costo_sub(char a, char b) {
    return costReplace[a - 'a'][b - 'a'];
}

// Calcula el costo de insertar el carácter 'b'
int costo_ins(char b) {
    return costInsert[b - 'a'];
}

// Calcula el costo de eliminar el carácter 'a'
int costo_del(char a) {
    return costDelete[a - 'a'];
}

// Calcula el costo de transponer los caracteres 'a' y 'b' adyacentes
int costo_trans(char a, char b) {
    return costTranspose[a - 'a'][b - 'a'];
}

// Función de fuerza bruta para encontrar la distancia mínima de edición
int editDistanceBruteForce(string str1, string str2, int m, int n) {
    // Caso base: si una de las cadenas es vacía, el costo es el de insertar o eliminar
    if (m == 0) return n > 0 ? n * costo_ins(str2[n - 1]) : 0;
    if (n == 0) return m > 0 ? m * costo_del(str1[m - 1]) : 0;

    // Si los últimos caracteres son iguales, no hay costo para esta posición
    if (str1[m - 1] == str2[n - 1]) {
        return editDistanceBruteForce(str1, str2, m - 1, n - 1);
    }

    // Calcular los costos de las operaciones
    int insertCost = editDistanceBruteForce(str1, str2, m, n - 1) + costo_ins(str2[n - 1]);
    int deleteCost = editDistanceBruteForce(str1, str2, m - 1, n) + costo_del(str1[m - 1]);
    int replaceCost = editDistanceBruteForce(str1, str2, m - 1, n - 1) + costo_sub(str1[m - 1], str2[n - 1]);

    // Encontrar el mínimo entre inserción, eliminación y sustitución
    int minCost = min({insertCost, deleteCost, replaceCost});

    // Si los caracteres son adyacentes y se pueden transponer, calcular el costo de transposición
    if (m > 1 && n > 1 && str1[m - 1] == str2[n - 2] && str1[m - 2] == str2[n - 1]) {
        int transposeCost = editDistanceBruteForce(str1, str2, m - 2, n - 2) + costo_trans(str1[m - 2], str1[m - 1]);
        minCost = min(minCost, transposeCost);
    }

    return minCost;
}

vector<pair<string, string>> leerCadenas(const string& filename) {
    vector<pair<string, string>> cadenas;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return cadenas;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string str1, str2;

        // Separar las cadenas por la coma
        if (getline(ss, str1, ',') && getline(ss, str2, ',')) {
            cadenas.emplace_back(str1, str2);
        }
    }

    file.close();
    return cadenas;
}

void limpiarCadena(string& str) {
    str.erase(remove_if(str.begin(), str.end(), [](char c) {
        return c < 'a' || c > 'z'; // Filtra caracteres fuera de rango
    }), str.end());
}

int main() {
    // Cargar los costos desde los archivos de texto
    cargarCostos();

    // Cargar los datasets desde los archivos de texto
    vector<pair<string, string>> similares = leerCadenas("similar_strings_100.txt");
    vector<pair<string, string>> repetidas = leerCadenas("repeated_strings_100.txt");
    vector<pair<string, string>> transposiciones = leerCadenas("transposable_strings_100.txt");
    vector<pair<string, string>> distintas = leerCadenas("distinct_strings_100.txt");
    
    // Definir las cadenas de entrada
    int n = 0;
    while (n < 10){
        string str1 = similares[n].first;
        string str2 = similares[n].second;
        limpiarCadena(str1);
        limpiarCadena(str2);

        auto inicio = chrono::high_resolution_clock::now();
        int result = editDistanceBruteForce(str1, str2, str1.length(), str2.length());
        auto fin = chrono::high_resolution_clock::now();

        auto duracion = chrono::duration_cast<chrono::nanoseconds >(fin - inicio).count();
        
        // Calcular y mostrar la distancia mínima de edición
        cout << result << " " << duracion << endl;
        n++;
    }
    


    return 0;
}