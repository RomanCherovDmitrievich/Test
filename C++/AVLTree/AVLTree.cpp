#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <memory>
#include <algorithm>

using namespace std;

/// @brief Определить, состоит ли переданная из целых целое 
/// @param str Проверяемая строка.
/// @return Если строка содержит целое число, то возвращается \a true, иначе \a false.
bool is_integer(const string &str)
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

/// @brief  Именованное пространство в котором помещаются реализации объявленных методов за исключением специализаций.
namespace impl
{

  /// @brief Функция выделения памяти для контейнеров с двунаправленным итератором невозможна, поэтому её реализация пустая.
  /// @tparam Container Тип контейнера, для хранения элементов матрицы.
  /// @param container Контейнер со значениями матрицы.  
  /// @param columns Количество колонок в матрице.
  /// @param rows Количество строк в матрице.
  /// @param iterator_tag Классификация итератора.
  template <typename Container>
  void memory_reservation(
    Container &matrix,
    typename Container::size_type columns,
    typename Container::size_type rows,
    bidirectional_iterator_tag iterator_tag
    )
  {
    (void)matrix;
    (void)columns;
    (void)rows;
    (void)iterator_tag;
  }

  /// @brief Функция выделения памяти для контейнеров с итератором произвольного доступа.
  /// @tparam Container Тип контейнера, для хранения элементов матрицы.
  /// @param matrix Контейнер со значениями матрицы.  
  /// @param columns Количество колонок в матрице.
  /// @param rows Количество строк в матрице.
  /// @param iterator_tag Классификация итератора.
  template <typename Container>
  void memory_reservation(
    Container &matrix,
    typename Container::size_type columns,
    typename Container::size_type rows,
    random_access_iterator_tag iterator_tag
    )
  {
    (void)iterator_tag;
    matrix.reserve(columns * rows);
  }

}

/// @brief Зарезервировать динамическую память в контейнере для матрицы размером columns x rows.
/// @tparam Container Тип контейнера, для хранения элементов матрицы.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
template <typename Container>
void memory_reservation(Container &matrix, typename Container::size_type columns, typename Container::size_type rows)
{
  impl::memory_reservation(
    matrix,
    columns,
    rows,
    typename iterator_traits<typename Container::iterator>::iterator_category()
    );
}

/// @brief Заполнить матрицу значениями.
/// @tparam Container Тип контейнера, для хранения элементов матрицы.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
template <typename Container>
void fill_matrix(Container &matrix, typename Container::size_type columns, typename Container::size_type rows)
{
  memory_reservation(matrix, columns, rows);
  // srand(static_cast<unsigned int>(time(nullptr)));
  // typename Container::value_type value = static_cast<typename Container::value_type>(column * rows);
  //typename Container::value_type value = static_cast<typename Container::value_type>(1);
  for (typename Container::size_type y = 0; y < rows; ++y)
    for (typename Container::size_type x = 0; x < columns; ++x)
    {
      matrix.push_back(static_cast<typename Container::value_type>(x + 1));
      //matrix.push_back(value);
      //value -= 1;
      //value += 1;
    }
}

/// @brief Вывести значения матрицы в стандартный поток ввода-вывода. Элементы матрицы, которые присутствуют в контейнере 
/// \c highlight_iterators выводить красным цветом.
/// @tparam Container Тип контейнера, для хранения элементов матрицы.
/// @tparam ContainerIterators Тип контейнера для хранения итераторов элементов матрицы, которые нужно выделять при выводе.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
/// @param highlight_iterators Контейнер итераторов, которые указывают на элементы матрицы, которые нужно выделять при
/// выводе. 
template <typename Container, typename ContainerIterators>
void matrix_output(
  const Container &matrix,
  typename Container::size_type column,
  typename Container::size_type rows,
  const ContainerIterators &highlight_iterators
  )
{
  typename Container::const_iterator highlight;
  typename ContainerIterators::const_iterator highlight_iterator = highlight_iterators.begin();
  if (highlight_iterator == highlight_iterators.end())
    highlight = matrix.end();
  else
    highlight = *highlight_iterator;

  // Максимальное количество символов, которое возможно при выводе значения матрицы. Добавляем 1 символ для того, чтобы радом
  // стоящие значения матрицы не сливались.
  int width = static_cast<int>(log10(numeric_limits<typename Container::value_type>::max()) + 1);
  if constexpr(numeric_limits<typename Container::value_type>::is_signed == true)
    // Увеличить длину выводимой строки на 1 для вывода символа "-" для отрицательных значений.
    width += 1;

  typename Container::const_iterator it = matrix.begin();
  for (typename Container::size_type y = 0; y < rows; ++y)
  {
    for (typename Container::size_type x = 0; x < column; ++x)
    {
      if (highlight == it)
      {
        // Вывод элемента матрицы красным цветом.
        cout << "\033[91m" << setw(width) << *it << " \033[0m";
        // Сдвинуть на следующий элемент, который нужно выделять красным цветом.
        if (highlight_iterator == highlight_iterators.end())
          // Больше нет элементов, которые нужно выделять красным цветом.
          highlight = matrix.end();
        else
        {
          ++highlight_iterator;
          highlight = *highlight_iterator;
        }
      }
      else
      {
        if (y % 2 != 0)
          // Вывод элемента матрицы чётных строк зелёным цветом.
          cout << "\033[92m" << setw(width) << *it << " \033[0m";
        else
          // Вывод элементов матрицы нечётных строк серым цветом.
          cout << "\033[90m" << setw(width) << *it << " \033[0m";
      }
      ++it;
    }
    cout << endl;
  }
}

