﻿#pragma once
#include"ConvolutionalCode.h"
class Trellis //класс хранения решетки кода
{
private:
	//ВАЖНО: состояния в двоичной форме представлены так, что младший бит - бит первого блока регистра, т.е. для регистра из 3 блоков, где 1->1->0, числовая форма будет 011, т.е. 3
	uint16_t num_of_bits;//количество бит в состоянии
	uint16_t out_size;//количество кодовых бит в выходной последовательности
	std::vector<std::vector<uint16_t>> outputs; //выходные последовательности для состояний при подаче 0 или 1 на вход регистра
	//индекс вектора соответствует наполнению регистра в форме двоичного числа. В 0 элементе хранится выходная последовательность в форме двоичного числа
	// при подаче 0 на вход регистра, в 1 элементе - при подаче 1
	std::vector<std::vector<uint16_t>> edges;//переходы между состояниями регистра при подаче 0 или 1 на вход регистра
	//индекс вектора соответствует наполнению регистра в форме двоичного числа. В 0 элементе хранится наполнение регистра на следующем шаге
	// при подаче 0 на вход регистра, в 1 элементе - при подаче 1
public:
	Trellis();
	Trellis(ConvolutionalCode code);
	std::vector<std::vector<uint16_t>> check_states(std::vector<uint16_t> &states, uint16_t part); //функция вычисления расстояний и путей для кодовых бит, соотв. 1 информационному биту
};
std::vector<uint16_t> to_vect(uint16_t num, uint16_t size); //функция перевода числа в двоичный вектор заданного размера
uint16_t to_num(std::vector<uint16_t> vect); //функция перевода вектора в число
uint16_t distance(uint16_t first, uint16_t second); //функция вычисления расстояния Хэмминга между двоичными представлениями 2 чисел
