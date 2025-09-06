#pragma once

// pch.h
// ����Ԥ����ͷ�ļ���
// �·��г����ļ�������һ�Σ�����˽������ɵĹ������ܡ�
// ��Ҳ��Ӱ�� IntelliSense ���ܣ�����������ɺ�������������ܡ�
// ���ǣ�����˴��г����ļ��е��κ�һ��������֮���и��£����Ƕ��������±��롣
// �����ڴ˴����ҪƵ�����µ��ļ����⽫ʹ������������Ч��

// �ڴ˴����ҪԤ����ı�ͷ

// C++ ��׼�ⳣ��ͷ�ļ���ȫ

// ====== �������Թ��� / ͨ�ù��� ======
#include <iostream>   // ��׼��������� (cout, cin, endl)
#include <iomanip>    // �����������ʽ�� (setw, setprecision)
#include <ios>        // ����IO������
#include <iosfwd>     // ����IO���ǰ������

#include <algorithm>  // �㷨�� (sort, find, copy, transform)
#include <functional> // ��������������� (plus, less, bind, function)
#include <utility>    // ʵ�ù��� (pair, move, forward, swap)
#include <tuple>      // Ԫ�� (tuple)

#include <string>     // �ַ����� (string, wstring, stoi, to_string)
#include <string_view>// C++17 �ַ�����ͼ

#include <vector>     // ��̬���� (vector)
#include <array>      // C++11 �̶���С���� (array)
#include <list>       // ˫������ (list)
#include <forward_list>// C++11 �������� (forward_list)
#include <deque>      // ˫�˶��� (deque)

#include <set>        // ���� (set, multiset)
#include <map>        // ӳ�� (map, multimap)
#include <unordered_set> // C++11 ���򼯺� (unordered_set)
#include <unordered_map> // C++11 ����ӳ�� (unordered_map)

#include <stack>      // ջ (stack)
#include <queue>      // ���� (queue, priority_queue)

#include <iterator>   // ������ (begin, end, back_inserter)

#include <memory>     // ��̬�ڴ���� (unique_ptr, shared_ptr, weak_ptr, allocator)
#include <scoped_allocator> // C++11 Ƕ�׷�����

#include <type_traits>// C++11 �������� (is_integral, enable_if)
#include <typeindex>  // C++11 type_index
#include <typeinfo>   // ������Ϣ (typeid)

#include <exception>  // �쳣���� (exception)
#include <stdexcept>  // ��׼�쳣�� (runtime_error, out_of_range)

#include <system_error> // C++11 ϵͳ����֧��

// ====== ��ֵ���� ======
#include <limits>     // ��ֵ���� (numeric_limits)
#include <cmath>      // C�����ѧ���� (sqrt, pow, sin, cos)
#include <complex>    // ���� (complex)
#include <valarray>   // ��ֵ���� (valarray)
#include <random>     // C++11 ���������
#include <numeric>    // ��ֵ���� (accumulate, inner_product)
#include <ratio>      // C++11 ����������������

// ====== ʱ������� ======
#include <chrono>     // C++11 ʱ��� (system_clock, duration)
#include <ctime>      // C���ʱ�������

// ====== �ļ��������� ======
#include <fstream>    // �ļ��� (ifstream, ofstream, fstream)
#include <sstream>    // �ַ����� (istringstream, ostringstream)

// ====== ���̺߳Ͳ��� ======
#include <thread>     // C++11 �߳� (thread)
#include <mutex>      // C++11 ������ (mutex, lock_guard, unique_lock)
#include <atomic>     // C++11 ԭ�Ӳ��� (atomic)
#include <future>     // C++11 �첽���� (future, async, promise)
#include <condition_variable> // C++11 ��������

// ====== �������ù��� ======
#include <cassert>    // ���� (assert)
#include <cerrno>     // C�������� (errno)
#include <cctype>     // �ַ����� (isalpha, tolower)
#include <clocale>    // ���ػ����ú�C��񱾵ػ�
#include <locale>     // C++���ػ���ʩ (std::locale)
#include <codecvt>    // C++11 �ַ�����ת�� (�����ã�����ʱ����)

#include <cstddef>    // �������ͺͺ� (size_t, nullptr_t, offsetof)
#include <cstdint>    // C++11 �̶������������ (int8_t, uint32_t)
#include <cstdio>     // C���������� (printf, scanf)
#include <cstdlib>    // C���ͨ�ù��� (malloc, free, exit, rand)
#include <cstring>    // C����ַ������� (strlen, strcpy, memcpy)

#include <bitset>     // λ�� (bitset)
#include <regex>      // C++11 ������ʽ

// ע�⣺<codecvt> �� C++17 ���ѱ����ã�����ĳЩ�������汾���Կ�����Ҫ���ڱ���ת����
// �����´��룬����ʹ�õ������⣨�� iconv�����������ת����

#ifdef LH_PLATFORM_WINDOWS
	#include<Windows.h>
#endif