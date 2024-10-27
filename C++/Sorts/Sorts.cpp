#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (disable : 4365)
#pragma warning (disable : 4668)
#pragma warning (disable : 4710)
#pragma warning (disable : 4711)
#pragma warning (disable : 5045)
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <functional>
#include <chrono>

#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (default : 4711)
#pragma warning (default : 4710)
#pragma warning (default : 4668)
#pragma warning (default : 4365)
#endif

using namespace std;

namespace impl
{

  template< typename Iterator >
  inline typename Iterator::difference_type positive_shift_iterator(
    Iterator& it,
    Iterator end,
    typename Iterator::difference_type count,
    bidirectional_iterator_tag
    )
  {
    typename Iterator::difference_type i = 0;
    for (; i < count && it != end; ++i)
      ++it;
    return i;
  }

  template< typename Iterator >
  inline typename Iterator::difference_type positive_shift_iterator(
    Iterator& it,
    Iterator end,
    typename Iterator::difference_type count,
    random_access_iterator_tag
    )
  {
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

  template< typename Iterator >
  inline typename Iterator::difference_type positive_shift_iterator(
    Iterator& it,
    Iterator end,
    typename Iterator::difference_type count
    )
  {
    return positive_shift_iterator(it, end, count, typename iterator_traits<Iterator>::iterator_category());
  }

  template< typename Iterator >
  inline typename Iterator::difference_type negative_shift_iterator(
    Iterator& it,
    Iterator begin,
    typename Iterator::difference_type count,
    bidirectional_iterator_tag
    )
  {
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

  template< typename Iterator >
  inline typename Iterator::difference_type negative_shift_iterator(
    Iterator& it,
    Iterator begin,
    typename Iterator::difference_type count,
    random_access_iterator_tag
    )
  {
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

  template< typename Iterator >
  inline typename Iterator::difference_type negative_shift_iterator(
    Iterator& it,
    Iterator begin,
    typename Iterator::difference_type count
    )
  {
    return negative_shift_iterator(it, begin, count, typename iterator_traits<Iterator>::iterator_category());
  }

  template< typename Iterator, typename Compare >
  pair<typename Iterator::difference_type, typename Iterator::difference_type> bubble_sort(
    Iterator first,
    Iterator last,
    Compare cmp,
    typename Iterator::difference_type step
    )
  {
    pair<typename Iterator::difference_type, typename Iterator::difference_type> res(0, 0);
    if (distance(first, last) > step)
    {
      Iterator begin1(first);
      Iterator end2(last);
      bool stop_flag = false;
      while (begin1 != last && stop_flag == false)
      {
        Iterator begin2(first);
        Iterator next(begin2);
        positive_shift_iterator(next, end2, step);
        // Делаем предположение, что все элементы отсортированы.
        stop_flag = true;
        while (next != end2)
        {
          ++res.first;
          if (!cmp(*begin2, *next))
          {
            // Одна пара значений была неупорядоченной, поэтому сбрасываем флаг остановки сортировки.
            stop_flag = false;
            ++res.second;
            iter_swap(begin2, next);
          }
          begin2 = next;
          positive_shift_iterator(next, last, step);
        }
        positive_shift_iterator(begin1, last, step);
        end2 = next;
      }
    }
    return res;
  }

}

template< typename Iterator >
inline pair<typename Iterator::difference_type, typename Iterator::difference_type> bubble_sort(
  Iterator first,
  Iterator last,
  typename Iterator::difference_type step = 1,
  bool sorting_direction = true
  )
{
  if (sorting_direction != false)
    return impl::bubble_sort(first, last, less_equal< typename Iterator::value_type >(), step);
  return impl::bubble_sort(first, last, greater_equal< typename Iterator::value_type >(), step);
}

namespace impl
{

  template< typename Iterator, typename Compare >
  pair<typename Iterator::difference_type, typename Iterator::difference_type> selection_sort(
    Iterator first,
    Iterator last,
    Compare cmp,
    typename Iterator::difference_type step
    )
  {
    pair<typename Iterator::difference_type, typename Iterator::difference_type> result(0, 0);
    auto i(first);
    while (i != last)
    {
      auto min(i);
      auto j(i);
      positive_shift_iterator(j, last, step);
      while (j != last)
      {
        ++result.first;
        if (cmp(*j, *min))
          min = j;
        positive_shift_iterator(j, last, step);
      }
      if (i != min)
      {
        ++result.second;
        iter_swap(i, min);
      }
      positive_shift_iterator(i, last, step);
    }
    return result;
  }

}

template< typename Iterator >
inline pair<typename Iterator::difference_type, typename Iterator::difference_type> selection_sort(
  Iterator first,
  Iterator last,
  typename Iterator::difference_type step = 1,
  bool sorting_direction = true
  )
{
  if (sorting_direction != false)
    return impl::selection_sort(first, last, less< typename Iterator::value_type >(), step);
  return impl::selection_sort(first, last, greater< typename Iterator::value_type >(), step);
}

namespace impl
{

  template< typename Iterator, typename Compare >
  pair<typename Iterator::difference_type, typename Iterator::difference_type> insertion_sort(
    Iterator first,
    Iterator last,
    Compare cmp,
    typename Iterator::difference_type step
    )
  {
    pair<typename Iterator::difference_type, typename Iterator::difference_type> result(0, 0);
    auto i(first);
    positive_shift_iterator(i, last, step);
    while (i != last)
    {
      auto tmp(*i);
      auto j(i);
      auto next_j(j);
      while (j != first)
      {
        next_j = j;
        negative_shift_iterator(j, first, step);
        ++result.first;
        if (cmp(tmp, *j))
        {
          ++result.second;
          *next_j = *j;
        }
        else
          break;
      }
      ++result.first;
      if (cmp(tmp, *j))
      {
        ++result.second;
        *j = tmp;
      }
      positive_shift_iterator(i, last, step);
    }

    return result;
  }

}

template< typename Iterator >
inline pair<typename Iterator::difference_type, typename Iterator::difference_type> insertion_sort(
  Iterator first,
  Iterator last,
  typename Iterator::difference_type step = 1,
  bool sorting_direction = true
  )
{
  if (sorting_direction != false)
    return impl::insertion_sort(first, last, less< typename Iterator::value_type >(), step);
  return impl::insertion_sort(first, last, greater< typename Iterator::value_type >(), step);
}

namespace impl
{

  template<typename Iterator, typename Compare>
  pair<typename Iterator::difference_type, typename Iterator::difference_type> shell_sort(
    Iterator first,
    Iterator last,
    Compare cmp,
    typename Iterator::difference_type step
    )
  {
    pair<typename Iterator::difference_type, typename Iterator::difference_type> result(0, 0);
    auto size = distance(first, last);
    if (size > step && size < step * 2)
      size = step * 2;
    size /= step;
    for (auto d = size / 2; d != 0; d /= 2)
    {
      auto i(first);
      auto d_step = d * step;
      positive_shift_iterator(i, last, d_step);
      while (i != last)
      {
        auto j(i);
        while ((distance(first, j) >= d_step))
        {
          auto cur(j);
          negative_shift_iterator(cur, first, d_step);
          ++result.first;
          if (cmp(*j, *cur))
          {
            ++result.second;
            iter_swap(j, cur);
          }
          else
            break;
          negative_shift_iterator(j, first, d_step);
        }
        positive_shift_iterator(i, last, step);
      }
    }
    return result;
  }

}

template<typename Iterator>
inline pair<typename Iterator::difference_type, typename Iterator::difference_type> shell_sort(
  Iterator first,
  Iterator last,
  typename Iterator::difference_type step = 1,
  bool sorting_direction = true
  )
{
  if (sorting_direction != false)
    return impl::shell_sort(first, last, less< typename Iterator::value_type >(), step);
  return impl::shell_sort(first, last, greater< typename Iterator::value_type >(), step);
}

namespace impl
{

  template< typename Iterator, typename Compare >
  pair<typename Iterator::difference_type, typename Iterator::difference_type> quick_sort(
    Iterator first,
    Iterator last,
    Compare cmp,
    typename Iterator::difference_type step
    )
  {
    pair<typename Iterator::difference_type, typename Iterator::difference_type> result(0, 0);
    auto size = distance(first, last);
    if (size > step)
    {
      Iterator left(first);
      Iterator right(last);
      Iterator pivot(left);
      positive_shift_iterator(left, last, step);
      auto last_left_shift = step;
      while (left != right)
      {
        ++result.first;
        if (cmp(*left, *pivot))
          last_left_shift = positive_shift_iterator(left, last, step);
        else
        {
          do
          {
            if (right != last)
            {
              [[likely]];
              negative_shift_iterator(right, first, step);
            }
            else
            {
              [[unlikely]];
              negative_shift_iterator(right, first, (size % step == 0 ? step : size - (size / step) * step));
            }
            ++result.first;
          } while ((left != right) && cmp(*pivot, *right));
          if (left != right && *left != *right)
          {
            ++result.second;
            iter_swap(left, right);
          }
        }
      }
      negative_shift_iterator(left, first, last_left_shift);
      if (first != left && *first != *left)
      {
        ++result.second;
        iter_swap(first, left);
      }
      
      auto recursive_result = quick_sort(first, left, cmp, step);
      result.first += recursive_result.first;
      result.second += recursive_result.second;
      recursive_result = quick_sort(right, last, cmp, step);
      result.first += recursive_result.first;
      result.second += recursive_result.second;
    }
    return result;
  }

}

template< typename Iterator >
inline pair<typename Iterator::difference_type, typename Iterator::difference_type> quick_sort(
  Iterator first,
  Iterator last,
  typename Iterator::difference_type step = 1,
  bool sorting_direction = true
  )
{
  if (sorting_direction != false)
    return impl::quick_sort(first, last, less< typename Iterator::value_type >(), step);
  return impl::quick_sort(first, last, greater< typename Iterator::value_type >(), step);
}

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

template< typename Container >
void fill_matrix(Container& c, typename Container::difference_type column, typename Container::difference_type rows)
{
  //srand(static_cast<unsigned int>(time(nullptr)));
  typename Container::value_type value = static_cast<typename Container::value_type>(column * rows);
  //typename Container::value_type value = static_cast<typename Container::value_type>(1);
  for (typename Container::difference_type x = 0; x < column; ++x)
    for (typename Container::difference_type y = 0; y < rows; ++y)
    {
      //c.push_back(static_cast<Container::value_type>(rand()) % 1000);
      c.push_back(value);
      value -= 1;
      //value += 1;
    }
}

template<typename Container>
void matrix_output(const Container& m, typename Container::difference_type column, typename Container::difference_type rows)
{
  int width = static_cast<int>(log10(numeric_limits<typename Container::value_type>::max()) + 1);
  if (numeric_limits<typename Container::value_type>::is_signed == true)
    // Увеличить длину выводимой строки на 1 для вывода символа "-" для отрицательных значений.
    width += 1;
  typename Container::const_iterator it = m.begin();
  for (typename Container::difference_type x = 0; x < column; ++x)
  {
    for (typename Container::difference_type y = 0; y < rows; ++y)
    {
      cout << setw(width) << *it++ << " ";
    }
    cout << endl;
  }
  cout << defaultfloat;
}

template<typename Container>
pair<typename Container::difference_type, typename Container::difference_type> sort_matrix(
  Container& container,
  typename Container::difference_type columns,
  typename Container::difference_type rows,
  function<
    pair<typename Container::difference_type, typename Container::difference_type>(
      typename Container::iterator,
      typename Container::iterator,
      typename Container::difference_type,
      bool)>& sort_method
  )
{
  typename Container::difference_type compare_count = 0;
  typename Container::difference_type swap_count = 0;
  typename Container::iterator begin(container.begin());
  typename Container::iterator end;
  for (auto row = 0; row < rows; ++row)
  {
    end = begin;
    impl::positive_shift_iterator(end, container.end(), columns);
    if (row % 2 != 0)
      ++begin;
    auto res = sort_method(begin, end, 2, true);
    compare_count += res.first;
    swap_count += res.second;
    begin = end;
  }
  begin = container.begin();
  end = container.end();
  for (auto column = 0; column < columns; ++column, ++begin)
  {
    typename Container::iterator first(begin);
    if (column % 2 == 0)
      impl::positive_shift_iterator(first, container.end(), columns);
    auto res = sort_method(first, end, columns * 2, false);
    compare_count += res.first;
    swap_count += res.second;
  }
  return make_pair(compare_count, swap_count);
}

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  try
  {
    setlocale(LC_ALL, "Russian");
    cout.imbue(locale("ru_RU.UTF-8"));

    typedef vector<short> Container;
    typedef typename Container::difference_type DifferenceType;
    typedef typename Container::iterator Iterator;

    DifferenceType columns(0);
    DifferenceType rows(0);
    {
      bool flag(false);
      string str;
      const DifferenceType max_columns = 100;
      do
      {
        cout << "Введите количество колонок от 1 до " << max_columns << ": ";
        cin >> str;

        flag = false;
        if (*str.begin() == '+')
          str.erase(str.begin());
        if (is_integer(str) && str.size() <= to_string(max_columns).size())
        {
          columns = stoi(str);
          if (columns > 0 && columns <= max_columns)
            flag = true;
        }
        if (flag == false)
          cout << "ОШИБКА: Введено неверное количество колонок." << endl;
      } while (!flag);
    
      const DifferenceType max_rows = 100;
      do
      {
        cout << "Введите количество строк от 1 до " << max_rows << ": ";
        cin >> str;

        flag = false;
        if (*str.begin() == '+')
          str.erase(str.begin());
        if (is_integer(str) && str.size() <= to_string(max_rows).size())
          try
          {
            rows = stoi(str);
            if (rows > 0 && rows <= max_rows)
              flag = true;
          }
          catch(...)
          {
            // stoi может генерировать исключительные ситуации. Если такое возникнет, то перехватываем его и оставляем flag
            // в прежнем состоянии.
          }
        if (flag == false)
          cout << "ОШИБКА: Введено неверное количество строк." << endl;
      } while (!flag);
    }

    Container m;
    fill_matrix(m, columns, rows);
    cout << "Оригинальная матрица." << endl;
    matrix_output(m, columns, rows);

    Container tmp;
    function<pair<DifferenceType, DifferenceType>(Iterator, Iterator, DifferenceType, bool)> sort_method;
    chrono::steady_clock::time_point start_time, end_time;

    tmp = m;
    sort_method = bubble_sort<Iterator>;
    start_time = chrono::steady_clock::now();
    auto bubble_result = sort_matrix(tmp, columns, rows, sort_method);
    end_time = chrono::steady_clock::now();
    auto bubble_elapsed = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << endl << "Матрица после сортировки методом bubble_sort. Сравнений = " << bubble_result.first
      << ". Обменов = " << bubble_result.second
      << ". Время сортировки матрицы = " << bubble_elapsed.count() << " наносекунд." << endl;
    matrix_output(tmp, columns, rows);

    tmp = m;
    sort_method = selection_sort<Iterator>;
    start_time = chrono::steady_clock::now();
    auto selection_result = sort_matrix(tmp, columns, rows, sort_method);
    end_time = chrono::steady_clock::now();
    auto selection_elapsed = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << endl << "Матрица после сортировки методом selection_sort. Сравнений = " << selection_result.first
      << ". Обменов = " << selection_result.second
      << ". Время сортировки матрицы = " << selection_elapsed.count() << " наносекунд."  << endl;
    matrix_output(tmp, columns, rows);

    tmp = m;
    sort_method = insertion_sort<Iterator>;
    start_time = chrono::steady_clock::now();
    auto insertion_result = sort_matrix(tmp, columns, rows, sort_method);
    end_time = chrono::steady_clock::now();
    auto insertion_elapsed = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << endl << "Матрица после сортировки методом insertion_sort. Сравнений = " << insertion_result.first
      << ". Обменов = " << insertion_result.second
      << ". Время сортировки матрицы = " << insertion_elapsed.count() << " наносекунд."  << endl;
    matrix_output(tmp, columns, rows);

    tmp = m;
    sort_method = shell_sort<Iterator>;
    start_time = chrono::steady_clock::now();
    auto shell_result = sort_matrix(tmp, columns, rows, sort_method);
    end_time = chrono::steady_clock::now();
    auto shell_elapsed = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << endl << "Матрица после сортировки методом shell_sort. Сравнений = " << shell_result.first
      << ". Обменов = " << shell_result.second
      << ". Время сортировки матрицы = " << shell_elapsed.count() << " наносекунд."  << endl;
    matrix_output(tmp, columns, rows);

    tmp = m;
    sort_method = quick_sort<Iterator>;
    start_time = chrono::steady_clock::now();
    auto quick_result = sort_matrix(tmp, columns, rows, sort_method);
    end_time = chrono::steady_clock::now();
    auto quick_elapsed = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << endl << "Матрица после сортировки методом quick_sort. Сравнений = " << quick_result.first
      << ". Обменов = " << quick_result.second
      << ". Время сортировки матрицы = " << quick_elapsed.count() << " наносекунд."  << endl;
    matrix_output(tmp, columns, rows);
  }
  catch (bad_alloc const&)
  {
    cout << "ОШИБКА: Недостаточно памяти для выполнения программы." << endl;
    return EXIT_FAILURE;
  }
  catch(...)
  {
    cout << "ОШИБКА: Неизвестная ошибка." << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

#if _MSC_VER >= 1920 && _MSC_VER <= 1929
#pragma warning (default : 5045)
#endif