/// @brief Объявление функции преобразование строки в целое число.
/// @tparam T Тип целого числа.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <typename T>
void string_to_integer(const string &str, T &value);

/// @brief Специализация функции преобразования строки в целое число типа short.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, short &value)
{

  int tmp(stoi(str));
  if (tmp < static_cast<int>(numeric_limits<short>::min()) || tmp > static_cast<int>(numeric_limits<short>::max()))
    throw out_of_range("Значение не может быть записано в short");
  value = static_cast<short>(tmp);
}

/// @brief Специализация функции преобразования строки в целое число типа unsigned short.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, unsigned short &value)
{
  int tmp(stoi(str));
  if (tmp < 0 || tmp > static_cast<int>(numeric_limits<unsigned short>::max()))
    throw out_of_range("Значение не может быть записано в unsigned short");
  value = static_cast<unsigned short>(tmp);
}

/// @brief Специализация функции преобразования строки в целое число типа int.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, int &value)
{
  value = stoi(str);
}

/// @brief Специализация функции преобразования строки в целое число типа unsigned int.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, unsigned int &value)
{
  long tmp(stol(str));
  if (tmp < 0 || tmp > static_cast<long>(numeric_limits<unsigned int>::max()))
    throw out_of_range("Значение не может быть записано в unsigned int");
  value = static_cast<unsigned int>(tmp);
}

/// @brief Специализация функции преобразования строки в целое число типа long.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, long &value)
{
  value = stol(str);
}

/// @brief Специализация функции преобразования строки в целое число типа unsigned long.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, unsigned long &value)
{
  long long tmp(stoll(str));
  if constexpr(sizeof(unsigned long) < sizeof(long long))
  {
    // Если тип unsigned long меньше (по размеру) типа long long (модель LLP64 - Windows), то нужно проверять не только на
    // значение меньшее 0, но и с максимально допустимым значением типа unsigned long.
    if (tmp < 0 || tmp > static_cast<long long>(numeric_limits<unsigned long>::max()))
      throw out_of_range("Значение не может быть записано в unsigned long");
  }
  else
  {
    // Тип unsigned long и long long равны по размеру (модель LP64 - *nix), то проверять будем только на значение меньше 0,
    // т.к. максимальные значения может быть не более long long.
    if (tmp < 0)
      throw out_of_range("Значение не может быть записано в unsigned long");
  }
  value = static_cast<unsigned long>(tmp);
}

/// @brief Специализация функции преобразования строки в целое число типа long long.
/// @param str Преобразуемая строка.
/// @param value Ссылка на переменную для хранения целого числа.
template <>
void string_to_integer(const string &str, long long &value)
{
  value = stoll(str);
}

namespace impl
{

  /// @brief Реализация сдвига двунаправленного итератора \c it вперёд на \c count позиций, но не далее чем \c end.
  /// @tparam Iterator Тип итератора.
  /// @param it Итератор, который необходимо сдвинуть вперёд.
  /// @param end Итератор далее которого не нужно сдвигать итератор \c it.
  /// @param count Количество позиций на которые нужно сдвинуть итератор.
  /// @return Итоговое количество позиций на которое сдвинули итератор.
  /// @param iterator_tag Классификация итератора.
  template <typename Iterator>
  inline typename Iterator::difference_type positive_shift_iterator(
    Iterator &it, Iterator end,
    typename Iterator::difference_type count,
    bidirectional_iterator_tag iterator_tag
    )
  {
    (void)iterator_tag;
    typename Iterator::difference_type i = 0;
    for (; i < count && it != end; ++i)
      ++it;
    return i;
  }

  /// @brief Реализация сдвига итератора с произвольным доступом \c it вперёд на \c count позиций, но не далее чем \c end.
  /// @tparam Iterator Тип итератора.
  /// @param it Итератор, который необходимо сдвинуть вперёд.
  /// @param end Итератор далее которого не нужно сдвигать итератор \c it.
  /// @param count Количество позиций на которые нужно сдвинуть итератор.
  /// @return Итоговое количество позиций на которое сдвинули итератор.
  /// @param iterator_tag Классификация итератора.
  template <typename Iterator>
  inline typename Iterator::difference_type positive_shift_iterator(
    Iterator &it,
    Iterator end,
    typename Iterator::difference_type count,
    random_access_iterator_tag iterator_tag
    )
  {
    (void)iterator_tag;
    auto i = end - it;
    if (count <= i)
    {
      [[likely]];
      it += count;
      return count;
    }
    [[unlikely]];
    it += i;
    return i;
  }

}

