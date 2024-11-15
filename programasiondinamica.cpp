#include <bits/stdc++.h>
#include <ctime>      
#include <sys/resource.h> 
using namespace std;

class cEditDistance {
public:
    cEditDistance() {
        costInsert.resize(26);
        costDelete.resize(26);
        costReplace.assign(26, vector<int>(26));
        costTranspose.assign(26, vector<int>(26));
    }
    void loadCosts() {
        ifstream insertFile("cost_insert.txt"), deleteFile("cost_delete.txt"), 
                  replaceFile("cost_replace.txt"), transposeFile("cost_transpose.txt");

        if (!insertFile || !deleteFile || !replaceFile || !transposeFile) {
            cerr << "No se pudo abrir uno de los archivos de costo." << endl;
            exit(1);
        }
        for (int i = 0; i < 26; i++) insertFile >> costInsert[i];
        for (int i = 0; i < 26; i++) deleteFile >> costDelete[i];
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < 26; j++) {
                replaceFile >> costReplace[i][j];
            }
        }
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < 26; j++) {
                transposeFile >> costTranspose[i][j];
            }
        }

        insertFile.close();
        deleteFile.close();
        replaceFile.close();
        transposeFile.close();
    }
    void Init(int rows, int cols) {
        m_results.assign(rows, vector<int>(cols, -1));
        m_Operations.assign(rows - 1, vector<int>(cols - 1, -1));
    }
    int Memonized(const string& first, const string& second) {
        int length1 = first.length();
        int length2 = second.length();
        
        Init(length1 + 1, length2 + 1);  
        return Memonized(first, second, 0, 0);
    }

private:
    vector<int> costInsert, costDelete;              
    vector<vector<int>> costReplace, costTranspose;  
    vector<vector<int>> m_results;                   
    vector<vector<int>> m_Operations;                
    int Memonized(const string& first, const string& second, int index1, int index2) {
        if (m_results[index1][index2] == -1) {
            int length1 = first.length() - index1;
            int length2 = second.length() - index2;

            if (length1 == 0) {
                m_results[index1][index2] = length2 * costInsert[0];  // Costo basado en la longitud
            } else if (length2 == 0) {
                m_results[index1][index2] = length1 * costDelete[0];  // Costo basado en la longitud
            } else if (first[index1] == second[index2]) {
                m_results[index1][index2] = Memonized(first, second, index1 + 1, index2 + 1);
                m_Operations[index1][index2] = 0;
            } else {
                int insertCost = costInsert[second[index2] - 'a'] + Memonized(first, second, index1, index2 + 1);
                int deleteCost = costDelete[first[index1] - 'a'] + Memonized(first, second, index1 + 1, index2);
                int replaceCost = costReplace[first[index1] - 'a'][second[index2] - 'a'] + Memonized(first, second, index1 + 1, index2 + 1);
                int transposeCost = INT_MAX;
                if (index1 + 1 < first.length() && index2 + 1 < second.length() && first[index1] == second[index2 + 1] && first[index1 + 1] == second[index2]) {
                    transposeCost = costTranspose[first[index1] - 'a'][first[index1 + 1] - 'a'] + Memonized(first, second, index1 + 2, index2 + 2) + 1;
                }

                int minCost = min({insertCost, deleteCost, replaceCost, transposeCost});
                m_results[index1][index2] = minCost;

                // Guarda la operación correspondiente
                if (minCost == insertCost) m_Operations[index1][index2] = 1;
                else if (minCost == deleteCost) m_Operations[index1][index2] = 2;
                else if (minCost == replaceCost) m_Operations[index1][index2] = 3;
                else if (minCost == transposeCost) m_Operations[index1][index2] = 4;
            }
        }
        return m_results[index1][index2];
    }
};



// Función para obtener el uso máximo de memoria en kilobytes
long getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // Devuelve la memoria máxima usada en KB
}

int main() {
    cEditDistance editDistance;

    // Cargar los costos desde los archivos antes de calcular las distancias
    editDistance.loadCosts();

    ifstream file("dataset.csv");
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada." << endl;
        return 1;
    }

    ofstream outfile("cadenas_costos_resultado.csv", ios::app);
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida." << endl;
        return 1;
    }

    string line;
    bool first_line = outfile.tellp() == 0;
    if (first_line) {
        outfile << "Tipo de Caso,Cadena 1,Cadena 2,Distancia Levenshtein" << endl;
    }

    auto start_time = chrono::high_resolution_clock::now();

    int total_cost = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string tipoCaso, str1, str2;
        getline(ss, tipoCaso, ',');
        getline(ss, str1, ',');
        getline(ss, str2, ',');

        int distance = editDistance.Memonized(str1, str2);
        total_cost += distance;
        outfile << tipoCaso << "," << str1 << "," << str2 << "," << distance << endl;
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> execution_time = end_time - start_time;

    file.close();
    outfile.close();

    // Obtener la memoria máxima utilizada
    long memory_used = getMemoryUsage();

    // Mostrar resultados
    cout << "Resultados agregados en cadenas_costos_resultado.csv" << endl;
    cout << "Costo total de todas las distancias: " << total_cost << endl;
    cout << "Tiempo de ejecución: " << execution_time.count() << " segundos" << endl;
    cout << "Memoria máxima utilizada: " << memory_used << " KB" << endl;

    return 0;
}

