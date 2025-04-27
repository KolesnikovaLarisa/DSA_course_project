/*
Предметная область: регистрация постояльцев в гостинице
Метод хеширования: открытое хеширование
Метод сортировки: подсчётом
Вид списка: циклический однонаправленный
Метод обхода дерева: прямой
Алгоритм поиска слова в тексте: Боуера и Мура (БМ)
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <limits.h>
#include <ios>
#include <vector>

//#define NOMINMAX
#ifdef max
#undef max
#endif

using namespace std;

//--------------------------------список постояльцев-------------------------------------//
//структура, хранящая данные о постояльцах (однонаправленнный список)
struct Guest {
	string passport; // первичный ключ, "NNNN - NNNNNN", где N – цифры
	string full_name; // ФИО
	int year_of_birth; // год рождения 
	string address; // адрес
	string purpose; // цель прибытия
	Guest* next;

	Guest() {
		next = NULL;
	}

	Guest(string pass, string name, int y, string addr, string purp) {
		passport = pass;
		full_name = name;
		year_of_birth = y;
		address = addr;
		purpose = purp;
		next = NULL;
	}
};

//структура, хранящая данные о заерегистрированных вселениях (ононаправленный циклический список)
struct TrafficData {
	string passport; //"NNNN - NNNNNN", где N – цифры
	string number; //"ANNN"
	string checkin; //дата заселения
	string checkout; //дата выселения
	TrafficData* next;

	TrafficData();

	TrafficData(string p, string num, string in, string out) {
		passport = p;
		number = num;
		checkin = in;
		checkout = out;
		next = NULL;
	}
};

//функция добавления постояльца в список гостей
void push_guest(Guest** head, Guest* g) {
	Guest* cur = *head;
	Guest* p = g;
	if (*head == NULL) {
		*head = p;
	}
	else {
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = p;
	}
	p->next = NULL;
}

//функция удаления постояльца из списка гостей
void pop_guest(Guest** head, string pass) {
	Guest* cur = *head;
	if (cur->passport == pass) {
		*head = cur->next;
		delete cur;
		return;
	}
	while (cur->next != NULL) {
		if (cur->next->passport == pass) {
			Guest* p = cur->next;
			cur->next = (cur->next)->next;
			delete p;
			return;
		}
		cur = cur->next;
	}
}

//функция вывода переданного списка постояльцев
void print_guests(Guest* head) {
	Guest* p = head;
	if (p == NULL) {
		cout << "Список постояльцев пуст!\n";
		return;
	}
	while (p->next != NULL) {
		cout << p->passport << '\t' << p->full_name << "\t\t" << p->year_of_birth << "\t\t" << p->address << "\t\t" << p->purpose << "\n";
		p = p->next;
	}
	cout << p->passport << '\t' << p->full_name << "\t\t" << p->year_of_birth << "\t\t" << p->address << "\t\t" << p->purpose << "\n";
}

//функция вычисления размера списка постояльцев
int size_guests_list(Guest* head) {
	Guest* p = head;
	int s = 0;
	if (p == NULL) return 0;
	while (p->next != NULL) {
		s++;
		p = p->next;
	}
	return s + 1;
}

//функция для очистки переданного списка постояльцев
void free_list(Guest** head) {
	Guest* cur = *head;
	if (cur == NULL) return;
	if (cur->next == NULL) {
		delete cur;
		*head = NULL;
		return;
	}
	while (cur != nullptr) {
		Guest* next = cur->next;
		delete cur;
		cur = next;
	}
}

//функция проверки корректности введённого номера паспорта
void checkPassport(string& passport) {
	cin >> passport;
	bool f;
	while (true) {
		f = true;
		if (passport.size() != 11) f = false;  
		else {
			for (int i = 0; i < 4; ++i) {
				if (passport[i] < '0' || '9' < passport[i]) {
					f = false;
					break;
				}
			}
			if (passport[4] != '-') f = false;
			for (int i = 5; i < 11; ++i) {
				if (passport[i] < '0' || '9' < passport[i]) {
					f = false;
					break;
				}
			}
		}
		
		if (f) return;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> passport;
	}
}

//функция проверки корректности введённого года рождения
void checkYear(int& n) {
	cin >> n;
	while (!cin.good() || n <  1902 || 2024 < n || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> n;
	}
}

//функция проверки наличия записи в списке зарегистрированных вселений 
bool inTrafficData(TrafficData* head, string s) {
	if ('0' <= s[0] && s[0] <= '9') {
		if (head == nullptr) {
			return false;
		}
		TrafficData* cur = head;
		do {
			if (cur->passport == s) {
				return true;
			}
			cur = cur->next;
		} while (cur != head);
		return false;
	}
	else {
		if (head == nullptr) {
			return false;
		}
		TrafficData* cur = head;
		do {
			if (cur->number == s) {
				return true;
			}
			cur = cur->next;
		} while (cur != head);
		return false;
		
	}	
}

//-------------------------хеш-таблица с данными о постояльцах----------------------------//
//структура для работы с данными о постояльцах (реализация хеш-таблицы)
struct hashMap {
	int capacity;
	int size;
	Guest* arr[100];

	hashMap() {
		capacity = 100;
		size = 0;
		for (int i = 0; i < 100; ++i)
			arr[i] = nullptr;
	}
	~hashMap() {
		for (int i = 0; i < 100; ++i) {
			free_list(&arr[i]);
		}
	}

	int hashFunction(string);
	void insert(Guest*);
	void erase(string);
	bool inHashTable(string);
	Guest* searchByPassport(string);
	Guest* searchByName(string);
	void printTable();
};

//метод вычисления хеша
int hashMap::hashFunction(string key) {
	int hash = 0;
	for (int i = 0; i < 4; ++i) {
		hash += int(key[i]);
	}
	for (int i = 5; i < 11; ++i) {
		hash += int(key[i]);
	}
	return hash % capacity;
}

//метод добавления нового элемента в хеш-таблицу
void hashMap::insert(Guest* g) {
	int index = hashFunction(g->passport);
	push_guest(&arr[index], g);
	size++;
}

//метод удаления элемента из хеш-таблицы
void hashMap::erase(string passport) {
	int index = hashFunction(passport);
	pop_guest(&arr[index], passport);
	size--;
}

//метод проверки наличия элемента в хеш-таблице
bool hashMap::inHashTable(string passport) {
	int index = hashFunction(passport);
	Guest* ptr = arr[index];
	while (ptr != NULL) {
		if (ptr->passport == passport) return true;
		ptr = ptr->next;
	}
	return false;
}

//метод поиска в хеш-таблице по номеру паспорта постояльца
Guest* hashMap::searchByPassport(string passport) {
	Guest* ans = new Guest;
	for (int i = 0; i < capacity; ++i) {
		Guest* p = arr[i];
		while (p != NULL) {
			if (arr[i]->passport == passport) {
				ans = p;
				return ans;
			}
			p = p->next;
		}
	}
}

//метод поиска в хеш-таблице по ФИО постояльца
Guest* hashMap::searchByName(string name) {
	Guest* ans = NULL;
	for (int i = 0; i < capacity; ++i) {
		Guest* p = arr[i];
		while (p != NULL) {
			if (p->full_name == name) {
				push_guest(&ans, p);
			}
			p = p->next;
		}
	}
	return ans;
}

//метод вывода хеш-таблицы на экран
void hashMap::printTable() {
	if (size == 0) {
		cout << "Список зарегистрированных пуст!\n";
		return;
	}
	cout << "Номер паспорта" << '\t' << "ФИО" << "\t\t" << "Год рождения" << '\t' << "Адрес" << "\t\t" << "Цель прибытия" << '\n';
	for (int i = 0; i < capacity; ++i) {
		if (size_guests_list(arr[i]) != 0) {
			print_guests(arr[i]);
		}
	}
}

//------------------------------действия с постояльцами-----------------------------------//

//функция для регистрации нового постояльца
void registration(hashMap* table) {
	cout << "Заполните данные о постояльце: \n";
	string passport, name, address, purpose;
	int year;
	cout << "Номер паспорта (строка формата NNNN-NNNNNN, где N - цифры): ";
	checkPassport(passport);
	if (table->inHashTable(passport)) {
		cout << "Этот человек уже есть в базе!\n";
		return;
	}
	cout << "ФИО: ";
	cin.ignore();
	getline(cin, name);
	cout << "Год рождения: ";
	checkYear(year);
	cout << "Адрес: ";
	//cin.ignore();
	getline(cin, address);
	cout << "Цель прибытия: ";
	//cin.ignore();
	getline(cin, purpose);
	
	Guest* g = new Guest(passport, name, year, address, purpose);
	table->insert(g);
}

//функция для удаления постояльца
void deliting(hashMap* table, TrafficData* data) {
	if (table->size == 0) {
		cout << "Список зарегистрированных пуст!\n";
		return;
	}
	string passport;
	cout << "Введите номер паспорта постояльца: ";
	checkPassport(passport);
	cin.ignore();

	if (!table->inHashTable(passport)) {
		cout << "\nПостояльца с таким номером паспорта нет в списке!\n";
		return;
	}
	 
	if (inTrafficData(data, passport)) {
		cout << "\nУдаление невозмжно! Сначала надо зарегистрировать выселение постояльца!\n";
		return;
	}
	table->erase(passport);
	cout << "\nДанные о постояльце удалены!\n";
}

//функция поиска постояльца по номеру паспорта
void searchPassport(hashMap* table) {
	cout << "Введите номер паспорта, по которому хотите посмотреть информацию: ";
	string passport;
	checkPassport(passport);

	if (table->inHashTable(passport)) {
		Guest* ans = table->searchByPassport(passport);
		cout << "Номер паспорта" << '\t' << "ФИО" << "\t\t" << "Год рождения" << '\t' << "Адрес" << "\t\t" << "Цель прибытия" << '\n';
		cout << ans->passport << '\t' << ans->full_name << "\t\t" << ans->year_of_birth << "\t\t" << ans->address << "\t\t" << ans->purpose << "\n";
	}
	else {
		cout << "В списке постояльцев нет человека с таким номером паспорта!\n";
	}
}

//функция поиска постояльца по ФИО
void searchFIO(hashMap* table) {
	cout << "Введите ФИО постояльца, информацию о котором хотите посмотреть: ";
	//cin.ignore();
	string name;
	getline(cin, name);

	Guest* ans = table->searchByName(name);
	if (ans == NULL) cout << "В списке постояльцев нет человека с таким ФИО!\n";
	else {
		cout << "Номер паспорта" << '\t' << "ФИО" << "\t\t" << "Год рождения" << '\t' << "Адрес" << "\t\t" << "Цель прибытия" << '\n';
		print_guests(ans);
	}
	free_list(&ans);
}

//функция вывода меню для работы с постояльцами
void printGuestMenu() {
	system("cls");
	cout << "Выберите действие: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | Регистрация нового постояльца     |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | Удаление данных о постояльце      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | Просмотр всех зарегистрированных  |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 4. | Очистка данных о постояльцах      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 5. | Поиск по номеру паспорта          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 6. | Поиск по ФИО                      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | Вернуться в главное меню          |\n";
	cout << "+----+-----------------------------------+\n";
}

//функция для реализации работы меню постояльцев
void guestMenu(hashMap* table, TrafficData* data) {
	char menu_item;
	while (true) {
		printGuestMenu();
		cout << "Ваш выбор: ";
		cin >> menu_item;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("cls");
		switch (menu_item) {
		case '1':
			registration(table);
			system("pause");
			break;
		case '2':
			deliting(table, data);
			system("pause");
			break;
		case '3':
			table->printTable();
			system("pause");
			break;
		case '4':
			if (data == nullptr) {
				table->~hashMap();
				table->size = 0;
				cout << "Данные о постояльцах очищены!\n";
			}
			else {
				cout << "\nНевозможно очистить данные о всех постояльцах, так как есть зарегистрированные!\n";
			}
			system("pause");
			break;
		case '5':
			searchPassport(table);
			system("pause");
			break;
		case '6':
			searchFIO(table);
			system("pause");
			break;
		case '0':
			return;
		default:
			cout << "Некорректный ввод!\n";
			system("pause");
		}
	}
}

//------------------------действия с гостинничными номерами-------------------------------//
//структура для работы с номерами гостиницы (реализация AVL-дерева)
struct HotelRoom {
	string number; //строка формата "ANNN", где A - буква, обозначающая тип 
	//номера (Л - люкс, П - полулюкс, О - одноместный, М - многоместный);
	//NNN - порядковый номер (цифры)
	int count_of_seats; //количество мест
	int count_of_rooms; //количество комнат
	bool bathroom; //наличие санузла
	string facilities; //оборудование
	int height; //высота дерева
	HotelRoom* left;
	HotelRoom* right;
	int free_seats;


	HotelRoom(string n, int s, int r, bool b, string f) {
		number = n;
		count_of_seats = s;
		count_of_rooms = r;
		bathroom = b;
		facilities = f;
		left = right = 0;
		height = 1;
		if (n[0] == 'М') free_seats = s;
		else free_seats = 1;
	};
};

//функция проверки корректности введённого номера
void checkNumberRoom(string& num) {
	bool f;
	cin >> num;
	while (true) {
		f = true;
		if (num.size() != 4) {
			f = false;
		}
		else {
			if (!(num[0] == 'Л' || num[0] == 'П' || num[0] == 'О' || num[0] == 'М')) f = false;
			for (int i = 1; i < 4; ++i) {
				if (num[i] < '0' || '9' < num[i]) {
					f = false;
					break;
				}
			}
		}

		if (f) return;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> num;
	}
}

//функция проверки корректности введённого целого числа
void checkInt(int& n) {
	cin >> n;
	while (!cin.good() || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> n;
	}
}

//функция проверки корректности наличия ванной в номере
void checkBathroom(char& b) {
	cin >> b;
	while (!cin.good() || !(b == 'y' || b == 'n') || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> b;
	}
}

//функция вычисления высоты AVL-дерева
int height(HotelRoom* room) {
	if (room != NULL) return room->height;
	else return 0;
}

//функция вычисления баланс-фактора AVL-дерева
int getBalance(HotelRoom* room) {
	if (room != NULL) return height(room->right) - height(room->left);
	else return 0;
}

//функция обновления высоты AVL-дерева
void updateHeight(HotelRoom* room) {
	if (room != NULL) room->height = 1 + max(height(room->left), height(room->right));
}

//функция правого поворота AVL-дерева
HotelRoom* rotateRight(HotelRoom* room) {
	HotelRoom* room_new = room->left;
	room->left = room_new->right;
	room_new->right = room;
	updateHeight(room);
	updateHeight(room_new);
	return room_new;
}

//функция левого поворота AVL-дерева
HotelRoom* rotateLeft(HotelRoom* room) {
	HotelRoom* room_new = room->right;
	room->right = room_new->left;
	room_new->left = room;
	updateHeight(room);
	updateHeight(room_new);
	return room_new;
}

//функция балансировки AVL-дерева
HotelRoom* balance(HotelRoom* root)
{
	updateHeight(root);
	if (getBalance(root) == 2)
	{
		if (getBalance(root->right) < 0)
			root->right = rotateRight(root->right);
		return rotateLeft(root);
	}
	if (getBalance(root) == -2)
	{
		if (getBalance(root->left) > 0)
			root->left = rotateLeft(root->left);
		return rotateRight(root);
	}
	return root; // балансировка не нужна
}

//функция добавления нового элемента в AVL-дерево
HotelRoom* insert(HotelRoom* root, HotelRoom* room) {
	if (!root) return room;
	if (room->number < root->number) {
		root->left = insert(root->left, room);
	}
	else {
		root->right = insert(root->right, room);
	}
	return balance(root);
}

//функция проверки наличия номера в текущей базе
bool inHotelRoom(HotelRoom* root, string number) {
	if (!root) return false;

	if (number == root->number) {
		return true;
	}
	else if (number < root->number) {
		return inHotelRoom(root->left, number);
	}
	else {
		return inHotelRoom(root->right, number);
	}
}

//функция поиска минимального левого значения в поддереве
HotelRoom* findMin(HotelRoom* root) {
	if (root->left) return findMin(root->left);
	else return root;
}

//функция поиска минимального правого значения в поддереве
HotelRoom* removeMin(HotelRoom* root) {
	if (root->left == 0) {
		return root->right;
	}
	root->left = removeMin(root->left);
	return balance(root);
}

//функция удаления элемента из AVL-дерева
HotelRoom* remove(HotelRoom* root, string number) {
	if (!root) {
		return nullptr;
	}
	if (number < root->number) {
		root->left = remove(root->left, number);
	}
	else if (number > root->number) {
		root->right = remove(root->right, number);
	}
	else {
		HotelRoom* l = root->left;
		HotelRoom* r = root->right;

		if (!r) {
			root->left = nullptr;
			delete root;
			return l;
		}

		HotelRoom* mini = findMin(r);
		mini->right = removeMin(r);
		mini->left = l;
		if (r != mini) {
			delete r;
		}

		root = mini;
	}
	return balance(root);
}

//функция очищения данных о всех номерах 
void clearHotelRoom(HotelRoom* root) {
	if (root != nullptr) {
		clearHotelRoom(root->left);
		clearHotelRoom(root->right);
		delete root;
	}
}

//функция вывода номеров в порядке прямого обхода AVL-дерева
void printHotelRoom(HotelRoom* root) {
	if (!root) return;

	

	cout << root->number << '\t' << root->count_of_seats << "\t\t" << root->count_of_rooms << "\t\t\t" << (root->bathroom ? "true" : "false") << '\t' << root->facilities << '\n';
	if (root->left) printHotelRoom(root->left);
	if (root->right) printHotelRoom(root->right);
}

//функция поиска по номеру номера в AVL-дереве
HotelRoom* searchNumber(HotelRoom* root, string number) {
	if (number == root->number) {
		HotelRoom* result = new HotelRoom(root->number, root->count_of_seats, root->count_of_rooms, root->bathroom, root->facilities);
		return result;
	}
	else if (number < root->number) {
		return searchNumber(root->left, number);
	}
	else {
		return searchNumber(root->right, number);
	}
}

//функция доавления нового элемента в базу номеров
void addRoom(HotelRoom*& rooms) {
	string number, facilities;
	int count_of_seats, count_of_rooms;
	char b;
	bool bathroom;
	cout << "Заполните данные о номере: \n";
	cout << "Номер (формат \"ANNN\"): ";
	checkNumberRoom(number);
	if (inHotelRoom(rooms, number)) {
		cout << "Этот номер уже есть в базе!\n";
		return;
	}
	cout << "Количество мест: ";
	checkInt(count_of_seats);
	cout << "Количество комнат: ";
	checkInt(count_of_rooms);
	cout << "Наличие санузла (y/n): ";
	checkBathroom(b);
	if (b == 'y') bathroom = true;
	else bathroom = false;
	cout << "Оборудование: ";
	//cin.ignore();
	getline(cin, facilities);
	
	HotelRoom* room = new HotelRoom(number, count_of_seats, count_of_rooms, bathroom, facilities);
	rooms = insert(rooms, room);
}

//функция удаления элемента из базы элементов
void delRoom(HotelRoom*& rooms, TrafficData*& data) {
	string number;
	cout << "Введите номер, который нужно удалить: ";
	checkNumberRoom(number);

	if (!inHotelRoom(rooms, number)) {
		cout << "Такого номера нет в базе!\n";
		return;
	} 

	
	if (inTrafficData(data, number)) {
		cout << "\nУдаление невозмжно! Сначала надо зарегистрировать выселение постояльца!\n";
		return;
	}

	rooms = remove(rooms, number);
	cout << "Удаление выполнено!\n";
}

//функция вывода информации обо всех номерах
void printAllRooms(HotelRoom* rooms) {
	if (!rooms) {
		cout << "В базе нет номеров!\n";
		return;
	} 

	cout << "Номер" << '\t' << "Количество мест" << '\t' << "Количество комнат" << '\t' << "Санузел" << '\t' << "Оборудование\n";

	printHotelRoom(rooms);
	//printTree(rooms, rooms->height);
}

//функция поиска по номеру номера
void searchByNumber(HotelRoom* rooms) {
	string number;
	cout << "Введите номер, информацию о котором хотите получить: ";
	checkNumberRoom(number);

	HotelRoom* r = searchNumber(rooms, number);
	if (!inHotelRoom(rooms, number)) cout << "Такого номера нет в базе!\n";
	else {
		cout << "Номер" << '\t' << "Количество мест" << '\t' << "Количество комнат" << '\t' << "Санузел" << '\t' << "Оборудование\n";
		printHotelRoom(r);
	}
}

//функция реализующая алгоритм поиска Боуера-Мура
int BMsearch(string str, string substr) {
	int length_sub = substr.length();
	int length_str = str.length();
	if (length_str == 0 || length_sub == 0 || length_str < length_sub) {
		return -1;  // Если строки пустые или подстрока длиннее строки
	}

	int arr[256];
	for (int i = 0; i < 256; i++) {
		arr[i] = length_sub;
	}
	for (int i = 0; i < length_sub - 1; i++) {  // Последний символ не учитываем
		arr[(unsigned char)substr[i]] = length_sub - i - 1;
	}

	int position = length_sub - 1;
	while (position < length_str) {
		int j = length_sub - 1;
		while (j >= 0 && substr[j] == str[position - length_sub + 1 + j]) {
			j--;
		}
		if (j < 0) {  // Совпадение найдено
			return position - length_sub + 1;  // Возвращаем индекс начала совпадения
		}

		position += arr[(unsigned char)str[position]];  // Сдвигаем по таблице
	}

	return -1;  // Совпадение не найдено
}

//функция поиска оборудования в строке по AVL дереву
bool searchFacilityInHotelRooms(HotelRoom* root, const string& facility, vector<HotelRoom*>& results) {
	if (root == nullptr) return false;
	bool found = false;

	// Поиск в левом поддереве
	found |= searchFacilityInHotelRooms(root->left, facility, results);

	// Поиск оборудования в текущем узле
	if (BMsearch(root->facilities, facility) != -1) {
		results.push_back(new HotelRoom(*root));
		found = true; // найдено совпадение
	}

	// Поиск в правом поддереве
	found |= searchFacilityInHotelRooms(root->right, facility, results);

	return found;
}

//функция поиска по оборудованию
void searchByEquipment(HotelRoom* rooms) {
	string equipment;
	cout << "Введите название оборудоваия: ";
	getline(cin, equipment);

	vector<HotelRoom*> results;
	if (searchFacilityInHotelRooms(rooms, equipment, results)) {
		cout << "Номер" << '\t' << "Количество мест" << '\t' << "Количество комнат" << '\t' << "Санузел" << '\t' << "Оборудование\n";
		for (int i = 0; i < results.size(); ++i) {
			cout << results[i]->number << '\t' << results[i]->count_of_seats << "\t\t" << results[i]->count_of_rooms << "\t\t\t" << (results[i]->bathroom ? "true" : "false") << '\t' << results[i]->facilities << '\n';
		}
	}
	else {
		cout << "Нет номеров с таким оборудованием!\n";
	}
}

//функция вывода меню раюоты с номерами
void printRoomMenu() {
	system("cls");
	cout << "Выберите действие: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | Добавление нового номера          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | Удаление сведений о номере        |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | Просмотр всех номеров             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 4. | Очистка данных о номерах          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 5. | Поиск по номеру                   |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 6. | Поиск по оборудованию             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | Вернуться в главное меню          |\n";
	cout << "+----+-----------------------------------+\n";
}

//функция для работы с номерами
void roomMenu(HotelRoom*& rooms, TrafficData*& data) {
	char menu_item;
	while (true) {
		printRoomMenu();
		cout << "Ваш выбор: ";
		cin >> menu_item;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("cls");
		switch (menu_item) {
		case '1':
			addRoom(rooms);
			system("pause");
			break;
		case '2':
			delRoom(rooms, data);
			system("pause");
			break;
		case '3':
			printAllRooms(rooms);
			system("pause");
			break;
		case '4':
			if (data == nullptr) {
				clearHotelRoom(rooms);
				cout << "Данные о постояльцах очищены!\n";
			}
			else {
				cout << "\nНевозможно очистить данные о всех номерах, так как есть зарегистрированные!\n";
			}
			system("pause");
			break;
		case '5':
			searchByNumber(rooms);
			system("pause");
			break;
		case '6':
			searchByEquipment(rooms);
			system("pause");
			break;
		case '0':
			return;
		default:
			cout << "Некорректный ввод!\n";
			system("pause");
		}
		system("cls");
	}
}

//------------------------------действия с выселением и вселением------------------------------------//
//функция проверки корректности введённой даты
void checkDate(string& date) {
	bool f;
	cin >> date;
	while (true) {
		f = true;
		if (date.size() != 10) {
			f = false;
		}
		else {
			if (!(date[2] == '.' && date[5] == '.')) {
				f = false;
			}
			if (date[0] < '0' || '9' < date[0] || date[1] < '0' || '9' < date[1] ||
				date[3] < '0' || '9' < date[3] || date[4] < '0' || '9' < date[4] ||
				date[6] < '0' || '9' < date[6] || date[7] < '0' || '9' < date[7] ||
				date[8] < '0' || '9' < date[8] || date[9] < '0' || '9' < date[9]) {
				f = false;
			}
		}
		if (f) return;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Введено некорректное значение! ";
		cout << "Повторите ввод: ";
		cin >> date;
	}
}

//функция добавления нового вселения
void add(TrafficData** head, TrafficData* p) {
	TrafficData* cur = *head;

	if (cur == nullptr) {
		*head = p;
		p->next = p;
	}
	else {
		while (cur->next != *head)
			cur = cur->next;
		cur->next = p;
		p->next = *head;
	}
}

//функция удаления вселения (выселение)
void del(TrafficData** head, string passport, string number) {
	TrafficData* prev = nullptr;
	TrafficData* cur = *head;

	if (cur == nullptr) {
		// Список пустой, удалять нечего.
		return;
	}

	// Ищем удаляемый узел и его предыдущий элемент.
	do {
		if (cur->passport == passport && cur->number == number) {
			// Нашли удаляемый узел.
			break;
		}
		prev = cur;
		cur = cur->next;
	} while (cur != *head);

	if (cur->passport != passport || cur->number != number) {
		// Удаляемый элемент не найден.
		return;
	}

	if (prev == nullptr) {
		// Удаляем единственный элемент
		delete* head;
		*head = nullptr;
	}
	else if (cur->next == cur) {
		// Удаляем последний элемент в циклическом списке
		prev->next = cur;
		delete cur;
		*head = nullptr;
	}
	else {
		// Удаляем узел из середины или начала
		prev->next = cur->next;
		if (cur == *head) {
			*head = cur->next; // Меняем head, если удаляем первый элемент
		}
		delete cur;
	}
}

//функция проверки наличия зарегистрированного вселения
bool inTrafficData(TrafficData* head, string passport, string number) {
	if (head == nullptr) {
		return false;
	}
	TrafficData* cur = head;
	do {
		if (cur->passport == passport && cur->number == number) {
			return true;
		}
		cur = cur->next;
	} while (cur != head);
	return false;
}

//функция сортировки вставками 
void insertSorted(TrafficData** headRef, TrafficData* newNode) {
	// Специальный случай для головного узла
	if (*headRef == nullptr || (*headRef)->passport >= newNode->passport) {
		newNode->next = *headRef;
		*headRef = newNode;
	}
	else {
		// Найти узел, после которого необходимо осуществить вставку
		TrafficData* current = *headRef;
		while (current->next != nullptr && current->next->passport < newNode->passport) {
			current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
	}
}

//вспомогательная функция сортировки вставками 
void insertionSort(TrafficData** headRef) {
	// Инициализируем отсортированный список
	TrafficData* sorted = nullptr;

	// Перебираем текущий список и вставляем каждый его узел в отсортированный список
	TrafficData* current = *headRef;
	while (current != nullptr) {
		// Запоминаем следующий для текущего, потому что мы меняем его next
		TrafficData* next = current->next;

		// Вставляем текущий в отсортированный список
		insertSorted(&sorted, current);

		// Перемещаемся на следующий узел
		current = next;
	}

	// Изменяем headRef, чтобы он указывал на отсортированный список
	*headRef = sorted;
}

//функция для очищения данных о зарегистрированных заселениях
void clearTrafficData(TrafficData*& head) {
	while (head != nullptr) {
		TrafficData* temp = head; // Сохраняем текущий элемент.
		head = head->next; // Перемещаемся к следующему элементу.
		delete temp; // Удаляем текущий элемент.
	}
}

//функция регистрации вселения
void registrationCheckIn(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	cout << "Введите следующие данные для регистрации вселения: \n";
	string passport, number, checkin, checkout;
	cout << "Номер паспорта (строка формата NNNN-NNNNNN, где N - цифры): ";
	checkPassport(passport);
	if (!table->inHashTable(passport)) {
		cout << "\nЭтот постоялец не живёт в гостинице!\n";
		return;
	}
	cout << "Номер гостиничного номера (строка формата \"ANNN\"): ";
	checkNumberRoom(number);
	if (!inHotelRoom(rooms, number)) {
		cout << "\nЭтого номера нет в базе номеров!\n";
		return;
	}
	if (inTrafficData(data, passport, number)) {
		cout << "\nЭтот постоялец уже зарегистрирован!\n";
		return;
	}
	if (rooms->count_of_seats == 0) {
		cout << "\nВ этой комнате нет свободных мест!\n";
		return;
	}
	cout << "Дата заселения: ";
	checkDate(checkin);
	cout << "Дата выселения: ";
	checkDate(checkout);

	bool f = true;
	int d1 = checkin[0] * 10 + checkin[1], d2 = checkout[0] * 10 + checkout[1];
	int m1 = checkin[3] * 10 + checkin[4], m2 = checkout[3] * 10 + checkout[4];
	int y1 = checkin[6] * 1000 + checkin[7] * 100 + checkin[8] * 10 + checkin[9],
		y2 = checkout[6] * 1000 + checkout[7] * 100 + checkout[8] * 10 + checkout[9];
	if (y1 > y2) f = false;
	else if (y2 == y1 && m1 > m2) f = false;
	else if (y2 == y1 && m2 == m1 && d1 > d2) f = false;

	if (!f) {
		cout << "\nДата выселения не может быть меньше даты заселения!\n";
		return;
	}

	TrafficData* d = new TrafficData(passport, number, checkin, checkout);
	add(&data, d);
	insertionSort(&data);
	rooms->free_seats--;
	cout << "\nЗаселение загеристировано!\n";
}

//функиця регистрации выселения
void registrationCheckOut(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	cout << "Введите следующие данные для регистрации выселения: \n";
	string passport, number, checkin, checkout;
	cout << "Номер паспорта (строка формата NNNN-NNNNNN, где N - цифры): ";
	checkPassport(passport);
	if (!table->inHashTable(passport)) {
		cout << "\nЭтот постоялец не живёт в гостинице!\n";
		return;
	}
	cout << "Номер гостиничного номера (строка формата \"ANNN\"): ";
	checkNumberRoom(number);
	if (!inHotelRoom(rooms, number)) {
		cout << "\nЭтого номера нет в базе номеров!\n";
		return;
	}

	if (!inTrafficData(data, passport, number)) {
		cout << "\nТакой регистрации нет в базе!\n";
		return;
	}

	del(&data, passport, number);
	rooms->free_seats++;
	cout << "\nРегистрация выселения прошла!\n";
}

//функция вывода меню регистрации вселения и выселения
void printDataMenu() {
	system("cls");
	cout << "Выберите действие: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | Регистрация вселения              |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | Регистрация выселения             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | Вернуться в главное меню          |\n";
	cout << "+----+-----------------------------------+\n";
}

//функция реализации работы меню регистрации вселения и выселения
void dataMenu(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	char menu_item;
	while (true) {
		printDataMenu();
		cout << "Ваш выбор: ";
		cin >> menu_item;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		system("cls");
		switch (menu_item) {
		case '1':
			registrationCheckIn(data, table, rooms);
			system("pause");
			break;
		case '2':
			registrationCheckOut(data, table, rooms);
			system("pause");
			break;
		case '0':
			return;
		default:
			cout << "Некорректный ввод!\n";

			system("pause");
		}
		system("cls");
	}
}

//функция вывода главного меню
void printMainMenu() {
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | Постоялец                         |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | Гостиничный номер                 |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | Выселение и поселение постояльцев |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | Завершение работы                 |\n";
	cout << "+----+-----------------------------------+\n";
}

//функция реализации работы главного меню
void mainMenu(){
	hashMap table;
	HotelRoom* rooms = NULL;
	TrafficData* data = NULL;
	char menu_item;
	while (true) {
		cout << "Выберите, с какими данными нужно осуществить работу: \n";
		printMainMenu();
		cout << "Ваш выбор: ";
		cin >> menu_item;

		system("cls");

		switch (menu_item) {
		case '1':
			guestMenu(&table, data);
			break;
		case '2':
			roomMenu(rooms, data);
			break;
		case '3':
			dataMenu(data, &table, rooms);
			break;
		case '0':
			cout << "Работа программы завершена!\n";
			table.~hashMap();
			clearHotelRoom(rooms);
			clearTrafficData(data);

			return;
		}
		system("cls");
	}
	
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");

	mainMenu();

	return 0;
}