/// @brief Выполнить сдвиг итератора \c it вперёд на \c count позиций, но не далее чем \c end.
/// @tparam Iterator Тип итератора.
/// @param it Итератор, который необходимо сдвинуть вперёд.
/// @param end Итератор далее которого не нужно сдвигать итератор \c it.
/// @param count Количество позиций на которые нужно сдвинуть итератор.
/// @return Итоговое количество позиций на которое сдвинули итератор.
template <typename Iterator>
inline typename Iterator::difference_type positive_shift_iterator(
  Iterator &it,
  Iterator end,
  typename Iterator::difference_type count
  )
{
  return impl::positive_shift_iterator(it, end, count, typename iterator_traits<Iterator>::iterator_category());
}

namespace impl
{

  /// @brief Реализация сдвига двунаправленного итератора \c it назад на \c count позиций, но не далее чем \c begin.
  /// @tparam Iterator Тип итератора.
  /// @param it Итератор, который необходимо сдвинуть назад.
  /// @param begin Итератор далее которого не нужно сдвигать итератор \c it.
  /// @param count Количество позиций на которые нужно сдвинуть итератор.
  /// @return Итоговое количество позиций на которое сдвинули итератор.
  /// @param iterator_tag Классификация итератора.
  template <typename Iterator>
  inline typename Iterator::difference_type negative_shift_iterator(
    Iterator &it,
    Iterator begin,
    typename Iterator::difference_type count,
    bidirectional_iterator_tag iterator_tag
    )
  {
    (void)iterator_tag;
    typename Iterator::difference_type n = 0;
    while (count > 0)
    {
      --count;
      ++n;
      if (it == begin)
      {
        [[unlikely]];
        break;
      }
      --it;
    }
    return n;
  }

  /// @brief Реализация сдвига итератора с произвольным доступом \c it назад на \c count позиций, но не далее чем \c begin.
  /// @tparam Iterator Тип итератора.
  /// @param it Итератор, который необходимо сдвинуть назад.
  /// @param begin Итератор далее которого не нужно сдвигать итератор \c it.
  /// @param count Количество позиций на которые нужно сдвинуть итератор.
  /// @return Итоговое количество позиций на которое сдвинули итератор.
  /// @param iterator_tag Классификация итератора.
  template <typename Iterator>
  inline typename Iterator::difference_type negative_shift_iterator(
    Iterator &it,
    Iterator begin,
    typename Iterator::difference_type count,
    random_access_iterator_tag iterator_tag
    )
  {
    (void)iterator_tag;
    auto i = it - begin;
    if (count <= i)
    {
      [[likely]];
      it -= count;
      return count;
    }
    [[unlikely]];
    it -= i;
    return i;
  }

}

/// @brief Выполнить сдвиг итератора \c it назад на \c count позиций, но не далее чем \c begin.
/// @tparam Iterator Тип итератора.
/// @param it Итератор, который необходимо сдвинуть назад.
/// @param begin Итератор далее которого не нужно сдвигать итератор \c it.
/// @param count Количество позиций на которые нужно сдвинуть итератор.
/// @return Итоговое количество позиций на которое сдвинули итератор.
template <typename Iterator>
inline typename Iterator::difference_type negative_shift_iterator(
  Iterator &it,
  Iterator begin,
  typename Iterator::difference_type count
  )
{
  return impl::negative_shift_iterator(it, begin, count, typename iterator_traits<Iterator>::iterator_category());
}

namespace impl
{

  /// @brief Преобразовать величину сдвига итератора \c value в количество шагов, которое необходимо выполнить для сдвига
  /// двунаправленного итератора.
  /// @tparam SizeType Тип описывающий величину сдвига итератора.
  /// @param value Величина на которую нужно сдвинуть итератор.
  /// @param iterator_tag Классификация итератора.
  /// @return Для двунаправленного итератора значение сдвига равно количеству шагов, которые нужно выполнить, чтобы перейти,
  /// поэтому возвращаемое значение всего равно \c value. 
  template <typename SizeType>
  inline SizeType shift_to_step(SizeType value, bidirectional_iterator_tag iterator_tag)
  {
    (void)iterator_tag;
    // Переход для двунаправленных итераторов выполняется за столько же шагов, насколько его нужно изменить.
    return value;
  }

  /// @brief Преобразовать величину сдвига итератора \c value в количество шагов, которое необходимо выполнить для сдвига
  /// итератора с произвольным доступом.
  /// @tparam SizeType Тип описывающий величину сдвига итератора.
  /// @param value Величина на которую нужно сдвинуть итератор.
  /// @param iterator_tag Классификация итератора.
  /// @return Для итераторов с произвольным доступов сдвиг осуществляется в один шаг, поэтому возвращаемое значение всего
  /// равно 1. 
  template <typename SizeType>
  inline SizeType shift_to_step(SizeType value, random_access_iterator_tag iterator_tag)
  {
    (void)value;
    (void)iterator_tag;
    // Переход для итераторов со свободным доступов выполняется в один шаг.
    return 1;
  }

}

