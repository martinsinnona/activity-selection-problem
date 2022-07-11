#include "Actividades.h"
#include <fstream>
#include <ostream>
#include <algorithm>

Actividad::Actividad(const vector<pair<int, int>> actividades, const vector<int> beneficios) {

	this->actividades = actividades;
	this->beneficios = beneficios;
}

void Actividad::calcularRepeticiones(vector<int>& repeticiones, vector<pair<int, int>>& actividades) {
	
	repeticiones = vector<int>(actividades.size(), 0);

	for (int i = 1; i < actividades.size(); i++) {
		if (actividades[i].first == actividades[i - 1].first) repeticiones[i] = repeticiones[i - 1] + 1;
	}
}

void Actividad::parsearDatos(const string filename) {
	
	vector<int> datos;

	ifstream file(filename);
	if (file.is_open()) {

		string linea;
		getline(file, linea);

		vector<string> datosString;

		while (getline(file, linea, '	')) {

			int indice = linea.find("\n");
			if (indice != -1) {				// para aquellos carácteres que los separa un \n

				datosString.push_back(linea.substr(0, indice)); 

				string str = linea.substr(indice + 1, 10);
				if (str != "") datosString.push_back(str);

			}
			else {
				datosString.push_back(linea);
			}
		}

		for (string s : datosString) {
			datos.push_back(stoi(s));
		}

		file.close();
	}
	else {
		cout << "No se pudo abrir el archivo" << endl;
	}

	/*********************** EMPAQUETAMIENTO *****************************/

	repeticiones = vector<int>(datos.size() / 3, 0);

	for (int i = 0; i < datos.size(); i += 3) {

		actividades.push_back({ datos[i], datos[i + 1] });
		beneficios.push_back(datos[i + 2]);
	}
}

double Actividad::calcularTiempo() {
	t2 = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
}

void Actividad::calcularSaltos() {

	t1 = chrono::high_resolution_clock::now();

	calcularRepeticiones(repeticiones, actividades);
	saltosOptimos = vector<int>(actividades.size(), -1);

	for (int i = 0; i < actividades.size() - 1; i++) {			// O(n.log n)

		int indice = -1;
		int j = i + 1;
		int k = actividades.size() - 1;

		int mitad = (j + k) / 2;

		while (j <= k) {

			if (actividades[mitad].first > actividades[i].second) {

				indice = mitad;
				k = mitad - 1;

				if (actividades[indice].first == actividades[i].second + 1) {

					indice -= repeticiones[indice];
					break; 
				}

			}else {
				j = mitad + 1;
			}

			mitad = (j + k) / 2;
		}

		saltosOptimos[i] = indice;
	}

	//************************ VERIFICADOR (poner en TRUE : verifica los saltos) ***************************

	if (false) {

		for (int i = 0; i < actividades.size() - 1; i++) {			//O(n^2)

			if (i % 10000 == 0) cout << (float)i / actividades.size() * 100 << " %" << endl;
			for (int j = i + 1; j < actividades.size(); j++) {

				if (actividades[j].first > actividades[i].second) {
					if (saltosOptimos[i] != j) cout << "saltosOptimos[ " << i << " ] = " << saltosOptimos[i] << ", deberia ser: " << j << endl;
					break;
				}
			}
		}
	}

	//***************************************************************

	fflush;
}

Actividad::Actividad(const string filename) {
	
	parsearDatos(filename);
}

int Actividad::BeneficioMaximo(int i) {

	if (i == -1 || i >= beneficios.size()) return 0;
	if (memo[i] == -1) memo[i] = max(beneficios[i] + BeneficioMaximo(saltosOptimos[i]), BeneficioMaximo(i + 1));

	return memo[i]; 
}

int Actividad::BeneficioMaximoTD() {

	calcularSaltos();
	memo = vector<int>(actividades.size(), -1);

	return BeneficioMaximo(0);
}

int Actividad::BeneficioMaximoBU() {
	
	calcularSaltos();
	memo = vector<int>(actividades.size(), -1);

	for (int i = actividades.size() - 1; i >= 0; i--) BeneficioMaximo(i);
	return memo[0];
}

vector<int> Actividad::BeneficioMaximoSolucion() {

	vector<int> ret;

	int bmax = memo[0];
	int i = 0;

	while (i != -1 && i < memo.size() - 1) {
		
		if (memo[i] != memo[i + 1]) {

			ret.push_back(i);
			bmax -= beneficios[i];
			i = saltosOptimos[i];

		}else {
			i++;
		}
	}

	if (bmax > 0) ret.push_back(actividades.size() - 1);

	return ret;
}