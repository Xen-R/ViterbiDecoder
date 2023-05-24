#include<iostream>
#include"ConvolutionalCode.h"
#include"ConvCoder.h"
#include"Trellis.h"
#include"ViterbiDecoder.h"

int main() {
	std::vector<uint16_t> poly; //������� ���� ���������� ��� ������ ������� ���������� ������ � �������� � ���������, ������������ �������� ����
	poly.push_back(5);
	poly.push_back(7);
	ConvolutionalCode c(2, poly); //��� ������� ��� ������ ��� � 2 ������� � �������� � ���������� 5, 7
	ConvCoder cc(c); //�������� �����-������
	ViterbiDecoder vd(c); //�������� ������-��������
	std::vector<double> p, bers; //������� ������������ ������ � ������ � �� ���
	for (int i = 0; i < 20; i++) { //������� ������������ ������ � ������
		p.push_back(0.05 * i);
	}
	for (int i = 0; i < p.size(); i++) {
		double ber = 0;
		int num = 10; //���������� �������������
		int len = 100; //������������ ����� ����� - 1
		for (int j = 0; j < num; j++) {
			uint16_t size = rand()%len + 1; // ��������� ����� �����
			std::vector<uint16_t> wrd;
			for (int k = 0; k < size; k++) { //��������� ��������� ��������������� �����
				wrd.push_back(rand() % 1);
			}
			std::vector<uint16_t> enc = cc.encode(wrd); //����������� �����
			std::vector<uint16_t> noised = enc; 
			for (int k = 0; k < enc.size(); k++) { //��������� ���� � ������ ����������� ������ � ������
				double err = (double)rand() / RAND_MAX;
				if (err < p.at(i)) {
					noised.at(k) = noised.at(k) ^ 1;
				}
			}
			std::vector<uint16_t> dec = vd.decode(noised); //�������������
			uint16_t count = 0;
			for (int k = 0; k < enc.size(); k++) { //��������� ����������� ����������� � �������������
				if (enc.at(k) != dec.at(k)) {
					count++;
				}
			}
			ber += (double)count / dec.size(); //���������� ������ �� ��� ��� �����
		}
		ber = ber / num; //���������� ������� ������ �� ���
		bers.push_back(ber);
	}
	std::cout << "\n P = [ "; //����� ����������� � �����, ������� ��� ����������� � Matlab/Octave
	for (int i = 0; i < p.size(); i++) {
		std::cout << p.at(i) << " ";
	}
	std::cout << "]\n BER = [ ";
	for (int i = 0; i < p.size(); i++) {
		std::cout << bers.at(i) << " ";
	}
	std::cout << "]\n";
	return 0;
}