/// @brief Преобразовать величину сдвига итератора \c value в количество шагов, которое необходимо выполнить для сдвига
/// итератора.
/// @tparam SizeType Тип описывающий величину сдвига итератора.
/// @tparam Iterator Тип итератора, который необходимо сдвинуть на \c value элементов.
/// @param value Величина сдвига итератора
/// @return Количество шагов, которые необходимо выполнить для сдвига итератора.
template <typename Container>
inline typename Container::size_type shift_to_step(typename Container::size_type value)
{
  return (value != 0 ?
    impl::shift_to_step(value, typename iterator_traits<typename Container::const_iterator>::iterator_category()) :
    0
    );
}

/// @brief Установить итератор конца поиска для последовательного перебора элементов из чётных строк матрицы. 
/// @tparam Container Тип для хранения элементов матрицы.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
/// @param end Ссылка на константный итератор куда записывается итератор конца поиска.
/// @return Количество переходов, которые выполнили для того, чтобы установить итератор конца поиска.
template <typename Container>
typename Container::size_type set_end_of_search(
  const Container &matrix, 
  typename Container::size_type columns,
  typename Container::size_type rows,
  typename Container::const_iterator &end
  )
{
  typename Container::size_type step_counter(0);
  // Установить итератор на конца матрицы.
  end = matrix.end();
  // В случае, если количество строк нечётное, то итератор не нужно сдвигать и он останется в matrix.end(), иначе итератор
  // будет указывать нужно сдвигать на элемент матрицы с индексом [1, rows-1].
  typename Container::difference_type shift_from_end =
    static_cast<typename Container::difference_type>(matrix.size() - (rows % 2 == 0 ? rows : rows - 1) * columns);
  if (shift_from_end > 0)
  {
    auto shift = negative_shift_iterator(end, matrix.begin(), shift_from_end);
    // Увеличить счётчик переходов.
    step_counter += shift_to_step<Container>(static_cast<typename Container::size_type>(shift));
    if (shift != shift_from_end)
    {
      // Итератор конца поиска не удалось сдвинуть на нужную позицию. Делаем предположение, что передан неверный параметр
      // rows, которые не соответствуют длине переданного контейнера. Рассчитываем количество строк матрицы исходя из длины
      // контейнера и количества колонок (columns).
      rows = matrix.size() / columns;
      if (rows <= 1)
        // Длина переданного контейнера короткая и в контейнере мало колонок. Это ошибка переданных аргументов.
        throw invalid_argument("Размер контейнера матрицы и количество колонок со строками некорректные.");
      // Повторно рассчитываем величину сдвига.
      shift_from_end =
        static_cast<typename Container::difference_type>(matrix.size() - (rows % 2 == 0 ? rows : rows - 1) * columns);
      // И делаем повторную попытку установить итератор конца поиска.
      end = matrix.end();
      shift = negative_shift_iterator(end, matrix.begin(), shift_from_end);
      if (shift != shift_from_end)
        throw std::runtime_error("Не удаётся установить итератор конца поиска в нужную позицию.");
      // Увеличить счётчик переходов.
      step_counter += shift_to_step<Container>(static_cast<typename Container::size_type>(shift));
    }
  }
  return step_counter;
}

/// @brief Установить итератор начала поиска для последовательного перебора элементов из чётных строк матрицы.
/// @tparam Container Тип для хранения элементов матрицы.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
/// @param end Итератор конца поиска.
/// @param start Ссылка на константный итератор куда записывается итератор начала поиска.
/// @return Количество переходов, которые выполнили для того, чтобы установить итератор начала поиска.
template <typename Container>
typename Container::size_type set_start_of_search(
  const Container &matrix,
  typename Container::size_type columns,
  typename Container::size_type rows,
  const typename Container::const_iterator &end,
  typename Container::const_iterator &start
  )
{
  (void)rows;
  start = matrix.begin();
  // Установить итератор на элемент матрицы с индексом [1, 2].
  auto shift = positive_shift_iterator(start, end, static_cast<typename Container::difference_type>(columns));
  auto step_counter = shift_to_step<Container>(static_cast<typename Container::size_type>(shift));
  if (static_cast<typename Container::size_type>(shift) != columns)
    // Переданный контейнер очень короткий, т.к. в нём количество элементов меньше columns.
    throw std::runtime_error("Не удаётся установить итератор начала поиска в нужную позицию.");
  return step_counter;
}

