#pragma once
#include"ConvolutionalCode.h"
#include "Trellis.h"

class ViterbiDecoder //класс декодера
{
private:
	ConvolutionalCode code;
	Trellis trel; //решетка кода

public:
	ViterbiDecoder(ConvolutionalCode& c);
	std::vector<uint16_t> decode(std::vector<uint16_t> enc); //функция декодирования
};
