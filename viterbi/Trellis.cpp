#include "Trellis.h"
#include<iostream>
#include <algorithm>

Trellis::Trellis()
{
	num_of_bits = 0;
}

Trellis::Trellis(ConvolutionalCode code)
{
	num_of_bits = code.get_blocks_num() + 1; //��������� ���������� ��� � ��������� ��������: ���������� ������ + ������� ���
	out_size = code.get_out_size(); //��������� ������� �������� ������������������ ��� 1 ��������������� ����
	uint16_t variants = pow(2, num_of_bits - 1);
	for (int i = 0; i < variants; i++) { //������ �� ���� ��������� ������������������ ��� � �������� (�������� �������)
		std::vector<uint16_t> new_edge; 
		uint16_t plus_zero = (i << 1) & ((uint16_t)pow(2, num_of_bits) - 1); //���������� ��������� �������� ��� ������ �� ���� 0, 1, �������� �� ��������, ���������� ��������� �
		uint16_t plus_one = plus_zero + 1;//���������� ��������� �������� ��� ������ �� ���� 1
		std::vector<uint16_t> v1 = to_vect(plus_zero, num_of_bits); //������� ��������� �������� � ��������� �����
		std::vector<uint16_t> v2 = to_vect(plus_one, num_of_bits);
		new_edge.push_back(plus_zero % variants); //������ ������ ����� ������������ ��������, �.�. 00, ������ 0 �� ���� - 00
		new_edge.push_back(plus_one % variants);
		edges.push_back(new_edge);
		std::vector<uint16_t> new_out;
		std::vector<uint16_t> res_zero = code.poly_res(to_vect(plus_zero, num_of_bits)); //��������� �������� ������������������� ��� ��������� ��������� ��� 0 � ��� 1 �� �����
		std::vector<uint16_t> res_one = code.poly_res(to_vect(plus_one, num_of_bits));
		new_out.push_back(to_num(res_zero));
		new_out.push_back(to_num(res_one));
		outputs.push_back(new_out);
	}
}
std::vector<std::vector<uint16_t>> Trellis::check_states(std::vector<uint16_t> &states, uint16_t part)//������� �������������
{
	std::vector<uint16_t> dst;//���������� �������� � ��������� ��������
	std::vector<uint16_t> wrds;//�������� ������������������ ��������� � ��������� ��������
	std::vector<uint16_t> rts;//���������, �� �������� ��� ����������� ������� � ��� � �����. �������� �������� � �����. �����������
	std::vector<uint16_t> used;//�������� �� ���� ������ ��� ��������� �� ���� ����? 1- ��, 0 - ���
	wrds.resize(pow(2, num_of_bits - 1));
	uint16_t norm = out_size;
	for (int i = 0; i < pow(2, num_of_bits - 1); i++) {
		dst.push_back(0);
		rts.push_back(0);
		used.push_back(0);
	}
	for (int i = 0; i < states.size(); i++) { //������ �� ��������� ���������� �������� �� �����
		std::vector<uint16_t> dist; //���������� �� ����� ����� �� ��������� �������� ������������������� ��� ������ �� ���� ��������:
		uint16_t dist0 = distance(part, outputs.at(i).at(0)); //0
		uint16_t dist1 = distance(part, outputs.at(i).at(1)); //1
		dist.push_back(dist0);
		dist.push_back(dist1);
		for (int j = 0; j <= 1; j++) { //��� ���������� ����������
			if ((dst.at(edges.at(states.at(i)).at(j)) == 0 && !used.at(edges.at(states.at(i)).at(j))) || dst.at(edges.at(states.at(i)).at(j)) > dist.at(j)) {
				//��������� � ������� ���������� ����������� �� ��������� ��������, � ������� ����� �������� ������ �����. ���� �� ����																					
				dst.at(edges.at(states.at(i)).at(j)) = dist.at(j); //������ ����������, ���� ��� ������ ��� �� ���� �������� �� ����� ����
				rts.at(edges.at(states.at(i)).at(j)) = i; //������ ���������, ������ ������ � ���������, ��� �������� �������� ����������
				wrds.at(edges.at(states.at(i)).at(j)) = outputs.at(i).at(j); //������ �������� ������������������, �����. ����������� ��������� ��������
				if (dist.at(j) < norm) { //���������� ������������ ���������� ����� ���� � ����� ������������� ��� ������������
					norm = dist.at(j);
				}
				used.at(edges.at(states.at(i)).at(j)) = 1; //������� � ���, ��� ��� ����� ��������� ���� �������� ������
			}
		}
	}
	uint16_t sz = states.size(); 
	if (sz < pow(2, num_of_bits - 1)) { //���� �� ���� ���� ������ �� ��� ��������� ��������� ��� � ��������, ��������� ������
		for (int i = 0; i < sz; i++) {
			states.push_back(edges.at(states.at(i)).at(0));
			states.push_back(edges.at(states.at(i)).at(1));
		}
		std::sort(states.begin(), states.end());
		for (int i = 0; i < states.size() - 1; i++) {
			if (states.at(i + 1) == states.at(i)) {
				states.erase(states.begin() + i);
				i--;
			}
		}
	}
	for (int i = 0; i < dst.size(); i++) {//������������ ���������� �� ������� ���� ����������� ���������� �� ��������� ��������
		if (used.at(i)) {
			dst.at(i) = dst.at(i) - norm;
		}
	}
	std::vector<std::vector<uint16_t>> result;
	result.push_back(dst);
	result.push_back(rts);
	result.push_back(wrds);
	result.push_back(used);
	return result;
}
std::vector<uint16_t> to_vect(uint16_t num, uint16_t size) {//������� �������� ����� � ������ ��� ��������� ������������� �������� �����
	std::vector<uint16_t> res;
	for (int i = 0; i < size; i++) {
		res.push_back(num % 2);//������� ������ ������������ � 0 ������ � �.�.
		num = num >> 1;
	}

	return res;
}
uint16_t to_num(std::vector<uint16_t> vect) { //������� �������� ������� � �������� �����
	uint16_t res = 0;
	for (int i = vect.size() - 1; i >= 0; i--) {
		res = res << 1;
		res += vect.at(i); //�������� ������� ������������� ������� ������
	}

	return res;

}

uint16_t distance(uint16_t first, uint16_t second) //������� ���������� ���������� �������� ����� ��������� ��������������� ���� �����
{
	uint16_t tmp = first ^ second; //XOR ����� � ����� �������� ������� � �������� ������ ������ �� ��������, � ������� 1 ���� ������ � ������ �� ���� �����
	uint16_t weight = 0;
	while (tmp > 0) {
		if (tmp % 2 == 1) {
			weight++;//���������� ���������� ������ � �������� ������ XOR'� ����� - ���������� ���������� ��������
		}
		tmp = tmp >> 1;
	}
	return weight;
}


