#pragma once
#include"ConvolutionalCode.h"
class ConvCoder //класс кодера
{
private:
	ConvolutionalCode code;

public:
	ConvCoder(ConvolutionalCode &c);
	std::vector<uint16_t> encode(std::vector<uint16_t> word); //функция кодирования

};

