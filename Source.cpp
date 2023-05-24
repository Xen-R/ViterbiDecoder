#include<iostream>
#include"ConvolutionalCode.h"
#include"ConvCoder.h"
#include"Trellis.h"
#include"ViterbiDecoder.h"

int main() {
	std::vector<uint16_t> poly; //задание кода происходит при помощи задания количества блоков в регистре и полиномов, определяющих выходные биты
	poly.push_back(5);
	poly.push_back(7);
	ConvolutionalCode c(2, poly); //для примера был выбран код с 2 блоками в регистре и полиномами 5, 7
	ConvCoder cc(c); //создание класс-кодера
	ViterbiDecoder vd(c); //создание класса-декодера
	std::vector<double> p, bers; //векторы вероятностей ошибки в канале и на бит
	for (int i = 0; i < 20; i++) { //задание вероятностей ошибки в канале
		p.push_back(0.05 * i);
	}
	for (int i = 0; i < p.size(); i++) {
		double ber = 0;
		int num = 10; //количество экспериментов
		int len = 100; //максимальная длина слова - 1
		for (int j = 0; j < num; j++) {
			uint16_t size = rand()%len + 1; // генерация длины слова
			std::vector<uint16_t> wrd;
			for (int k = 0; k < size; k++) { //случайная генерация информационного слова
				wrd.push_back(rand() % 1);
			}
			std::vector<uint16_t> enc = cc.encode(wrd); //кодирование слова
			std::vector<uint16_t> noised = enc; 
			for (int k = 0; k < enc.size(); k++) { //наложение шума с учетом вероятности ошибки в канале
				double err = (double)rand() / RAND_MAX;
				if (err < p.at(i)) {
					noised.at(k) = noised.at(k) ^ 1;
				}
			}
			std::vector<uint16_t> dec = vd.decode(noised); //декодирование
			uint16_t count = 0;
			for (int k = 0; k < enc.size(); k++) { //сравнение результатов кодирования и декодирования
				if (enc.at(k) != dec.at(k)) {
					count++;
				}
			}
			ber += (double)count / dec.size(); //вычисление ошибки на бит для слова
		}
		ber = ber / num; //вычисление средней ошибки на бит
		bers.push_back(ber);
	}
	std::cout << "\n P = [ "; //вывод результатов в форме, удобной для подстановки в Matlab/Octave
	for (int i = 0; i < p.size(); i++) {
		std::cout << p.at(i) << " ";
	}
	std::cout << "]\n BER = [ ";
	for (int i = 0; i < p.size(); i++) {
		std::cout << bers.at(i) << " ";
	}
	std::cout << "]\n";
	return 0;
}