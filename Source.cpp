#include <iostream>;
#include <cmath>;
#include <fstream>;
#include <string>;
#include <sstream>;
using namespace std;

unsigned long int tsifra(char q, char w) {
	unsigned long int a;
	w == 'I' ? a = 1 : w == 'V' ? a = 5 : w == 'X' ? a = 10 : w == 'L' ? a = 50 : w == 'C' ? a = 100 : w == 'D' ? a = 500 : w == 'M' ? a = 1000 : a = 0;
	if (q == '-') { a *= 1000; }
	if (q == '=') { a *= 1000000; }
	return a;
}

unsigned long int rim(string q, string w, string e) {
	unsigned long int a;
	//cout << "!!! (" << q << ") (" << w << ") (" << e << ")" << endl;
	if (e[0] == '4' && q != "- ") {					//вычетание
		if (q == "  " || q == "--"|| q == "==") {
			if (w == "IV") { a = 4; }
			else {
				if (w == "IX") { a = 9; }
				else {
					if (w == "XL") { a = 40; }
					else {
						if (w == "XC") { a = 90; }
						else {
							if (w == "CD") { a = 400; }
							else {
								if (w == "CM") { a = 900; }
								else {
									a = 0;
								}
							}
						}
					}
				}
			}
		}
		else {
			if (w == "MV" && q == " -") { a = 4000; }
			else {
				if (w == "MX" && q == " -") { a = 9000; }
				else {
					if (w == "MV" && q == " -=") { a = 4000000; }
					else {
						if (w == "MX" && q == "-=") { a = 9000000; }
						else {
							a = 0;
						}
					}
				}
			}
		}
		if (q == "--") { a *= 1000; }
		if (q == "==") { a *= 1000000; }
	}
	else {					//сложение
		a = tsifra(q[0], w[0]) * stoi(e);
	}
	return a;
}




