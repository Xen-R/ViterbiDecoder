#include "ConvolutionalCode.h"

ConvolutionalCode::ConvolutionalCode()
{
	num_of_blocks = 0;
	num_of_output = 0;
}

ConvolutionalCode::ConvolutionalCode(uint16_t n_blocks, std::vector<uint16_t>& poly)
{
	num_of_blocks = n_blocks;
	num_of_output = poly.size(); //ADD POLY SIZE CHECK
	for (int i = 0; i < poly.size(); i++) { //перевод полиномов в форму двоичных векторов
		std::vector<uint16_t> p;
		uint16_t num = poly.at(i);
		while (num > 0) {
			p.push_back(num % 2);
			num = num >> 1;
		}
		polynoms.push_back(p);
	}
}

uint16_t ConvolutionalCode::get_blocks_num()
{
	return num_of_blocks;
}

uint16_t ConvolutionalCode::get_out_size()
{
	uint16_t ps = polynoms.size();
	return ps;
}

std::vector<uint16_t> ConvolutionalCode::poly_res(std::vector<uint16_t> state)
{
	std::vector<uint16_t> result;
	for (int i = 0; i < polynoms.size(); i++) {
		uint16_t tmp = 0;
		for (int j = 0; j < polynoms.at(i).size(); j++) {
			tmp = tmp ^ (polynoms.at(i).at(j) & state.at(j)); //вычисление результата подстановки бит в регистре (на входе и в блоках) в полиномы
		}
		result.push_back(tmp);
	}
	return result;
}
