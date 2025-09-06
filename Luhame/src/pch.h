#pragma once

// pch.h
// 这是预编译头文件。
// 下方列出的文件仅编译一次，提高了将来生成的构建性能。
// 这也会影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

// 在此处添加要预编译的标头

// C++ 标准库常用头文件大全

// ====== 核心语言功能 / 通用工具 ======
#include <iostream>   // 标准输入输出流 (cout, cin, endl)
#include <iomanip>    // 输入输出流格式化 (setw, setprecision)
#include <ios>        // 基础IO流类型
#include <iosfwd>     // 常用IO类的前向声明

#include <algorithm>  // 算法库 (sort, find, copy, transform)
#include <functional> // 函数对象和适配器 (plus, less, bind, function)
#include <utility>    // 实用工具 (pair, move, forward, swap)
#include <tuple>      // 元组 (tuple)

#include <string>     // 字符串类 (string, wstring, stoi, to_string)
#include <string_view>// C++17 字符串视图

#include <vector>     // 动态数组 (vector)
#include <array>      // C++11 固定大小数组 (array)
#include <list>       // 双向链表 (list)
#include <forward_list>// C++11 单向链表 (forward_list)
#include <deque>      // 双端队列 (deque)

#include <set>        // 集合 (set, multiset)
#include <map>        // 映射 (map, multimap)
#include <unordered_set> // C++11 无序集合 (unordered_set)
#include <unordered_map> // C++11 无序映射 (unordered_map)

#include <stack>      // 栈 (stack)
#include <queue>      // 队列 (queue, priority_queue)

#include <iterator>   // 迭代器 (begin, end, back_inserter)

#include <memory>     // 动态内存管理 (unique_ptr, shared_ptr, weak_ptr, allocator)
#include <scoped_allocator> // C++11 嵌套分配器

#include <type_traits>// C++11 类型特征 (is_integral, enable_if)
#include <typeindex>  // C++11 type_index
#include <typeinfo>   // 类型信息 (typeid)

#include <exception>  // 异常基类 (exception)
#include <stdexcept>  // 标准异常类 (runtime_error, out_of_range)

#include <system_error> // C++11 系统错误支持

// ====== 数值计算 ======
#include <limits>     // 数值极限 (numeric_limits)
#include <cmath>      // C风格数学函数 (sqrt, pow, sin, cos)
#include <complex>    // 复数 (complex)
#include <valarray>   // 数值数组 (valarray)
#include <random>     // C++11 随机数生成
#include <numeric>    // 数值运算 (accumulate, inner_product)
#include <ratio>      // C++11 编译期有理数算术

// ====== 时间和日期 ======
#include <chrono>     // C++11 时间库 (system_clock, duration)
#include <ctime>      // C风格时间和日期

// ====== 文件和流操作 ======
#include <fstream>    // 文件流 (ifstream, ofstream, fstream)
#include <sstream>    // 字符串流 (istringstream, ostringstream)

// ====== 多线程和并发 ======
#include <thread>     // C++11 线程 (thread)
#include <mutex>      // C++11 互斥量 (mutex, lock_guard, unique_lock)
#include <atomic>     // C++11 原子操作 (atomic)
#include <future>     // C++11 异步操作 (future, async, promise)
#include <condition_variable> // C++11 条件变量

// ====== 其他常用工具 ======
#include <cassert>    // 断言 (assert)
#include <cerrno>     // C风格错误码 (errno)
#include <cctype>     // 字符分类 (isalpha, tolower)
#include <clocale>    // 本地化设置和C风格本地化
#include <locale>     // C++本地化设施 (std::locale)
#include <codecvt>    // C++11 字符编码转换 (已弃用，但有时仍需)

#include <cstddef>    // 基础类型和宏 (size_t, nullptr_t, offsetof)
#include <cstdint>    // C++11 固定宽度整数类型 (int8_t, uint32_t)
#include <cstdio>     // C风格输入输出 (printf, scanf)
#include <cstdlib>    // C风格通用工具 (malloc, free, exit, rand)
#include <cstring>    // C风格字符串操作 (strlen, strcpy, memcpy)

#include <bitset>     // 位集 (bitset)
#include <regex>      // C++11 正则表达式

// 注意：<codecvt> 在 C++17 中已被弃用，但在某些编译器版本中仍可能需要用于编码转换。
// 对于新代码，建议使用第三方库（如 iconv）来处理编码转换。

#ifdef LH_PLATFORM_WINDOWS
	#include<Windows.h>
#endif