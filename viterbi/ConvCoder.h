#pragma once
#include"ConvolutionalCode.h"
class ConvCoder //����� ������
{
private:
	ConvolutionalCode code;

public:
	ConvCoder(ConvolutionalCode &c);
	std::vector<uint16_t> encode(std::vector<uint16_t> word); //������� �����������

};

