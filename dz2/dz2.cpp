// Шубный Юрий Вариант 26 сдвиг двухбайтового текста на 5 бит вправо
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
int main(int argc, const char* argv[]) {
	if (argc != 3) {
		cerr << "Error: Use three parameters\n";
		return 5;
	}
	const string mode(argv[1]); // Режим работы
	const string file_name(argv[2]); // Имя файла
	ofstream file; // файл для вывода и записи
	ifstream file_2; // файл для ввода и чтения 
	if (mode == "encryption") {
		// Режим шифрование
		string txt;
		int password;
		cout << "Write the text, it will be encrypted: ";
		cin >> txt;
		if (txt.size() % 2 != 0) { // если длина текста нечетна, то дописываем  0
			txt.push_back(0u);
		}
		cout << "Password: ";
		cin >> password;
		srand(password);

		const char* data = txt.data();

		vector<char> bytes(txt.size());
		for (unsigned int i = 0; i < txt.size(); i++) {
			bytes[i] = data[i];
		}

		file.open(file_name, ios::binary);
		for (unsigned int i = 0; i < txt.size(); i += 2) { // делим текст на 2 отдельных байта 
			unsigned char b1 = bytes[i]; 
			unsigned char b2 = bytes[i + 1];

			unsigned int t = ((static_cast<unsigned int>(b1) << 8u) | (static_cast<unsigned int>(b2))) ^ rand(); // сдвигаем 1 байт на 8 бит влево, соединяем его со 2м байтом и делаем оперцию xor с рандомным числом 
			//запишем последние 5 символов, иначе они потеряются
			int last = t % 2; // поелдений символ текста 
			int prelast = (t % (int)pow(2, 2)) / 2; // предпоследний
			int last_2 = (t % (int)pow(2, 3)) / (int)pow(2, 2); // третий с конца 
			int last_3 = (t % (int)pow(2, 4)) / (int)pow(2, 3); // четвертый с конца 
			int last_4 = (t % (int)pow(2, 5)) / (int)pow(2, 4); // пятый с конца
			t = t >> 5; // делаем сдвиг текста на 5 бит вправо
			t = last_4 * pow(2, 15) + last_3 * pow(2, 14) + last_2 * pow(2, 13) + prelast * pow(2, 12) + last * pow(2, 11) + t; // в начало закодированного текста вставим 5 последних символа
			
			// зписываем в файл
			char p1 = t >> 8;
		    char p2 = t;
			file.write(&p1, sizeof(p1));
			file.write(&p2, sizeof(p2));
		}
		file.close();
	}
	else if (mode == "decryption") {
		// Режим расшифрования
		int password;
		cout << "Password: ";
		cin >> password;
		srand(password);

		file_2.open(file_name, ios::binary);

		vector<char> encryptedbytes((std::istreambuf_iterator<char>(file_2)), std::istreambuf_iterator<char>()); // зашифрованные байты

		file_2.close();

		vector<char> decrtyptedbytes(encryptedbytes.size());
		// выпоняем в обратном порядке действия блока "encryption" для расшифровки 
		for (int i = 0; i < encryptedbytes.size(); i += 2) {
			unsigned char byte1 = encryptedbytes[i];
			unsigned char byte2 = encryptedbytes[i + 1];

			unsigned int t = (static_cast<unsigned int>(byte1) << 8u) | (static_cast<unsigned int>(byte2));
			int first_t = t / (int)pow(2, 15);
			int second_t = (t / (int)pow(2, 14)) % 2;
			int third_t = (t / (int)pow(2, 13)) % 2;
			int fourth_t = (t / (int)pow(2, 12)) % 2;
			int fivth_t = (t / (int)pow(2, 11)) % 2;
			t = t << 5;
			t = t + first_t * (int)pow(2, 4) + second_t * (int)pow(2, 3) + third_t * (int)pow(2, 2) + fourth_t * 2 + fivth_t;

			t = t ^ rand();

			unsigned char p1 = t >> 8;
			unsigned char p2 = t;
			decrtyptedbytes[i] = p1;
			decrtyptedbytes[i + 1] = p2;
		}
		// вывод расшифрованных данных
		for (int i = 0; i < decrtyptedbytes.size(); i++) {
			cout << decrtyptedbytes[i];
		}
	}
	return 0;
}