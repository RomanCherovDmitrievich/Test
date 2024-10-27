#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

bool IsSystemFilename(const string& filename)
{
  // Зарезервированные имена файлов. Список зарезервированных имён взят по ссылке
  // https://learn.microsoft.com/ru-ru/windows/win32/fileio/naming-a-file?redirectedfrom=MSDN
  static const char* system_filename[] = {
    "aux",
    "clock$",
    "com0",
    "com1",
    "com2",
    "com3",
    "com4",
    "com5",
    "com6",
    "com7",
    "com8",
    "com9",
    "comscsi",
    "lpt1",
    "lpt2",
    "lpt3",
    "lpt4",
    "lpt5",
    "lpt6",
    "lpt7",
    "lpt8",
    "lpt9",
    "lptno",
    "lptscsi",
    "con",
    "nul",
    "prn"
  };
  // Предикат для сравнения имён фалов.
  struct CompareFilename
  {
    CompareFilename(const string& filename)
      : Filename(filename)
    {
      transform(Filename.begin(), Filename.end(), Filename.begin(),
        [](char ch) -> char { return static_cast<char>(::tolower(ch)); });
    }

    bool operator() (const char* filename)
    {
      bool result = Filename.starts_with(filename);
      // Если введённое имя начинается с системного имени, то нужно проверить оставшиеся символы на присутствие
      // символов отличных от "."
      if (result != false)
      {
        auto it = Filename.begin();
        it += static_cast<string::difference_type>(strlen(filename));
        for (;it != Filename.end(); ++it)
          if (*it != '.')
          {
            // Имя файла начинается с системного имени, но далее есть символ не ".", т.е. это имя не системного файла.
            result = false;
            break;
          }
      }
      return result;
    }

  private:
    string Filename;
  };

  // Поиск переданного имени в списке системных файлов system_filename.
  return (find_if(
    &system_filename[0],
    &system_filename[sizeof(system_filename) / sizeof(char*)],
    CompareFilename(filename)) != &system_filename[sizeof(system_filename) / sizeof(char*)]);
}

bool IsTxtFile(const string& filename)
{
  if (filename.size() > 4)
  {
    string str(filename.substr(filename.size()-4));
    transform(str.begin(), str.end(), str.begin(), 
      [](char ch) -> char { return static_cast<char>(::tolower(ch)); });
    if (str == ".txt")
      return true;
  }
  return false;
}

void SplitToSentences(const string& str, list< list< string > >& sentences)
{
  // Установить начало поиска в начало переданной строки.
  auto start = str.begin();
  // Установить конец поиска в конец переданной строки.
  auto end = str.end();

  // Временный контейнер слов предложения.
  list<string> sentence;
  // Временный контейнер слова.
  string word;
  while (start != end)
  {
    // Поиск положения символа, который обозначает конец предложения.
    auto end_of_sentence = find_if(start, end, [](char ch) { return (ch == '.' || ch == '!' || ch == '?'); });
    // Разобрать получившееся предложение на слова.
    while (start != end_of_sentence)
    {
      // Ищем начало слова.
      start = find_if(start, end_of_sentence, [](char ch) { return (iswspace(static_cast<wint_t>(ch)) == 0); });
      // В предложении были слова?
      if (start != end_of_sentence)
      {
        // Ищем конец слова.
        auto end_of_word = find_if(start, end_of_sentence, [](char ch) { return (iswspace(static_cast<wint_t>(ch)) != 0); });
        {
          // Сохраняем слово.
          word.assign(start, end_of_word);
          // Добавляем во временный контейнер слов предложения.
          sentence.push_back(word);
          // Сдвигаем итератор начала поиска в точку за концом слова.
          start = end_of_word;
        }
      }
    }
    if (sentence.empty() == false)
    {
      // Добавляем список слов предложения в список предложений.
      sentences.push_back(sentence);
      // Отчищаем контейнер для следующего предложения.
      sentence.clear();
    }
    if (end_of_sentence != end)
      // Пропускаем символ обозначающий конец предложения.
      ++end_of_sentence;
    // Сдвигаем итератор начала поиска в точку за концом предложения.
    start = end_of_sentence;
  }
}

ptrdiff_t NumberOfOccurrences(const list< list < string > >& sentences, const string& word)
{
  ptrdiff_t c = 0;
  // Перебираем каждое предложение
  for (auto sentence : sentences)
    // Считаем количество вхождений слова в предложение и добавляем к общему числу.
    c += count(sentence.cbegin(), sentence.cend(), word);
  return c;
}

int main(int, char**)
{
  // Для корректного вывода кириллических символов в консоли.
  setlocale(LC_ALL, "Russian");

  string str;
  {
    string filename;
    do
    {
      cout << "Введите имя файла для чтения: ";
      cin >> filename;
      if (filename.empty() == false && IsSystemFilename(filename) != false)
      {
        cerr << "ОШИБКА: Неверное имя файла, т.к. это имя содержит зарезервированные системные имена." << endl;
        filename.clear();
      }
      if (filename.empty() == false && IsTxtFile(filename) == false)
      {
        cerr << "ОШИБКА: Указанное имя файла не содержит в конце имени \".txt\"." << endl;
        filename.clear();
      }
    } while (filename.empty());
    ifstream text(filename);
    if (text.is_open() == false)
    {
      cerr << "ОШИБКА: Нет файла \"" << filename << "\"." <<  endl;
      return 1;
    }
    // Чтение текстового файла в str.
    string line;
    while (getline(text, line))
    {
      str += line;
      str += "\r\n";
    }
    text.close();
  }
  // Разделить текст на список (предложения) списков слов.
  list< list <string> > sentences;
  SplitToSentences(str, sentences);
  if (sentences.empty() == false)
  {
    // Вывод первых слов каждого предложения.
    int i = 1;
    for (auto sentence : sentences)
    {
      cout << "Первое слово в предложении # " << i << " - \"" << *sentence.cbegin() << "\"" << endl;
      ++i;
    }
    // Подсчёт вхождений первых двух слов в тексте.
    auto sentence_it = sentences.begin();
    auto word = sentence_it->begin();
    for (i = 0; i < 2; ++i)
    {
      cout << "Количество вхождений в текст слова \"" << *word << "\": " << NumberOfOccurrences(sentences, *word) << endl;
      if (i > 0)
        break;
      ++word;
      if (word == sentence_it->end())
      {
        ++sentence_it;
        if (sentence_it == sentences.end())
        {
          cerr << "ПРЕДУПРЕЖДЕНИЕ: Нет второго слова." << endl;
          break;
        }
        word = sentence_it->begin();
      }
    }
  }
  return 0;
}
