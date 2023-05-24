#include "ViterbiDecoder.h"

ViterbiDecoder::ViterbiDecoder(ConvolutionalCode& c)
{
	code = c;
	Trellis tmp(c); //�������� ������� ����
	trel = tmp;
}

std::vector<uint16_t> ViterbiDecoder::decode(std::vector<uint16_t> enc) //������� �������������
{
	uint16_t part_size = code.get_out_size(); //��������� ���������� ������� ��� ��� 1 ��������������� ����
	std::vector<uint16_t> curr_states; //������ ������� ��������� ��������� ��������
	std::vector<std::vector<uint16_t>> curr_res; //������, �������� ���������� �����
	std::vector<uint16_t> dst; //������, �������� ���������� �� ��������� ����� �� ����������������
	uint16_t num_states = pow(2, code.get_blocks_num());
	for (int i = 0; i < num_states; i++) {
		dst.push_back(0);
	}
	std::vector<std::vector<uint16_t>> wrds;
	std::vector<std::vector<uint16_t>> rts;
	wrds.resize(num_states);
	rts.resize(num_states);

	curr_states.push_back(0);//���������� � �������� ������ ����
	for (int i = 0; i < enc.size(); i += part_size) {
		std::vector<uint16_t> part;
		part.insert(part.begin(), enc.begin() + i, enc.begin() + i + part_size); //��������� ����� �����, ����������� � 1 ��������������� ����
		uint16_t num_part = to_num(part); //������� � �������� �����
		std::vector<std::vector<uint16_t>> tmp_res = trel.check_states(curr_states, num_part); //���������� �������� �������� � ����� ������ ���������� �������� �� ����� ����� �����,
																							   //���������� �� ��� ��������� ��������� ��������
		std::vector<uint16_t> copy = dst;
		for (int j = 0; j < dst.size(); j++) {
			if (tmp_res.at(3).at(j)) { //���� �� ������� ���� ��� �������� ���� � ��� ��������� � check_states

				dst.at(j) = tmp_res.at(0).at(j) + copy.at(tmp_res.at(1).at(j)); //���������� ������� ������������ ���������� ���� � ��������� i ����� ������������ ����������, ����������� 
																				//� check_states � ������������ ���������� �� ���������, ���� �� �������� ��� ������ ��� ����� �������� � check_states
				wrds.at(j).push_back(tmp_res.at(2).at(j)); //���������� �������, ����������� ������� ����, ��������������� ���������������� ���� � i
				rts.at(j).push_back(tmp_res.at(1).at(j)); //���������� �������, ����������� �������� �� ���������, ���� �� �������� � i ������ ��� ����� ��������
			}
			else { //���� �� ������� ���� �� ��� �������� ���� � ��� ���������, ��������� ������� "�������" ����������, ����� �� ������ ��� ����������
				dst.at(j) += 0;
				wrds.at(j).push_back(-1);
				rts.at(j).push_back(-1);
			}
		}

	}
	uint16_t idx_min = 0, dst_min = enc.size()+1; // ����� ��������� ���� ������, ���� ���� � ����������� ����������� �������� �� ��������� �����
	for (int j = 0; j < dst.size(); j++) { 
		if (dst.at(j) < dst_min) { //��������� "������ ������" � ���� ���� � ����, ��� �������� � ����������� �����������, ����������� ����� � ���������� �����, ���������� ��������� � 0 ���������
								   //������ ��� ��� ����� ��������� ���� ��-�� ���������� �������� � ������� ��������� ��� ����������� ��������� �����������
			dst_min = dst.at(j);
			idx_min = j; //������ ������� ���� � ����������� ����������� �� ��������� �����
		}
	}
	std::vector<uint16_t> dec;
	uint16_t step_back = idx_min, pos = wrds.at(idx_min).size()-1;
	for (int j = 0; j <= wrds.at(idx_min).size() - 1; j++) { //��������� �������������� ����� �� ����
		std::vector<uint16_t> tmp_v = to_vect(wrds.at(step_back).at(pos-j), code.get_out_size()); //������� ������� ���, ��������������� ��������� �������� �� ���� pos-j, � ����� �������
		dec.insert(dec.begin(), tmp_v.begin(), tmp_v.end());//������ ������� ���, ��������������� ��������� �������� �� ���� pos-j
		step_back = rts.at(step_back).at(pos-j);//������� � ���������, ����������������� ��������, ������ �� ���������� ����
	}
	return dec;
}