/// @brief Последовательный поиск значения в чётных строках матрицы \c matrix.
/// @tparam Container Тип для хранения элементов матрицы.
/// @tparam ContainerIterators Тип для хранения итераторов элементов матрицы в которых находится искомое значение.
/// @param matrix Контейнер со значениями матрицы.  
/// @param columns Количество колонок в матрице.
/// @param rows Количество строк в матрице.
/// @param search_value Искомое значение.
/// @param result Контейнер итераторов которые указывают на элементы матрицы в которых храниться искомое значение. 
/// @return Количество переходов выполненное при поиске значения \c search_value в матрице \c matrix.
template <typename Container, typename ContainerIterators>
typename ContainerIterators::size_type sequential_search(
  const Container &matrix,
  typename Container::size_type columns,
  typename Container::size_type rows,
  const typename Container::value_type &search_value,
  ContainerIterators &result
  )
{
  // !!! По условием задачи нужно искать значения в чётных строках, но индексация для человека начинается не с 0, а с 1. !!!
  // !!! Для решения этой задачи все условия определения чётности или нечётности строки инверсированы. !!!

  result.clear();

  typename ContainerIterators::size_type step_counter(0);
  typename Container::size_type column(0);
  typename Container::size_type row(1);
  if (row >= rows)
    // Слишком мало строк матрицы. Прекращаем поиск.
    return step_counter;

  typename Container::const_iterator end;
  // Установить итератор конца поиска. Количество переходов выполненные для этого действия прибавляем к текущему счётчику 
  // переходов.
  step_counter += set_end_of_search(matrix, columns, rows, end);

  typename Container::const_iterator start;
  // Установить итератор начала поиска на элемент матрицы с индексом [1, 2]. Количество переходов выполненные для этого
  // действия прибавляем к текущему счётчику переходов.
  step_counter += set_start_of_search(matrix, columns, rows, end, start);

  // Последовательный перебор значений в чётных строках матрицы. 
  typename Container::difference_type shift;
  while (row < rows)
  {
    while (column < columns)
    {
      if (*start == search_value)
        // Записать итератор в котором храниться искомое значение.
        result.push_back(start);
      // Продолжить поиск.
      ++column;
      shift = positive_shift_iterator(start, end, 1);
      // Добавляем количество переходов необходимые для сдвига итератора на 1 элемент.
      step_counter += shift_to_step<Container>(static_cast<typename Container::size_type>(shift));
    }
    // Т.к. после последнего перехода итератор указывает на начало следующей строки, то пропускать одну строку по условию
    // задачи.
    shift = positive_shift_iterator(start, end, static_cast<typename Container::difference_type>(columns));
    // Добавляем количество переходов необходимые для сдвига итератора на следующую строку матрицы.
    step_counter += shift_to_step<Container>(static_cast<typename Container::size_type>(shift));
    // Счётчик текущей строки увеличиваем на 2, т.е. пропускаем нечётную.
    row += 2;
    // Сбрасывает счётчик колонок для того, чтобы начать поиск с начало строки.
    column = 0;
  }
  return step_counter;
}

/// @brief Узел дерева
/// @tparam T Тип элемента дерева.
template<typename T>
struct Node
{
public:
  /// @brief Тип объекта хранящийся в узле AVL-дерева.
  typedef T value_type;

public:
  /// @brief Значение узла дерева. 
  vector< T > Values;
  /// @brief Ссылка на левый узел дерева.
  shared_ptr<Node> Left;
  /// @brief Ссылка на правый узел дерева.
  shared_ptr<Node> Right;
  /// @brief Высота узла в дереве.
  ptrdiff_t Height;

public:
  /// @brief Конструктор инициализации узла дерева.
  /// @param value Значение, которым нужно инициализировать узел.
  Node(const T &value) :
    Height(1)
  {
    Values.push_back(value);
  }
  /// @brief Деструктор.
  ~Node() = default;

private:
  // Закрыть конструкторы, операторы присваивания и перемещения, кроме конструктора инициализации, чтобы нельзя было создать
  // объект иным способом. 
  Node() = delete;
  Node(const Node &) = delete;
  Node(Node &&) = delete;
  Node &operator=(const Node &) = delete;
  Node &operator=(Node &&) = delete;
};

namespace impl
{

  /// @brief Вернуть высоту узла \c node в дереве.
  /// @tparam T Тип значения узла дерева.
  /// @param node Узел дерева.
  /// @return Высота узла дерева.
  template<typename T>
  ptrdiff_t height(const shared_ptr<Node<T> > &node)
  {
    if (node == nullptr)
      return 0;
    return node->Height;
  }

  /// @brief Вернуть балансный фактор узла \c node.
  /// @tparam T Тип значения узла дерева.
  /// @param node Узел дерева.
  /// @return Балансный фактор узла.
  template<typename T>
  ptrdiff_t getBalance(const shared_ptr<Node<T> > &node)
  {
    if (node == NULL)
      return 0;
    return height(node->Left) - height(node->Right);
  }

  /// @brief Ротация вправо поддерева с корнем \c node.
  /// @tparam T Тип значения узла дерева.
  /// @param node Узел дерева, для которого нужно выполнить ротацию вправо.
  /// @return Новый корень дерева.
  template<typename T>
  shared_ptr<Node<T> > rightRotate(const shared_ptr<Node<T> > &node)
  {
    shared_ptr<Node<T> > left = node->Left;
    shared_ptr<Node<T> > tmp = left->Right;

    // Выполнить ротации.
    left->Right = node;
    node->Left = tmp;
    
    // Обновить высоту дерева.
    node->Height = 1 + max(height(node->Left), height(node->Right));
    left->Height = 1 + max(height(left->Left), height(left->Right));

    return left;
  }

