#pragma once
#include<stdint.h>
#include<vector>
class ConvolutionalCode //����� ��� �������� ���������� � ���� (���������� ������ � �������� � ���������)
{
private:
	uint16_t num_of_blocks; //���������� ������ � ��������
	uint16_t num_of_output; //���������� ������� ��� ��� 1 ��������������� ����
	std::vector<std::vector<uint16_t>> polynoms; //������ ��������� ����
public:
	ConvolutionalCode();
	ConvolutionalCode(uint16_t n_blocks, std::vector<uint16_t>& poly);
	uint16_t get_blocks_num();
	uint16_t get_out_size();
	std::vector<uint16_t> poly_res(std::vector<uint16_t> state); //������� ���������� ������� ��� ��� �������� � �������� ����������
};

