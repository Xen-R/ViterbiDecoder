#include "Trellis.h"
#include<iostream>
#include <algorithm>

Trellis::Trellis()
{
	num_of_bits = 0;
}

Trellis::Trellis(ConvolutionalCode code)
{
	num_of_bits = code.get_blocks_num() + 1; //получение количества бит в состоянии регистра: количество блоков + входной бит
	out_size = code.get_out_size(); //получение размера выходной последовательности для 1 информационного бита
	uint16_t variants = pow(2, num_of_bits - 1);
	for (int i = 0; i < variants; i++) { //проход по всем вариантам поледовательностей бит в регистре (исключая входной)
		std::vector<uint16_t> new_edge; 
		uint16_t plus_zero = (i << 1) & ((uint16_t)pow(2, num_of_bits) - 1); //вычисление состояния регистра при подаче на вход 0, 1, вышедшие из регистра, отсекаются операцией И
		uint16_t plus_one = plus_zero + 1;//вычисление состояния регистра при подаче на вход 1
		std::vector<uint16_t> v1 = to_vect(plus_zero, num_of_bits); //перевод состояний регистра в векторную форму
		std::vector<uint16_t> v2 = to_vect(plus_one, num_of_bits);
		new_edge.push_back(plus_zero % variants); //запись связей между наполнениями регистра, т.е. 00, подача 0 на вход - 00
		new_edge.push_back(plus_one % variants);
		edges.push_back(new_edge);
		std::vector<uint16_t> new_out;
		std::vector<uint16_t> res_zero = code.poly_res(to_vect(plus_zero, num_of_bits)); //получение выходных последовательностей для вариантов состояния при 0 и при 1 на входе
		std::vector<uint16_t> res_one = code.poly_res(to_vect(plus_one, num_of_bits));
		new_out.push_back(to_num(res_zero));
		new_out.push_back(to_num(res_one));
		outputs.push_back(new_out);
	}
}
std::vector<std::vector<uint16_t>> Trellis::check_states(std::vector<uint16_t> &states, uint16_t part)//функция декодирования
{
	std::vector<uint16_t> dst;//расстояния перехода в положения регистра
	std::vector<uint16_t> wrds;//выходные последовательности переходов в положения регистра
	std::vector<uint16_t> rts;//положение, из которого был осуществлен переход в это с соотв. выходным вектором и соотв. расстоянием
	std::vector<uint16_t> used;//получены ли были данные для положения на этом шаге? 1- да, 0 - нет
	wrds.resize(pow(2, num_of_bits - 1));
	uint16_t norm = out_size;
	for (int i = 0; i < pow(2, num_of_bits - 1); i++) {
		dst.push_back(0);
		rts.push_back(0);
		used.push_back(0);
	}
	for (int i = 0; i < states.size(); i++) { //проход по возможным положениям регистра на входе
		std::vector<uint16_t> dist; //расстояния от части слова до возможных выходных последовательностей при подаче на вход регистра:
		uint16_t dist0 = distance(part, outputs.at(i).at(0)); //0
		uint16_t dist1 = distance(part, outputs.at(i).at(1)); //1
		dist.push_back(dist0);
		dist.push_back(dist1);
		for (int j = 0; j <= 1; j++) { //для полученных расстояний
			if ((dst.at(edges.at(states.at(i)).at(j)) == 0 && !used.at(edges.at(states.at(i)).at(j))) || dst.at(edges.at(states.at(i)).at(j)) > dist.at(j)) {
				//сравнение с текущим записанным расстоянием до положения регистра, в который может привести подача соотв. бита на вход																					
				dst.at(edges.at(states.at(i)).at(j)) = dist.at(j); //запись расстояния, если оно меньше или не было записано до этого шага
				rts.at(edges.at(states.at(i)).at(j)) = i; //запись положения, откуда пришли в положение, для которого записано расстояние
				wrds.at(edges.at(states.at(i)).at(j)) = outputs.at(i).at(j); //запись выходной последовательности, соотв. полученному состоянию регистра
				if (dist.at(j) < norm) { //нахождение минимального расстояния среди всех с целью использования для нормализации
					norm = dist.at(j);
				}
				used.at(edges.at(states.at(i)).at(j)) = 1; //отметка о том, что для этого положения были записаны данные
			}
		}
	}
	uint16_t sz = states.size(); 
	if (sz < pow(2, num_of_bits - 1)) { //если на вход были поданы не все возможные положения бит в регистре, дополняем массив
		for (int i = 0; i < sz; i++) {
			states.push_back(edges.at(states.at(i)).at(0));
			states.push_back(edges.at(states.at(i)).at(1));
		}
		std::sort(states.begin(), states.end());
		for (int i = 0; i < states.size() - 1; i++) {
			if (states.at(i + 1) == states.at(i)) {
				states.erase(states.begin() + i);
				i--;
			}
		}
	}
	for (int i = 0; i < dst.size(); i++) {//нормализация полученных на текущем шаге минимальных расстояний до положений регистра
		if (used.at(i)) {
			dst.at(i) = dst.at(i) - norm;
		}
	}
	std::vector<std::vector<uint16_t>> result;
	result.push_back(dst);
	result.push_back(rts);
	result.push_back(wrds);
	result.push_back(used);
	return result;
}
std::vector<uint16_t> to_vect(uint16_t num, uint16_t size) {//функция перевода числа в вектор его двоичного представления заданной длины
	std::vector<uint16_t> res;
	for (int i = 0; i < size; i++) {
		res.push_back(num % 2);//младший разряд записывается в 0 индекс и т.д.
		num = num >> 1;
	}

	return res;
}
uint16_t to_num(std::vector<uint16_t> vect) { //функция перевода вектора в числовую форму
	uint16_t res = 0;
	for (int i = vect.size() - 1; i >= 0; i--) {
		res = res << 1;
		res += vect.at(i); //нулевому индексу соответствует младший разряд
	}

	return res;

}

uint16_t distance(uint16_t first, uint16_t second) //функция нахождения расстояния Хэмминга между двоичными представлениями двух чисел
{
	uint16_t tmp = first ^ second; //XOR чисел с целью оставить единицы в двоичной записи только на позициях, в которых 1 есть только у одного из двух чисел
	uint16_t weight = 0;
	while (tmp > 0) {
		if (tmp % 2 == 1) {
			weight++;//вычисление количества единиц в двоичной записи XOR'а чисел - вычисление расстояния Хэмминга
		}
		tmp = tmp >> 1;
	}
	return weight;
}