  /// @brief Ротация влево поддерева с корнем \c node.
  /// @tparam T Тип значения узла дерева.
  /// @param node Узел дерева, для которого нужно выполнить ротацию вправо.
  /// @return Новый корень дерева.
  template<typename T>
  shared_ptr<Node<T> > leftRotate(const shared_ptr<Node<T> >& node)
  {
    shared_ptr<Node<T> > right = node->Right;
    shared_ptr<Node<T> > tmp = right->Left;

    // Выполнить ротации.
    right->Left = node;
    node->Right = tmp;

    // Обновить высоту дерева.
    node->Height = 1 + max(height(node->Left), height(node->Right));
    right->Height = 1 + max(height(right->Left), height(right->Right));

    return right;
  }

  /// @brief Рекурсивная функция для вставки значения \c value в поддерево, корнем которого является узел \c node и ротацией
  /// нового корня поддерева.
  /// @tparam T Тип значения узла дерева.
  /// @tparam Compare Тип бинарного предиката сравнения двух значений на "меньше". 
  /// @param node Корневой узел дерева.
  /// @param value Значение, которое нужно вставить в дерево.
  /// @param cmp Объект бинарного предиката для сравнения значений на "меньше".
  /// @return Корневой узел дерева.
  template<typename T, typename Compare>
  shared_ptr<Node<T> > insert(shared_ptr<Node<T> > &node, const T &value, Compare cmp)  
  {
    // Выполните обычную вставку BST.
    if (node == nullptr)
    {
      node.reset(new Node<T>(value));
      return node;
    }

    if (cmp(value, node->Values.at(0)))
      node->Left = insert(node->Left, value, cmp);
    else if (cmp(node->Values.at(0), value))
      node->Right = insert(node->Right, value, cmp);
    else
    {
      node->Values.push_back(value);
      return node;
    }

    // Обновить высоту этого родительского узла.
    node->Height = 1 + max(impl::height(node->Left), impl::height(node->Right));

    // Получите коэффициент баланса этого узла-предка, чтобы проверить, стал ли этот узел несбалансированным.
    auto balance = impl::getBalance(node);

    // Если этот узел становится неуравновешенным, то возможны 4 случая.
    // Left-Left случай.
    if (balance > 1 && cmp(value, node->Left->Values.at(0)))
      return rightRotate(node);

    // Right-Right случай.
    if (balance < -1 && cmp(node->Right->Values.at(0), value))
      return leftRotate(node);

    // Left-Right случай.
    if (balance > 1 && cmp(node->Left->Values.at(0), value))
    {
      node->Left = leftRotate(node->Left);
      return rightRotate(node);
    }

    // Right-Left случай.
    if (balance < -1 && cmp(value, node->Right->Values.at(0)))
    {
      node->Right = rightRotate(node->Right);
      return leftRotate(node);
    }

    // Корневой узел дерева (неизмененный).
    return node;
  }

  /// @brief Бинарный предикат для сравнения значений на которые указывает итератор.
  /// @tparam Iterator 
  template<typename Iterator>
  struct LessIteratorCompare
  {
    /// @brief Сравнение значений двух итераторов.
    /// @param left Итератор расположенный с левой стороны сравнения. 
    /// @param right Итератор расположенный с правой стороны сравнения.
    /// @return Если \c left < \c right , то возвращается \a true иначе \a false.
    bool operator ()(const Iterator &left, const Iterator &right)
    {
      return (*left < *right);
    }
  };

}

/// @brief Построить AVL-дерево элементов матрицы расположенные в чётных строках. 
/// @tparam Container Тип контейнера значений матрицы.
/// @param matrix матрица.
/// @return Корневой узел AVL-дерева.
template<typename Container>
shared_ptr<Node<typename Container::const_iterator> > build_avl_tree(
  const Container& matrix,
  typename Container::size_type columns,
  typename Container::size_type rows
  )
{
  shared_ptr<Node<typename Container::const_iterator> > head;
  typename Container::size_type column(0);
  typename Container::size_type row(1);
  if (row >= rows)
    // Слишком мало строк матрицы. Прекращаем поиск.
    return head;

  typename Container::const_iterator end;
  // Установить итератор конца перебора элементов матрицы.
  set_end_of_search(matrix, columns, rows, end);

  typename Container::const_iterator start;
  // Установить итератор начала перебора элементов матрицы на индекс [1, 2].
  set_start_of_search(matrix, columns, rows, end, start);

  // Последовательный перебор значений в чётных строках матрицы. 
  while (row < rows)
  {
    while (column < columns)
    {
      head = insert(head, start, impl::LessIteratorCompare<typename Container::const_iterator>());
      ++column;
      positive_shift_iterator(start, end, 1);
    }
    // Сдвигаем итератор на следующую строку.
    positive_shift_iterator(start, end, static_cast<typename Container::difference_type>(columns));
    // Счётчик текущей строки увеличиваем на 2, т.е. пропускаем нечётную.
    row += 2;
    // Сбрасывает счётчик колонок для того, чтобы начать поиск с начало строки.
    column = 0;
  }
  return head;
}

