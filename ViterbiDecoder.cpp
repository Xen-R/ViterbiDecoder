#include "ViterbiDecoder.h"

ViterbiDecoder::ViterbiDecoder(ConvolutionalCode& c)
{
	code = c;
	Trellis tmp(c); //создание решетки кода
	trel = tmp;
}

std::vector<uint16_t> ViterbiDecoder::decode(std::vector<uint16_t> enc) //функция декодирования
{
	uint16_t part_size = code.get_out_size(); //получение количества кодовых бит для 1 информационного бита
	std::vector<uint16_t> curr_states; //вектор текущих возможных состояний регистра
	std::vector<std::vector<uint16_t>> curr_res; //вектор, хранящий результаты шагов
	std::vector<uint16_t> dst; //вектор, хранящий расстояния от принятого слова до восстановленного
	uint16_t num_states = pow(2, code.get_blocks_num());
	for (int i = 0; i < num_states; i++) {
		dst.push_back(0);
	}
	std::vector<std::vector<uint16_t>> wrds;
	std::vector<std::vector<uint16_t>> rts;
	wrds.resize(num_states);
	rts.resize(num_states);

	curr_states.push_back(0);//изначально в регистре всегда нули
	for (int i = 0; i < enc.size(); i += part_size) {
		std::vector<uint16_t> part;
		part.insert(part.begin(), enc.begin() + i, enc.begin() + i + part_size); //выделение части слова, относящейся к 1 информационному биту
		uint16_t num_part = to_num(part); //перевод в числовую форму
		std::vector<std::vector<uint16_t>> tmp_res = trel.check_states(curr_states, num_part); //вычисление наиболее выгодных с точки зрения расстояния Хэмминга от части слова путей,
																							   //приводящих во все возможные состояния регистра
		std::vector<uint16_t> copy = dst;
		for (int j = 0; j < dst.size(); j++) {
			if (tmp_res.at(3).at(j)) { //если на текущем шаге был возможен путь в это состояние в check_states

				dst.at(j) = tmp_res.at(0).at(j) + copy.at(tmp_res.at(1).at(j)); //вычисление полного минимального расстояния пути в состояние i путем суммирования расстояния, полученного 
																				//в check_states и накопленного расстояния до состояния, путь из которого был выбран как самый выгодный в check_states
				wrds.at(j).push_back(tmp_res.at(2).at(j)); //заполнение вектора, содержащего кодовые биты, соответствующие рассматриваемому пути в i
				rts.at(j).push_back(tmp_res.at(1).at(j)); //заполнение вектора, содержащего указание на состояние, путь из которого в i выбран как самый выгодный
			}
			else { //если на текущем шаге не был возможен путь в это состояние, заполняем векторы "пустыми" значениями, чтобы их размер был одинаковым
				dst.at(j) += 0;
				wrds.at(j).push_back(-1);
				rts.at(j).push_back(-1);
			}
		}

	}
	uint16_t idx_min = 0, dst_min = enc.size()+1; // после получения всех данных, ищем путь с минимальным расстоянием Хэмминга до принятого слова
	for (int j = 0; j < dst.size(); j++) { 
		if (dst.at(j) < dst_min) { //поскольку "строго меньше" и цикл идет с нуля, при ситуации с минимальным расстоянием, относящимся сразу к нескольким путям, выбираются пришедшие в 0 состояние
								   //потому что это более вероятные пути из-за приведения регистра в нулевое состояние при проведдении процедуры кодирования
			dst_min = dst.at(j);
			idx_min = j; //запись индекса пути с минимальным расстоянием до принятого слова
		}
	}
	std::vector<uint16_t> dec;
	uint16_t step_back = idx_min, pos = wrds.at(idx_min).size()-1;
	for (int j = 0; j <= wrds.at(idx_min).size() - 1; j++) { //процедура восстановления слова по пути
		std::vector<uint16_t> tmp_v = to_vect(wrds.at(step_back).at(pos-j), code.get_out_size()); //перевод кодовых бит, соответствующих состоянию регистра на шаге pos-j, в форму вектора
		dec.insert(dec.begin(), tmp_v.begin(), tmp_v.end());//запись кодовых бит, соответствующих состоянию регистра на шаге pos-j
		step_back = rts.at(step_back).at(pos-j);//переход к состоянию, предшествовавшему текущему, исходя из найденного пути
	}
	return dec;
}
