#include "ConvCoder.h"

ConvCoder::ConvCoder(ConvolutionalCode &c)
{
	code = c;
}

std::vector<uint16_t> ConvCoder::encode(std::vector<uint16_t> word) //функция кодирования
{
	std::vector<uint16_t> result;
	std::vector<uint16_t> state;
	for (int i = 0; i < code.get_blocks_num(); i++) {
		state.push_back(0); //изначально в блоках регистра содержатся нули
	}
	for (int i = 0; i < word.size(); i++) {
		state.insert(state.begin(), word.at(i)); //подача бита слова на вход регистра
		std::vector<uint16_t> out = code.poly_res(state); //вычисление кодовых битов для текущего состояния регистра
		result.insert(result.end(), out.begin(), out.end());
		state.pop_back(); //сдвиг в регистре - выход бита из последнего блока
	}
	for (int i = 0; i < code.get_blocks_num()+1; i++) { //приведение слова в нулевое состояние посредством подачи нулей до момента заполнения всех блоков регистра нулями
		state.insert(state.begin(),0); //подача 0 на вход регистра
		std::vector<uint16_t> out = code.poly_res(state); //вычисление кодовых битов для текущего состояния регистра
		result.insert(result.end(), out.begin(), out.end());
		state.pop_back(); //сдвиг в регистре - выход бита из последнего блока
	}

	return result;
}
