#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (disable : 4365)
#pragma warning (disable : 4668)
#pragma warning (disable : 4710)
#pragma warning (disable : 4711)
#pragma warning (disable : 5045)
#endif

#include <iostream> 
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (default : 4711)
#pragma warning (default : 4710)
#pragma warning (default : 4668)
#pragma warning (default : 4365)
#endif

using namespace std;

bool is_integer(const string& str)
{
  bool result(false);
  if (!str.empty())
  {
    auto it = str.begin();
    if (*it == '-' or *it == '+')
      ++it;
    for (; it != str.end(); ++it)
      if (isdigit(*it) == false)
      {
        result = false;
        break;
      }
      else
        result = true;
  }
  return result;
}

chrono::year_month_day entering_birth_dates(long i)
{
  bool flag(false);
  string str;

  chrono::year y;
  do
  {
    cout << "Введите год рождения человека с номером " << i << ": ";
    cin >> str;

    flag = false;
    if (is_integer(str) && str.size() < 7)
    {
      // Не удалось заставить работать преобразователь строки в год. Стандартная реализации допускает ввод меньше
      // минимального и больше максимального года. При этом возвращает округлённое значение и не детектирует ошибку.
      // stringstream stream_str(str);
      // chrono::from_stream(stream_str, "%6Y", y);

      // Воспользуемся стандартным преобразователем строки в целое знаковое число.
      const int tmp = stoi(str);
      const auto now = chrono::system_clock::now();
      const auto today = floor< chrono::years >(now);
      // Год рождения может быть только знаковое целое число в пределах значений в пределах определённых в типе year.
      // Так же нельзя вводить нулевой год.
      if (
        (
          tmp >= static_cast<int>(y.min()) + today.time_since_epoch().count() + 1 &&
          tmp <= static_cast<int>(y.max())
          ) &&
        tmp != 0
        )
      {
        y = chrono::year(static_cast<int>(tmp));
        flag = y.ok();
      }
    }
    if (flag == false)
      cout << "ОШИБКА: Введён неверный год рождения." << endl;
  } while (!flag);

  chrono::year_month ym;
  chrono::month m;
  do
  {
    cout << "Введите номер месяца рождения человека с номером " << i << ": ";
    cin >> str;

    flag = false;
    if (is_integer(str))
    {
#ifdef _WIN32
      stringstream stream_str(str);
      chrono::from_stream(stream_str, "%2m", m);
#else
      unsigned mi = static_cast<unsigned>(stoi(str));
      chrono::month tmp(mi);
      m = tmp;
#endif
      ym = y / m;
      flag = m.ok() && ym.ok();
    }
    if (flag == false)
      cout << "ОШИБКА: Введён неверный номер месяца рождения." << endl;
  } while (!flag);

  chrono::year_month_day ymd;
  chrono::day d;
  do
  {
    cout << "Введите день в месяце рождения человека с номером " << i << ": ";
    cin >> str;

    flag = false;
    if (is_integer(str))
    {
#ifdef _WIN32
      stringstream stream_str(str);
      chrono::from_stream(stream_str, "%2d", d);
#else
      unsigned di = static_cast<unsigned>(stoi(str));
      chrono::day tmp(di);
      d = tmp;
#endif

      ymd = ym / d;
      flag = d.ok() && ymd.ok();
    }
    if (flag == false)
      cout << "ОШИБКА: Введён неверный день месяца рождения." << endl;
  } while (!flag);

  cout << "Дата рождения человека с номером " << i << ": "
    << static_cast<unsigned int>(ymd.day()) << "."
    << static_cast<unsigned int>(ymd.month()) << "."
    << static_cast<int>(ymd.year()) << endl;

  return ymd;
}

// Возраст вычисляется по модулю, т.е. если дата рождения введена больше чем сегодня, то возраст будет всё равно
// положительным.
chrono::year_month_day calc_age(const chrono::time_point<chrono::system_clock, chrono::days>& today,
  const chrono::year_month_day& birth_date)
{
  auto tmp_birth_date = birth_date;
  if (static_cast<int>(tmp_birth_date.year()) <= 0)
    // Т.к. нулевого года в Григорианском календаре нет, то если ввели нулевой или отрицательный год рождения, то
    // нужно увеличить год на 1, чтобы корректно рассчитать возраст (в днях).
    tmp_birth_date += chrono::years(1);
  const auto y = chrono::duration_cast<chrono::years>(
    today > chrono::sys_days(tmp_birth_date) ?
    today - chrono::sys_days(tmp_birth_date) :
    chrono::sys_days(tmp_birth_date) - today);
  const auto m = chrono::duration_cast<chrono::months>(
    today > chrono::sys_days(tmp_birth_date) ?
    today - chrono::sys_days(tmp_birth_date) - y :
    chrono::sys_days(tmp_birth_date) - today - y);
  const auto d = chrono::duration_cast<chrono::days>(
    today > chrono::sys_days(tmp_birth_date) ?
    today - chrono::sys_days(tmp_birth_date) - y - m :
    chrono::sys_days(tmp_birth_date) - today - y - m);
  return chrono::year_month_day(
    chrono::year(static_cast<int>(y.count())),
    chrono::month(static_cast<unsigned int>(m.count())),
    chrono::day(static_cast<unsigned int>(d.count())));
}

