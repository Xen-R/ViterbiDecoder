#include "ConvCoder.h"

ConvCoder::ConvCoder(ConvolutionalCode &c)
{
	code = c;
}

std::vector<uint16_t> ConvCoder::encode(std::vector<uint16_t> word) //������� �����������
{
	std::vector<uint16_t> result;
	std::vector<uint16_t> state;
	for (int i = 0; i < code.get_blocks_num(); i++) {
		state.push_back(0); //���������� � ������ �������� ���������� ����
	}
	for (int i = 0; i < word.size(); i++) {
		state.insert(state.begin(), word.at(i)); //������ ���� ����� �� ���� ��������
		std::vector<uint16_t> out = code.poly_res(state); //���������� ������� ����� ��� �������� ��������� ��������
		result.insert(result.end(), out.begin(), out.end());
		state.pop_back(); //����� � �������� - ����� ���� �� ���������� �����
	}
	for (int i = 0; i < code.get_blocks_num()+1; i++) { //���������� ����� � ������� ��������� ����������� ������ ����� �� ������� ���������� ���� ������ �������� ������
		state.insert(state.begin(),0); //������ 0 �� ���� ��������
		std::vector<uint16_t> out = code.poly_res(state); //���������� ������� ����� ��� �������� ��������� ��������
		result.insert(result.end(), out.begin(), out.end());
		state.pop_back(); //����� � �������� - ����� ���� �� ���������� �����
	}

	return result;
}