/// @brief Выполнить поиск в AVL-дереве с корнем \c node значения \c search_value.
/// @tparam AVLNode Тип узла AVL-дерева.
/// @tparam ValueType Тип искомого значения.
/// @tparam ContainerIterators Тип для хранения итераторов элементов матрицы в которых находится искомое значение.
/// @param node Корневой узел AVL-дерева.
/// @param search_value Искомое значение.
/// @param result Контейнер итераторов которые указывают на элементы матрицы в которых храниться искомое значение. 
/// @return Количество переходов выполненное при поиске значения \c search_value в AVL-дереве с корнем \c node.
template <typename AVLNode, typename ValueType, typename ContainerIterators>
typename ContainerIterators::size_type avl_tree_search(
  const AVLNode &root,
  const ValueType &search_value,
  ContainerIterators &result
  )
{
  result.clear();
  typename ContainerIterators::size_type step_counter(0);

  AVLNode node(root);
  while (node != nullptr)
  {
    ++step_counter;
    if (*node->Values.at(0) < search_value)
      node = node->Right;
    else if (search_value < *node->Values.at(0))
      node = node->Left;
    else
    {
      // Если в узле записано значение, которое не меньше и не больше search_value, то это именно искомое значение.
      for(auto it = node->Values.begin(); it != node->Values.end(); ++it)
        result.push_back(*it);
      break;
    }
  }
  return step_counter;
}

/// @brief Выполнить поиск значения \c search_value в AVL-дереве с корнем \c node методом центрированного типа обхода.
/// @tparam AVLNode Тип узла AVL-дерева.
/// @tparam ValueType Тип искомого значения.
/// @tparam ContainerIterators Тип для хранения итераторов элементов матрицы в которых находится искомое значение.
/// @param node Корневой узел AVL-дерева.
/// @param search_value Искомое значение.
/// @param result Контейнер итераторов которые указывают на элементы матрицы в которых храниться искомое значение. 
/// @return Количество переходов выполненное при поиске значения \c search_value в AVL-дереве с корнем \c node.
template <typename AVLNode, typename ValueType, typename ContainerIterators>
typename ContainerIterators::size_type inorder_search(
  const AVLNode &node,
  const ValueType &search_value,
  ContainerIterators &result
  )
{
  result.clear();
  typename ContainerIterators::size_type step_counter(0);

  if (node != nullptr)
  {
    if (node->Left != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в левой части дерева.
      step_counter += inorder_search(node->Left, search_value, result);
      if (result.empty() == false)
        return step_counter;
    }
    // Сравнить значение корневого дерева.
    if (!(*node->Values.at(0) < search_value) && !(search_value < *node->Values.at(0)))
    {
      // Если в узле записано значение, которое не меньше и не больше search_value, то это именно искомое значение.
      for(auto it = node->Values.begin(); it != node->Values.end(); ++it)
        result.push_back(*it);
      return step_counter;
    }
    if (node->Right != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в правой части дерева.
      step_counter += inorder_search(node->Right, search_value, result);
    }
  }
  return step_counter;
}

/// @brief Выполнить поиск значения \c search_value в AVL-дереве с корнем \c node методом прямого обхода.
/// @tparam AVLNode Тип узла AVL-дерева.
/// @tparam ValueType Тип искомого значения.
/// @tparam ContainerIterators Тип для хранения итераторов элементов матрицы в которых находится искомое значение.
/// @param node Корневой узел AVL-дерева.
/// @param search_value Искомое значение.
/// @param result Контейнер итераторов которые указывают на элементы матрицы в которых храниться искомое значение. 
/// @return Количество переходов выполненное при поиске значения \c search_value в AVL-дереве с корнем \c node.
template <typename AVLNode, typename ValueType, typename ContainerIterators>
typename ContainerIterators::size_type preorder_search(
  const AVLNode &node,
  const ValueType &search_value,
  ContainerIterators &result
  )
{
  result.clear();
  typename ContainerIterators::size_type step_counter(0);

  if (node != nullptr)
  {
    // Сравнить значение корневого дерева.
    if (!(*node->Values.at(0) < search_value) && !(search_value < *node->Values.at(0)))
    {
      // Если в узле записано значение, которое не меньше и не больше search_value, то это именно искомое значение.
      for(auto it = node->Values.begin(); it != node->Values.end(); ++it)
        result.push_back(*it);
      return step_counter;
    }
    if (node->Left != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в левой части дерева.
      step_counter += preorder_search(node->Left, search_value, result);
      if (result.empty() == false)
        return step_counter;
    }
    if (node->Right != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в правой части дерева.
      step_counter += preorder_search(node->Right, search_value, result);
    }
  }
  return step_counter;
}

