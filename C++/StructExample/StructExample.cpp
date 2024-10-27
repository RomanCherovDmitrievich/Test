#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Human
{
  string Name;
  unsigned short Age;
};

int main()
{
  // Для корректного вывода кириллических символов в консоли.
  setlocale(LC_ALL, "Russian");

  cout << "Работа с данными объекта структуры." << endl;
  Human h;
  h.Name = "Человек";
  h.Age = 1;
  cout << "h - Имя: " << h.Name << ". Возраст: " << h.Age << endl;

  cout << "Работа с данными объекта структуры через указатель." << endl;
  Human * ph = &h;
  ph->Name = "Указатель на человека";
  ph->Age = 2;
  cout << "ph - Имя: " << ph->Name << ". Возраст: " << ph->Age << endl;
  cout << "h - Имя: " << h.Name << ". Возраст: " << h.Age << endl;

  cout << "Работа с данными массива структур по индексу." << endl;
  Human ha[] = { {"Человек 0", 0}, {"Человек 1", 1} };
  cout << "ha[0] - Имя: " << ha[0].Name << ". Возраст: " << ha[0].Age << endl;
  cout << "ha[1] - Имя: " << ha[1].Name << ". Возраст: " << ha[1].Age << endl;

  cout << "Работа с данными массива структур по указателю." << endl;
  Human* pha = &ha[0];
  for (int i = 0; i < static_cast<int>(sizeof(ha) / sizeof(Human)); ++i)
  {
    pha->Name += " указатель";
    pha->Age += 10;
    cout << "pha - Имя: " << pha->Name << ". Возраст: " << pha->Age << endl;
    ++pha;
  }
  cout << "ha[0] - Имя: " << ha[0].Name << ". Возраст: " << ha[0].Age << endl;
  cout << "ha[1] - Имя: " << ha[1].Name << ". Возраст: " << ha[1].Age << endl;

  cout << "Работа с данными вектора структур по итератору." << endl;
  vector<Human> hv;

  h.Name = "Вектор 1";
  h.Age = 1;
  hv.push_back(h);
  h.Name = "Вектор 2";
  h.Age = 2;
  hv.push_back(h);
  h.Name = "Вектор 3";
  h.Age = 3;
  hv.push_back(h);
  for (auto it = hv.begin(); it != hv.end(); ++it)
    cout << "it - Имя: " << it->Name << ". Возраст: " << it->Age << endl;

  return 0;
}