void print_age(const char* age_description, const chrono::year_month_day& age,
  const char* date_description, const chrono::year_month_day& date)
{
  cout << age_description;
  // Выводить года, если они больше 0.
  if (static_cast<int>(age.year()) > 0)
  {
    cout << " " << static_cast<int>(age.year()) << " ";
    if (static_cast<int>(age.year()) % 10 == 1 && static_cast<int>(age.year()) % 100 != 11)
      cout << "год";
    else if (static_cast<int>(age.year()) % 10 > 1 && static_cast<int>(age.year()) % 10 < 5)
      cout << "года";
    else
      cout << "лет";
  }
  // Выводить месяца, если они больше 0.
  if (static_cast<unsigned int>(age.month()) > 0)
  {
    cout << " " << static_cast<unsigned int>(age.month()) << " ";
    if (static_cast<unsigned int>(age.month()) % 10 == 1)
      cout << "месяц";
    else if (static_cast<unsigned int>(age.month()) % 10 > 1 && static_cast<unsigned int>(age.month()) % 10 < 5)
      cout << "месяца";
    else
      cout << "месяцев";
  }

  // Выводить дни, если они больше 0. Исключение - возраст равен 0 дней.
  if ((static_cast<unsigned int>(age.day()) > 0) or
    (
      static_cast<unsigned int>(age.day()) == 0 and
      static_cast<int>(age.year()) == 0 and
      static_cast<unsigned int>(age.month()) == 0
      ))
  {
    cout << " " << static_cast<unsigned int>(age.day()) << " ";
    if (
      static_cast<unsigned int>(age.day()) % 10 == 1 &&
      static_cast<unsigned int>(age.day()) % 100 != 11
      )
      cout << "день";
    else if (
      static_cast<unsigned int>(age.day()) % 10 > 1 &&
      static_cast<unsigned int>(age.day()) % 10 < 5 &&
      !(
        static_cast<unsigned int>(age.day()) % 100 > 11 &&
        static_cast<unsigned int>(age.day()) % 100 < 15
        )
      )
      cout << "дня";
    else
      cout << "дней";
  }

  cout << date_description
    << static_cast<unsigned int>(date.day()) << "."
    << static_cast<unsigned int>(date.month()) << "."
    << static_cast<int>(date.year()) << endl;
}

int main()
{
  // Для корректного вывода кириллических символов в консоли.
  setlocale(LC_ALL, "Russian");

  // Получение сегодняшней даты.
  auto now = chrono::system_clock::now();
  auto today = floor< chrono::days >(now);

  // Ввод дней рождения.
  vector< chrono::year_month_day > birth_dates;
  for (long i = 0; i < 2; ++i)
  {
    birth_dates.push_back(entering_birth_dates(i + 1));
  }

  // Поиск максимального и минимального возраста (в днях), а так же вычисление среднего возраста (в днях).
  chrono::year_month_day max_birth_date(chrono::year::max(), chrono::December, chrono::day(31));
  chrono::year_month_day max_age(chrono::year::min(), chrono::January, chrono::day(1));
  chrono::year_month_day min_birth_date(chrono::year::min(), chrono::January, chrono::day(1));
  chrono::year_month_day min_age(chrono::year::max(), chrono::December, chrono::day(31));
  int middle_age_in_days(0);
  for (auto it = birth_dates.begin(); it != birth_dates.end(); ++it)
  {
    const auto age = calc_age(today, *it);

    if (max_birth_date > *it)
    {
      max_age = age;
      max_birth_date = *it;
    }
    // Использовать тут else не стоит, т.к. даты могут вводиться по возрастанию и в этот блока кода не будем попадать,
    // т.е. не сможем определить минимальный возраст.
    if (min_birth_date < *it)
    {
      min_age = age;
      min_birth_date = *it;
    }
    if (static_cast<int>(it->year()) <= 0)
      // Т.к. нулевого года в Григорианском календаре нет, то если ввели нулевой или отрицательный год рождения, то
      // нужно увеличить год на 1, чтобы корректно рассчитать возраст (в днях).
      middle_age_in_days += static_cast<int>(
        chrono::duration_cast<chrono::days>(today - chrono::sys_days(*it + chrono::years(1))).count()
        );
    else
      middle_age_in_days += static_cast<int>(chrono::duration_cast<chrono::days>(today - chrono::sys_days(*it)).count());
  }
  // Вычисление среднего возраста (в днях).
  middle_age_in_days /= static_cast<int>(birth_dates.size());

  chrono::year_month_day middle_birth_date(chrono::sys_days(today - chrono::days(middle_age_in_days)));
  if (static_cast<int>(middle_birth_date.year()) <= 0)
    // Т.к. нулевого года в Григорианском календаре нет, то если в результате вычисления получился нулевой или
    // отрицательный год рождения, нужно уменьшить год на 1, чтобы корректно отобразить дату.
    middle_birth_date -= chrono::years(1);
  const auto middle_age = calc_age(today, middle_birth_date);
  print_age("Средний возраст:", middle_age, ". Дата рождения: ", middle_birth_date);
  print_age("Максимальный возраст:", max_age, ". Дата рождения: ", max_birth_date);
  print_age("Минимальный возраст:", min_age, ". Дата рождения: ", min_birth_date);

  return 0;
}

#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (default : 5045)
#endif