/// @brief Выполнить поиск значения \c search_value в AVL-дереве с корнем \c node методом обратного обхода.
/// @tparam AVLNode Тип узла AVL-дерева.
/// @tparam ValueType Тип искомого значения.
/// @tparam ContainerIterators Тип для хранения итераторов элементов матрицы в которых находится искомое значение.
/// @param node Корневой узел AVL-дерева.
/// @param search_value Искомое значение.
/// @param result Контейнер итераторов которые указывают на элементы матрицы в которых храниться искомое значение. 
/// @return Количество переходов выполненное при поиске значения \c search_value в AVL-дереве с корнем \c node.
template <typename AVLNode, typename ValueType, typename ContainerIterators>
typename ContainerIterators::size_type postorder_search(
  const AVLNode &node,
  const ValueType &search_value,
  ContainerIterators &result
  )
{
  result.clear();
  typename ContainerIterators::size_type step_counter(0);

  if (node != nullptr)
  {
    if (node->Left != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в левой части дерева.
      step_counter += postorder_search(node->Left, search_value, result);
      if (result.empty() == false)
        return step_counter;
    }
    if (node->Right != nullptr)
    {
      ++step_counter;
      // Посетить все узлы в правой части дерева.
      step_counter += postorder_search(node->Right, search_value, result);
      if (result.empty() == false)
        return step_counter;
    }
    // Сравнить значение корневого дерева.
    if (!(*node->Values.at(0) < search_value) && !(search_value < *node->Values.at(0)))
    {
      // Если в узле записано значение, которое не меньше и не больше search_value, то это именно искомое значение.
      for(auto it = node->Values.begin(); it != node->Values.end(); ++it)
        result.push_back(*it);
    }
  }
  return step_counter;
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  cout.imbue(locale("ru_RU.UTF-8"));
  try
  {
    typedef vector<short> MatrixContainer;
    typedef vector<MatrixContainer::const_iterator> ContainerIterators;

    typename MatrixContainer::size_type columns(0);
    typename MatrixContainer::size_type rows(0);
    MatrixContainer matrix;
    ContainerIterators found_iterators;
    MatrixContainer::value_type search_value(0);
    {
      bool flag(false);
      string str;
      const typename MatrixContainer::size_type max_columns = 100;
      do
      {
        cout << "Введите количество колонок от 1 до " << max_columns << ": ";
        cin >> str;

        flag = false;
        if (is_integer(str))
          try
          {
            string_to_integer(str, columns);
            if (columns > 0 && columns <= max_columns)
              flag = true;
          }
          catch (...)
          {
            flag = false;
          }
        if (flag == false)
          cout << "ОШИБКА: Введено неверное количество колонок." << endl;
      } while (!flag);

      const typename MatrixContainer::size_type max_rows = 100;
      do
      {
        cout << "Введите количество строк от 1 до " << max_rows << ": ";
        cin >> str;

        flag = false;
        if (is_integer(str))
          try
          {
            string_to_integer(str, rows);
            if (rows > 0 && rows <= max_rows)
              flag = true;
          }
          catch (...)
          {
            flag = false;
          }
        if (flag == false)
          cout << "ОШИБКА: Введено неверное количество строк." << endl;
      } while (!flag);

      fill_matrix(matrix, columns, rows);

      cout << "Оригинальная матрица." << endl;
      matrix_output(matrix, columns, rows, found_iterators);

      do
      {
        cout << "Введите искомое число от " << numeric_limits<typename MatrixContainer::value_type>::min()
          << " до " << numeric_limits<typename MatrixContainer::value_type>::max() << ": ";
        cin >> str;

        flag = false;
        if (is_integer(str))
          try
          {
            string_to_integer(str, search_value);
            flag = true;
          }
          catch (...)
          {
            cout << "ОШИБКА: Число за пределами допустимого значения." << endl;
          }
        else
          cout << "ОШИБКА: Неверно введено десятеричное целое число." << endl;
      } while (!flag);
    }

    typename MatrixContainer::size_type step_counter(0);
    step_counter = sequential_search(matrix, columns, rows, search_value, found_iterators);
    cout << "Последовательный поиск. Количество вхождений искомого числа в матрице: " << found_iterators.size()
      << ". Количество переходов: " << step_counter << "." << endl;
    matrix_output(matrix, columns, rows, found_iterators);

    // Построить AVL-дерево по матрице.
    auto avl_tree = build_avl_tree(matrix, columns, rows);

//    step_counter = avl_tree_search(avl_tree, search_value, found_iterators);
//    cout << "Поиск в AVL-дереве. Количество вхождений искомого числа в матрице: " << found_iterators.size()
//      << ". Количество переходов: " << step_counter << "." << endl;
//    matrix_output(matrix, columns, rows, found_iterators);

    step_counter = inorder_search(avl_tree, search_value, found_iterators);
    cout << "Поиск в AVL-дереве методом центрального обхода. Количество вхождений искомого числа в матрице: "
      << found_iterators.size() << ". Количество переходов: " << step_counter << "." << endl;
    matrix_output(matrix, columns, rows, found_iterators);

    step_counter = preorder_search(avl_tree, search_value, found_iterators);
    cout << "Поиск в AVL-дереве методом прямого обхода. Количество вхождений искомого числа в матрице: "
      << found_iterators.size() << ". Количество переходов: " << step_counter << "." << endl;
    matrix_output(matrix, columns, rows, found_iterators);

    step_counter = postorder_search(avl_tree, search_value, found_iterators);
    cout << "Поиск в AVL-дереве методом обратного обхода. Количество вхождений искомого числа в матрице: "
      << found_iterators.size() << ". Количество переходов: " << step_counter << "." << endl;
    matrix_output(matrix, columns, rows, found_iterators);
  }
  catch (const exception &e)
  {
    cout << "ОШИБКА: " << e.what() << endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    cout << "ОШИБКА: Неизвестная ошибка." << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
