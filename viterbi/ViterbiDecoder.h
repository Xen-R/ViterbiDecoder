#pragma once
#include"ConvolutionalCode.h"
#include "Trellis.h"

class ViterbiDecoder //����� ��������
{
private:
	ConvolutionalCode code;
	Trellis trel; //������� ����

public:
	ViterbiDecoder(ConvolutionalCode& c);
	std::vector<uint16_t> decode(std::vector<uint16_t> enc); //������� �������������
};