int main() {
	setlocale(LC_ALL, "rus");

	while (true) {
		cout << "Введите римское число: ";  
		string lat; /*Ввели*/
		int err = 0, n = 1; // счётчик ошибок, кол-во повторяющихся элементов
		unsigned long int count = 0; // арабское число
		cin >> lat;
		char** spl = new char* [2];/*разбиение по символам с надчёркованием*/
		for (int i = 0; i < 2; i++)
			spl[i] = new char[lat.length()];
		for (int i = 0; i < 2; i++) { for (int j = 0; j < lat.length(); j++) { spl[i][j] = ' '; } }
		char q; // рассматриваемый элемент
		int z = 0; //счётчик для массива строк spl

		//заполнение spl
		for (int i = 0; i < lat.length(); i++) {
			q = lat[i];
			if (q != '-') {
				spl[1][z] = q;
			}
			else {
				if (q == '-' && lat[i + 1] == '-') { spl[0][z] = '='; spl[1][z] = lat[i + 2]; i += 2; }
				else { spl[0][z] = '-'; spl[1][z] = lat[i + 1]; i++; }
			}
			z++;
		}

		if (z > 3) { for (int i = 0; i < (z - 3); i++) { if (spl[1][i] + spl[1][i + 1] + spl[1][i + 2] + spl[1][i + 3] == spl[1][i] * 4) { err++; cout << ">>ERR AAAA<<\n"; } } } // исключаем 4 повторения

		//задание двумерного динамического массива
		string** chsl = new string * [4];
		for (int i = 0; i < 4; i++)
			chsl[i] = new string[z];

		int x = 0; //счётчик для массива строк chsl

		//заполнение chsl
		for (int i = 0; i < z; i++) {
			if (tsifra(spl[0][i], spl[1][i]) < tsifra(spl[0][i + 1], spl[1][i + 1]) && z - i - 1 > 0) { chsl[0][x] = spl[0][i]; chsl[0][x] += spl[0][i + 1]; chsl[1][x] += spl[1][i]; chsl[1][x] += spl[1][i + 1]; chsl[2][x] = "4 "; i++; } // пред < след  =>  вычитание
			else {
				if (spl[1][i] == spl[1][i + 1] && spl[0][i] == spl[0][i + 1] && spl[1][i] == spl[1][i + 2] && spl[0][i] == spl[0][i + 2] && z - i - 1 > 1) { chsl[0][x] = spl[0][i]; chsl[0][x] += spl[0][i + 1]; chsl[0][x] += spl[0][i + 2]; chsl[1][x] += spl[1][i]; chsl[1][x] += spl[1][i + 1]; chsl[1][x] += spl[1][i + 2]; chsl[2][x] = " 3 "; i += 2; } // 3 цифр подряд, счётчик
				else {
					if (spl[1][i] == spl[1][i + 1] && spl[0][i] == spl[0][i + 1] && (spl[1][i] != spl[1][i + 2] || spl[0][i] != spl[0][i + 2]) && z - i - 1 > 0) { chsl[0][x] = spl[0][i]; chsl[0][x] += spl[0][i + 1]; chsl[1][x] += spl[1][i]; chsl[1][x] += spl[1][i + 1]; chsl[2][x] = "2 ";  i++; } // 2 цифр подряд, счётчик
					else { chsl[0][x] = spl[0][i]; chsl[1][x] = spl[1][i]; chsl[2][x] = "1"; } //одиночная цифра
				}
			}
			x++;
		}

		//задание двумерного массива символов				[0] - надчёркивания; [1] - V,L,D,-V,-L,-D; [2] - I,X,C,M,-X,-C;
		char prs[4][9] = {
			{' ',	' ',	' ',	'-',	'-',	'-',	'=',	'=',	'='},
			{'V',	'L',	'D',	'V',	'L',	'D',	'V',	'L',	'D'},
			{' ',	' ',	' ',	' ',	'-',	'-',	'-',	'=',	'='},
			{'I',	'X',	'C',	'M',	'X',	'C',	'M',	'X',	'C'}
		};

		//задание двумерного массива чисел					[0] - кол-во вхождений V,L,D,-V,-L,-D; [1] - кол-во вычитаний I,X,C,M,-X,-C;
		int pri[2][9] = {
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0}
		};

		//кол-во вхождений V,L,D,-V,-L,-D
		for (int i = 0; i < z; i++) {
			switch (spl[0][i])
			{
			case ' ': {
				switch (spl[1][i])
				{
				case 'V': {
					pri[0][0]++;
					break;
				}
				case 'L': {
					pri[0][1]++;
					break;
				}
				case 'D': {
					pri[0][2]++;
					break;
				}
				}
				break;
			}
			case '-': {
				switch (spl[1][i])
				{
				case 'V': {
					pri[0][3]++;
					break;
				}
				case 'L': {
					pri[0][4]++;
					break;
				}
				case 'D': {
					pri[0][5]++;
					break;
				}
				}
				break;
			}
			case '=': {
				switch (spl[1][i])
				{
				case 'V': {
					pri[0][6]++;
					break;
				}
				case 'L': {
					pri[0][7]++;
					break;
				}
				case 'D': {
					pri[0][8]++;
					break;
				}
				break;
				}
			}
			break;
			}

		}
		for (int i = 0; i < 9; i++) { if (pri[0][i] > 1) { err++; cout << ">>ERR VV<<\n"; } }

		int vhod = 0;
		//кол-во вычитаний I,X,C,M,-X,-C,-M,--X,--C
		for (int i = 0; i < x; i++) {
			if (chsl[2][i] == "4 ") {
				switch (chsl[0][i][0])
				{
				case ' ': {
					switch (chsl[1][i][0])
					{
					case 'I': {
						if (pri[1][0] == 0) { pri[1][0] = i + 1; }
						break;
					}
					case 'X': {
						if (pri[1][1] == 0) { pri[1][1] = i + 1; }
						break;
					}
					case 'C': {
						if (pri[1][2] == 0) { pri[1][2] = i + 1; }
						break;
					}
					case 'M': {
						if (pri[1][3] == 0) { pri[1][3] = i + 1; }
						break;
					}
					}
					break;
				}
				case '-': {
					switch (chsl[1][i][0])
					{
					case 'X': {
						if (pri[1][1] == 0) { pri[1][4] = i + 1; }
						break;
					}
					case 'C': {
						if (pri[1][2] == 0) { pri[1][5] = i + 1; }
						break;
					}
					case 'M': {
						if (pri[1][3] == 0) { pri[1][6] = i + 1; }
						break;
					}
					}
					break;
				}
				case '=': {
					switch (chsl[1][i][0])
					{
					case 'X': {
						if (pri[1][1] == 0) { pri[1][7] = i + 1; }
						break;
					}
					case 'C': {
						if (pri[1][2] == 0) { pri[1][8] = i + 1; }
						break;
					}
					}
					break;
				}
				default:
					break;
				}
			}
		}
		int k; // счётчик типа i и j
		for (int i = 0; i < 9; i++) {
			int n4 = 0;
			if (pri[1][i] > 0) {
				for (int j = pri[1][i]; j <= x; j++) {
					for (int k = 0; k < chsl[1][j - 1].length(); k++) {
						if (prs[2][i] == chsl[0][j - 1][k] && prs[3][i] == chsl[1][j - 1][k]) { if (n4 == 0 && chsl[2][j - 1] == "4 " && k == 1) { n4++; } else { vhod++; } }
					}
				}
				if (vhod > 1) { err++; cout << ">>ERR IXI<<\n"; }
				vhod = 0;
			}
		}



		//собираем арабское число
		for (int i = 0; i < x; i++) {
			count += rim(chsl[0][i], chsl[1][i], chsl[2][i]);
			chsl[3][i] = to_string(rim(chsl[0][i], chsl[1][i], chsl[2][i]));
			if (rim(chsl[0][i], chsl[1][i], chsl[2][i]) == 0) { err++; cout << ">>ERR IC<<\n"; }
		}

		//проверка на IX X
		for (int i = 0; i < x - 1; i++) {
			if (stoul(chsl[3][i]) < stoul(chsl[3][i + 1])) { err++; cout << ">>ERR IXX<<\n"; }
			for (int j = i + 1; j < x; j++) { if (chsl[3][i] == chsl[3][j]) { err++; cout << ">>ERR IXIX<<\n"; } }
		}



		//выводы

		if (err > 0 || count == 0) { cout << "Ошибка перевода\n"; }
		else { cout << "Арабское число: " << count << endl; }

		for (int i = 0; i < 2; i++) { for (int j = 0; j < z; j++) { cout << spl[i][j] << " "; } cout << endl; } cout << endl;
		for (int i = 0; i < 4; i++) { for (int j = 0; j < x; j++) { cout << chsl[i][j] << " "; } cout << endl; } cout << endl;
		for (int i = 0; i < 2; i++) { for (int j = 0; j < 9; j++) { cout << pri[i][j] << " "; } cout << endl; }
		cout << endl << err;
		cout << endl << endl;
	}

	cout << "\n\n\n----------------------Работа корневой программы завершена----------------------";
	return 0;
}
