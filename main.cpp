/*
���������� �������: ����������� ����������� � ���������
����� �����������: �������� �����������
����� ����������: ���������
��� ������: ����������� ����������������
����� ������ ������: ������
�������� ������ ����� � ������: ������ � ���� (��)
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

//--------------------------------������ �����������-------------------------------------//
//���������, �������� ������ � ����������� (����������������� ������)
struct Guest {
	string passport; // ��������� ����, "NNNN - NNNNNN", ��� N � �����
	string full_name; // ���
	int year_of_birth; // ��� �������� 
	string address; // �����
	string purpose; // ���� ��������
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

//���������, �������� ������ � ������������������� ��������� (��������������� ����������� ������)
struct TrafficData {
	string passport; //"NNNN - NNNNNN", ��� N � �����
	string number; //"ANNN"
	string checkin; //���� ���������
	string checkout; //���� ���������
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

//������� ���������� ���������� � ������ ������
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

//������� �������� ���������� �� ������ ������
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

//������� ������ ����������� ������ �����������
void print_guests(Guest* head) {
	Guest* p = head;
	if (p == NULL) {
		cout << "������ ����������� ����!\n";
		return;
	}
	while (p->next != NULL) {
		cout << p->passport << '\t' << p->full_name << "\t\t" << p->year_of_birth << "\t\t" << p->address << "\t\t" << p->purpose << "\n";
		p = p->next;
	}
	cout << p->passport << '\t' << p->full_name << "\t\t" << p->year_of_birth << "\t\t" << p->address << "\t\t" << p->purpose << "\n";
}

//������� ���������� ������� ������ �����������
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

//������� ��� ������� ����������� ������ �����������
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

//������� �������� ������������ ��������� ������ ��������
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
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> passport;
	}
}

//������� �������� ������������ ��������� ���� ��������
void checkYear(int& n) {
	cin >> n;
	while (!cin.good() || n <  1902 || 2024 < n || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> n;
	}
}

//������� �������� ������� ������ � ������ ������������������ �������� 
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

//-------------------------���-������� � ������� � �����������----------------------------//
//��������� ��� ������ � ������� � ����������� (���������� ���-�������)
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

//����� ���������� ����
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

//����� ���������� ������ �������� � ���-�������
void hashMap::insert(Guest* g) {
	int index = hashFunction(g->passport);
	push_guest(&arr[index], g);
	size++;
}

//����� �������� �������� �� ���-�������
void hashMap::erase(string passport) {
	int index = hashFunction(passport);
	pop_guest(&arr[index], passport);
	size--;
}

//����� �������� ������� �������� � ���-�������
bool hashMap::inHashTable(string passport) {
	int index = hashFunction(passport);
	Guest* ptr = arr[index];
	while (ptr != NULL) {
		if (ptr->passport == passport) return true;
		ptr = ptr->next;
	}
	return false;
}

//����� ������ � ���-������� �� ������ �������� ����������
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

//����� ������ � ���-������� �� ��� ����������
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

//����� ������ ���-������� �� �����
void hashMap::printTable() {
	if (size == 0) {
		cout << "������ ������������������ ����!\n";
		return;
	}
	cout << "����� ��������" << '\t' << "���" << "\t\t" << "��� ��������" << '\t' << "�����" << "\t\t" << "���� ��������" << '\n';
	for (int i = 0; i < capacity; ++i) {
		if (size_guests_list(arr[i]) != 0) {
			print_guests(arr[i]);
		}
	}
}

//------------------------------�������� � ������������-----------------------------------//

//������� ��� ����������� ������ ����������
void registration(hashMap* table) {
	cout << "��������� ������ � ����������: \n";
	string passport, name, address, purpose;
	int year;
	cout << "����� �������� (������ ������� NNNN-NNNNNN, ��� N - �����): ";
	checkPassport(passport);
	if (table->inHashTable(passport)) {
		cout << "���� ������� ��� ���� � ����!\n";
		return;
	}
	cout << "���: ";
	cin.ignore();
	getline(cin, name);
	cout << "��� ��������: ";
	checkYear(year);
	cout << "�����: ";
	//cin.ignore();
	getline(cin, address);
	cout << "���� ��������: ";
	//cin.ignore();
	getline(cin, purpose);
	
	Guest* g = new Guest(passport, name, year, address, purpose);
	table->insert(g);
}

//������� ��� �������� ����������
void deliting(hashMap* table, TrafficData* data) {
	if (table->size == 0) {
		cout << "������ ������������������ ����!\n";
		return;
	}
	string passport;
	cout << "������� ����� �������� ����������: ";
	checkPassport(passport);
	cin.ignore();

	if (!table->inHashTable(passport)) {
		cout << "\n���������� � ����� ������� �������� ��� � ������!\n";
		return;
	}
	 
	if (inTrafficData(data, passport)) {
		cout << "\n�������� ���������! ������� ���� ���������������� ��������� ����������!\n";
		return;
	}
	table->erase(passport);
	cout << "\n������ � ���������� �������!\n";
}

//������� ������ ���������� �� ������ ��������
void searchPassport(hashMap* table) {
	cout << "������� ����� ��������, �� �������� ������ ���������� ����������: ";
	string passport;
	checkPassport(passport);

	if (table->inHashTable(passport)) {
		Guest* ans = table->searchByPassport(passport);
		cout << "����� ��������" << '\t' << "���" << "\t\t" << "��� ��������" << '\t' << "�����" << "\t\t" << "���� ��������" << '\n';
		cout << ans->passport << '\t' << ans->full_name << "\t\t" << ans->year_of_birth << "\t\t" << ans->address << "\t\t" << ans->purpose << "\n";
	}
	else {
		cout << "� ������ ����������� ��� �������� � ����� ������� ��������!\n";
	}
}

//������� ������ ���������� �� ���
void searchFIO(hashMap* table) {
	cout << "������� ��� ����������, ���������� � ������� ������ ����������: ";
	//cin.ignore();
	string name;
	getline(cin, name);

	Guest* ans = table->searchByName(name);
	if (ans == NULL) cout << "� ������ ����������� ��� �������� � ����� ���!\n";
	else {
		cout << "����� ��������" << '\t' << "���" << "\t\t" << "��� ��������" << '\t' << "�����" << "\t\t" << "���� ��������" << '\n';
		print_guests(ans);
	}
	free_list(&ans);
}

//������� ������ ���� ��� ������ � ������������
void printGuestMenu() {
	system("cls");
	cout << "�������� ��������: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | ����������� ������ ����������     |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | �������� ������ � ����������      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | �������� ���� ������������������  |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 4. | ������� ������ � �����������      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 5. | ����� �� ������ ��������          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 6. | ����� �� ���                      |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | ��������� � ������� ����          |\n";
	cout << "+----+-----------------------------------+\n";
}

//������� ��� ���������� ������ ���� �����������
void guestMenu(hashMap* table, TrafficData* data) {
	char menu_item;
	while (true) {
		printGuestMenu();
		cout << "��� �����: ";
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
				cout << "������ � ����������� �������!\n";
			}
			else {
				cout << "\n���������� �������� ������ � ���� �����������, ��� ��� ���� ������������������!\n";
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
			cout << "������������ ����!\n";
			system("pause");
		}
	}
}

//------------------------�������� � ������������� ��������-------------------------------//
//��������� ��� ������ � �������� ��������� (���������� AVL-������)
struct HotelRoom {
	string number; //������ ������� "ANNN", ��� A - �����, ������������ ��� 
	//������ (� - ����, � - ��������, � - �����������, � - ������������);
	//NNN - ���������� ����� (�����)
	int count_of_seats; //���������� ����
	int count_of_rooms; //���������� ������
	bool bathroom; //������� �������
	string facilities; //������������
	int height; //������ ������
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
		if (n[0] == '�') free_seats = s;
		else free_seats = 1;
	};
};

//������� �������� ������������ ��������� ������
void checkNumberRoom(string& num) {
	bool f;
	cin >> num;
	while (true) {
		f = true;
		if (num.size() != 4) {
			f = false;
		}
		else {
			if (!(num[0] == '�' || num[0] == '�' || num[0] == '�' || num[0] == '�')) f = false;
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
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> num;
	}
}

//������� �������� ������������ ��������� ������ �����
void checkInt(int& n) {
	cin >> n;
	while (!cin.good() || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> n;
	}
}

//������� �������� ������������ ������� ������ � ������
void checkBathroom(char& b) {
	cin >> b;
	while (!cin.good() || !(b == 'y' || b == 'n') || cin.get() != '\n') {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> b;
	}
}

//������� ���������� ������ AVL-������
int height(HotelRoom* room) {
	if (room != NULL) return room->height;
	else return 0;
}

//������� ���������� ������-������� AVL-������
int getBalance(HotelRoom* room) {
	if (room != NULL) return height(room->right) - height(room->left);
	else return 0;
}

//������� ���������� ������ AVL-������
void updateHeight(HotelRoom* room) {
	if (room != NULL) room->height = 1 + max(height(room->left), height(room->right));
}

//������� ������� �������� AVL-������
HotelRoom* rotateRight(HotelRoom* room) {
	HotelRoom* room_new = room->left;
	room->left = room_new->right;
	room_new->right = room;
	updateHeight(room);
	updateHeight(room_new);
	return room_new;
}

//������� ������ �������� AVL-������
HotelRoom* rotateLeft(HotelRoom* room) {
	HotelRoom* room_new = room->right;
	room->right = room_new->left;
	room_new->left = room;
	updateHeight(room);
	updateHeight(room_new);
	return room_new;
}

//������� ������������ AVL-������
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
	return root; // ������������ �� �����
}

//������� ���������� ������ �������� � AVL-������
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

//������� �������� ������� ������ � ������� ����
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

//������� ������ ������������ ������ �������� � ���������
HotelRoom* findMin(HotelRoom* root) {
	if (root->left) return findMin(root->left);
	else return root;
}

//������� ������ ������������ ������� �������� � ���������
HotelRoom* removeMin(HotelRoom* root) {
	if (root->left == 0) {
		return root->right;
	}
	root->left = removeMin(root->left);
	return balance(root);
}

//������� �������� �������� �� AVL-������
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

//������� �������� ������ � ���� ������� 
void clearHotelRoom(HotelRoom* root) {
	if (root != nullptr) {
		clearHotelRoom(root->left);
		clearHotelRoom(root->right);
		delete root;
	}
}

//������� ������ ������� � ������� ������� ������ AVL-������
void printHotelRoom(HotelRoom* root) {
	if (!root) return;

	

	cout << root->number << '\t' << root->count_of_seats << "\t\t" << root->count_of_rooms << "\t\t\t" << (root->bathroom ? "true" : "false") << '\t' << root->facilities << '\n';
	if (root->left) printHotelRoom(root->left);
	if (root->right) printHotelRoom(root->right);
}

//������� ������ �� ������ ������ � AVL-������
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

//������� ��������� ������ �������� � ���� �������
void addRoom(HotelRoom*& rooms) {
	string number, facilities;
	int count_of_seats, count_of_rooms;
	char b;
	bool bathroom;
	cout << "��������� ������ � ������: \n";
	cout << "����� (������ \"ANNN\"): ";
	checkNumberRoom(number);
	if (inHotelRoom(rooms, number)) {
		cout << "���� ����� ��� ���� � ����!\n";
		return;
	}
	cout << "���������� ����: ";
	checkInt(count_of_seats);
	cout << "���������� ������: ";
	checkInt(count_of_rooms);
	cout << "������� ������� (y/n): ";
	checkBathroom(b);
	if (b == 'y') bathroom = true;
	else bathroom = false;
	cout << "������������: ";
	//cin.ignore();
	getline(cin, facilities);
	
	HotelRoom* room = new HotelRoom(number, count_of_seats, count_of_rooms, bathroom, facilities);
	rooms = insert(rooms, room);
}

//������� �������� �������� �� ���� ���������
void delRoom(HotelRoom*& rooms, TrafficData*& data) {
	string number;
	cout << "������� �����, ������� ����� �������: ";
	checkNumberRoom(number);

	if (!inHotelRoom(rooms, number)) {
		cout << "������ ������ ��� � ����!\n";
		return;
	} 

	
	if (inTrafficData(data, number)) {
		cout << "\n�������� ���������! ������� ���� ���������������� ��������� ����������!\n";
		return;
	}

	rooms = remove(rooms, number);
	cout << "�������� ���������!\n";
}

//������� ������ ���������� ��� ���� �������
void printAllRooms(HotelRoom* rooms) {
	if (!rooms) {
		cout << "� ���� ��� �������!\n";
		return;
	} 

	cout << "�����" << '\t' << "���������� ����" << '\t' << "���������� ������" << '\t' << "�������" << '\t' << "������������\n";

	printHotelRoom(rooms);
	//printTree(rooms, rooms->height);
}

//������� ������ �� ������ ������
void searchByNumber(HotelRoom* rooms) {
	string number;
	cout << "������� �����, ���������� � ������� ������ ��������: ";
	checkNumberRoom(number);

	HotelRoom* r = searchNumber(rooms, number);
	if (!inHotelRoom(rooms, number)) cout << "������ ������ ��� � ����!\n";
	else {
		cout << "�����" << '\t' << "���������� ����" << '\t' << "���������� ������" << '\t' << "�������" << '\t' << "������������\n";
		printHotelRoom(r);
	}
}

//������� ����������� �������� ������ ������-����
int BMsearch(string str, string substr) {
	int length_sub = substr.length();
	int length_str = str.length();
	if (length_str == 0 || length_sub == 0 || length_str < length_sub) {
		return -1;  // ���� ������ ������ ��� ��������� ������� ������
	}

	int arr[256];
	for (int i = 0; i < 256; i++) {
		arr[i] = length_sub;
	}
	for (int i = 0; i < length_sub - 1; i++) {  // ��������� ������ �� ���������
		arr[(unsigned char)substr[i]] = length_sub - i - 1;
	}

	int position = length_sub - 1;
	while (position < length_str) {
		int j = length_sub - 1;
		while (j >= 0 && substr[j] == str[position - length_sub + 1 + j]) {
			j--;
		}
		if (j < 0) {  // ���������� �������
			return position - length_sub + 1;  // ���������� ������ ������ ����������
		}

		position += arr[(unsigned char)str[position]];  // �������� �� �������
	}

	return -1;  // ���������� �� �������
}

//������� ������ ������������ � ������ �� AVL ������
bool searchFacilityInHotelRooms(HotelRoom* root, const string& facility, vector<HotelRoom*>& results) {
	if (root == nullptr) return false;
	bool found = false;

	// ����� � ����� ���������
	found |= searchFacilityInHotelRooms(root->left, facility, results);

	// ����� ������������ � ������� ����
	if (BMsearch(root->facilities, facility) != -1) {
		results.push_back(new HotelRoom(*root));
		found = true; // ������� ����������
	}

	// ����� � ������ ���������
	found |= searchFacilityInHotelRooms(root->right, facility, results);

	return found;
}

//������� ������ �� ������������
void searchByEquipment(HotelRoom* rooms) {
	string equipment;
	cout << "������� �������� �����������: ";
	getline(cin, equipment);

	vector<HotelRoom*> results;
	if (searchFacilityInHotelRooms(rooms, equipment, results)) {
		cout << "�����" << '\t' << "���������� ����" << '\t' << "���������� ������" << '\t' << "�������" << '\t' << "������������\n";
		for (int i = 0; i < results.size(); ++i) {
			cout << results[i]->number << '\t' << results[i]->count_of_seats << "\t\t" << results[i]->count_of_rooms << "\t\t\t" << (results[i]->bathroom ? "true" : "false") << '\t' << results[i]->facilities << '\n';
		}
	}
	else {
		cout << "��� ������� � ����� �������������!\n";
	}
}

//������� ������ ���� ������ � ��������
void printRoomMenu() {
	system("cls");
	cout << "�������� ��������: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | ���������� ������ ������          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | �������� �������� � ������        |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | �������� ���� �������             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 4. | ������� ������ � �������          |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 5. | ����� �� ������                   |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 6. | ����� �� ������������             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | ��������� � ������� ����          |\n";
	cout << "+----+-----------------------------------+\n";
}

//������� ��� ������ � ��������
void roomMenu(HotelRoom*& rooms, TrafficData*& data) {
	char menu_item;
	while (true) {
		printRoomMenu();
		cout << "��� �����: ";
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
				cout << "������ � ����������� �������!\n";
			}
			else {
				cout << "\n���������� �������� ������ � ���� �������, ��� ��� ���� ������������������!\n";
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
			cout << "������������ ����!\n";
			system("pause");
		}
		system("cls");
	}
}

//------------------------------�������� � ���������� � ���������------------------------------------//
//������� �������� ������������ �������� ����
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
		cerr << "������� ������������ ��������! ";
		cout << "��������� ����: ";
		cin >> date;
	}
}

//������� ���������� ������ ��������
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

//������� �������� �������� (���������)
void del(TrafficData** head, string passport, string number) {
	TrafficData* prev = nullptr;
	TrafficData* cur = *head;

	if (cur == nullptr) {
		// ������ ������, ������� ������.
		return;
	}

	// ���� ��������� ���� � ��� ���������� �������.
	do {
		if (cur->passport == passport && cur->number == number) {
			// ����� ��������� ����.
			break;
		}
		prev = cur;
		cur = cur->next;
	} while (cur != *head);

	if (cur->passport != passport || cur->number != number) {
		// ��������� ������� �� ������.
		return;
	}

	if (prev == nullptr) {
		// ������� ������������ �������
		delete* head;
		*head = nullptr;
	}
	else if (cur->next == cur) {
		// ������� ��������� ������� � ����������� ������
		prev->next = cur;
		delete cur;
		*head = nullptr;
	}
	else {
		// ������� ���� �� �������� ��� ������
		prev->next = cur->next;
		if (cur == *head) {
			*head = cur->next; // ������ head, ���� ������� ������ �������
		}
		delete cur;
	}
}

//������� �������� ������� ������������������� ��������
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

//������� ���������� ��������� 
void insertSorted(TrafficData** headRef, TrafficData* newNode) {
	// ����������� ������ ��� ��������� ����
	if (*headRef == nullptr || (*headRef)->passport >= newNode->passport) {
		newNode->next = *headRef;
		*headRef = newNode;
	}
	else {
		// ����� ����, ����� �������� ���������� ����������� �������
		TrafficData* current = *headRef;
		while (current->next != nullptr && current->next->passport < newNode->passport) {
			current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
	}
}

//��������������� ������� ���������� ��������� 
void insertionSort(TrafficData** headRef) {
	// �������������� ��������������� ������
	TrafficData* sorted = nullptr;

	// ���������� ������� ������ � ��������� ������ ��� ���� � ��������������� ������
	TrafficData* current = *headRef;
	while (current != nullptr) {
		// ���������� ��������� ��� ��������, ������ ��� �� ������ ��� next
		TrafficData* next = current->next;

		// ��������� ������� � ��������������� ������
		insertSorted(&sorted, current);

		// ������������ �� ��������� ����
		current = next;
	}

	// �������� headRef, ����� �� �������� �� ��������������� ������
	*headRef = sorted;
}

//������� ��� �������� ������ � ������������������ ����������
void clearTrafficData(TrafficData*& head) {
	while (head != nullptr) {
		TrafficData* temp = head; // ��������� ������� �������.
		head = head->next; // ������������ � ���������� ��������.
		delete temp; // ������� ������� �������.
	}
}

//������� ����������� ��������
void registrationCheckIn(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	cout << "������� ��������� ������ ��� ����������� ��������: \n";
	string passport, number, checkin, checkout;
	cout << "����� �������� (������ ������� NNNN-NNNNNN, ��� N - �����): ";
	checkPassport(passport);
	if (!table->inHashTable(passport)) {
		cout << "\n���� ��������� �� ���� � ���������!\n";
		return;
	}
	cout << "����� ������������ ������ (������ ������� \"ANNN\"): ";
	checkNumberRoom(number);
	if (!inHotelRoom(rooms, number)) {
		cout << "\n����� ������ ��� � ���� �������!\n";
		return;
	}
	if (inTrafficData(data, passport, number)) {
		cout << "\n���� ��������� ��� ���������������!\n";
		return;
	}
	if (rooms->count_of_seats == 0) {
		cout << "\n� ���� ������� ��� ��������� ����!\n";
		return;
	}
	cout << "���� ���������: ";
	checkDate(checkin);
	cout << "���� ���������: ";
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
		cout << "\n���� ��������� �� ����� ���� ������ ���� ���������!\n";
		return;
	}

	TrafficData* d = new TrafficData(passport, number, checkin, checkout);
	add(&data, d);
	insertionSort(&data);
	rooms->free_seats--;
	cout << "\n��������� ���������������!\n";
}

//������� ����������� ���������
void registrationCheckOut(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	cout << "������� ��������� ������ ��� ����������� ���������: \n";
	string passport, number, checkin, checkout;
	cout << "����� �������� (������ ������� NNNN-NNNNNN, ��� N - �����): ";
	checkPassport(passport);
	if (!table->inHashTable(passport)) {
		cout << "\n���� ��������� �� ���� � ���������!\n";
		return;
	}
	cout << "����� ������������ ������ (������ ������� \"ANNN\"): ";
	checkNumberRoom(number);
	if (!inHotelRoom(rooms, number)) {
		cout << "\n����� ������ ��� � ���� �������!\n";
		return;
	}

	if (!inTrafficData(data, passport, number)) {
		cout << "\n����� ����������� ��� � ����!\n";
		return;
	}

	del(&data, passport, number);
	rooms->free_seats++;
	cout << "\n����������� ��������� ������!\n";
}

//������� ������ ���� ����������� �������� � ���������
void printDataMenu() {
	system("cls");
	cout << "�������� ��������: \n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | ����������� ��������              |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | ����������� ���������             |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | ��������� � ������� ����          |\n";
	cout << "+----+-----------------------------------+\n";
}

//������� ���������� ������ ���� ����������� �������� � ���������
void dataMenu(TrafficData*& data, hashMap* table, HotelRoom* rooms) {
	char menu_item;
	while (true) {
		printDataMenu();
		cout << "��� �����: ";
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
			cout << "������������ ����!\n";

			system("pause");
		}
		system("cls");
	}
}

//������� ������ �������� ����
void printMainMenu() {
	cout << "+----+-----------------------------------+\n";
	cout << "| 1. | ���������                         |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 2. | ����������� �����                 |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 3. | ��������� � ��������� ����������� |\n";
	cout << "+----+-----------------------------------+\n";
	cout << "| 0. | ���������� ������                 |\n";
	cout << "+----+-----------------------------------+\n";
}

//������� ���������� ������ �������� ����
void mainMenu(){
	hashMap table;
	HotelRoom* rooms = NULL;
	TrafficData* data = NULL;
	char menu_item;
	while (true) {
		cout << "��������, � ������ ������� ����� ����������� ������: \n";
		printMainMenu();
		cout << "��� �����: ";
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
			cout << "������ ��������� ���������!\n";
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