#pragma once
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

class Actividad {

	public:
		Actividad(const string fileName);
		Actividad(const vector<pair<int, int>> actividades, const vector<int> beneficios);

		int BeneficioMaximoTD();
		int BeneficioMaximoBU();
		vector<int> BeneficioMaximoSolucion();
		
		double calcularTiempo();

	private:
		vector<pair<int, int>> actividades;
		vector<int> beneficios;
		vector<int> saltosOptimos;
		vector<int> memo;
		vector<int> repeticiones;

		int BeneficioMaximo(int i);
		void parsearDatos(const string filename);
		void calcularSaltos();
		void calcularRepeticiones(vector<int>& repeticiones, vector<pair<int, int>>& actividades);

		chrono::high_resolution_clock::time_point t1;
		chrono::high_resolution_clock::time_point t2;
};