#pragma once
#include<stdint.h>
#include<vector>
class ConvolutionalCode //класс для хранения информации о коде (количества блоков в регистре и полиномов)
{
private:
	uint16_t num_of_blocks; //количество блоков в регистре
	uint16_t num_of_output; //количество кодовых бит для 1 информационного бита
	std::vector<std::vector<uint16_t>> polynoms; //вектор полиномов кода
public:
	ConvolutionalCode();
	ConvolutionalCode(uint16_t n_blocks, std::vector<uint16_t>& poly);
	uint16_t get_blocks_num();
	uint16_t get_out_size();
	std::vector<uint16_t> poly_res(std::vector<uint16_t> state); //функция вычисления кодовых бит для регистра с заданным состоянием
};

