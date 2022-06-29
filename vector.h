#include<iostream>
using namespace std;
namespace myvector
{
	template<class T>
	class vector{
		typedef T* Iterator;
		typedef const T* const_Iterator;
	public:
		//默认析构和构造，例如vector<int> a，不带参数;
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{}
		//正向迭代器
		const_Iterator cbegin() const {
			return _start;
		}
		const_Iterator cend() const {
			return _finish;
		}
		Iterator begin() {
			return _start;
		}
		Iterator end() {
			return _finish;
		}
		const T& front() const {
			return *begin();
		}
		const T& back() const {
			return *(_finish - 1);
		}
		//计算可用空间
		size_t capacity() const {
			return _end_of_storage - _start;
		}
		//计算已用大小
		size_t size() const {
			return _finish - _start;
		}
		//填充构造形式，例如vector<int> a(int n1, int n2)，
		//括号内从左到右分别代表构造个数和初始化值
		vector(int n, const T& val = T())
			:_start(new T[n])
			, _finish(_start)
			, _end_of_storage(_start+n) 
		{
			for (size_t i = 0; i < n; i++) {
				*_finish++ = val;
			}
		}
		//判断vector容器是否为空
		bool empty() {
			if (_start == _finish) return true;
			else return false;
		}
		//改变vector的容量到指定的n值
		void reserve(size_t n) {
			size_t current_capacity = capacity();
			size_t current_size = size();
			//n大于原vector的容量的话，则需要重新分配空间，若<=capacity，则不需要处理
			if (n > current_capacity) {
				Iterator new_size = new T[n];
				if (_start) {
					//用memcpy有读取权限的报错
					for (size_t i = 0; i < current_size; i++) {
						new_size[i] = _start[i];
					}
					delete[] _start;
				}
				_start = new_size;
				_finish = _start + current_size;
				_end_of_storage = _start + n;
			}
		}
		//改变容量大小 
		void resize(size_t n, const T& val) {
			size_t current_capacity = capacity();
			size_t current_size = size();
			if (n <= current_size) {
				for (size_t i = current_size; i > n; i--) {
					--_finish;
				}
			}
			else if (n > current_size) {
				if (n <= current_capacity) {
					for (size_t i = current_size; i < n; i++) {
						*_finish++ = val;
					}
				}
				else if (n > current_capacity) {
					Iterator new_vector = new T[n];
					memcpy(new_vector, _start, sizeof(T) * current_size);
					delete[] _start;
					_start = new_vector;
					_end_of_storage = _start + n;
					_finish = _start + current_size;
					for (size_t i = current_size; i < n; i++) {
						*_finish++ = val;
					}
				}
			}
		}
		//在最后的元素背后添加新的元素
		void push_back(const T& val) {
			if (_finish == _end_of_storage) {
				size_t new_capacity = capacity() == 0 ? 4 : capacity() * 2;
				reserve(new_capacity);
			}
			*_finish++ = val;
		}
		//删除最后一个元素
		void pop_back() {
			if (_start != _finish) {
				--_finish;
			}
		}
		//清除所有的元素
		void clear() {
			_finish = _start;
		}
		//删除指定位置的元素
		Iterator erase(Iterator position) {
			if (position<_start || position>_finish) {
				cout << "Warning! Out of range!" << endl;
				return end();
			}
			auto s = position + 1;
			while (s != _finish) {
				*(s - 1) = *s;
				s++;
			}
			_finish--;
			return position;
		}
		//在指定位置插入元素
		Iterator insert(Iterator position, const T& val) {
			if (position < _start || position > _finish) {
				cout << "Warning! Out of range!" << endl;
				return end();
			}
			if (_finish == _end_of_storage) {
				size_t diff_of_pos_start = position - _start;
				size_t current_size = size();
				size_t new_capacity = capacity() == 0 ? 4 : capacity() * 2;
				Iterator new_size = new T[new_capacity];
				if (_start) {
					for (size_t i = 0; i < current_size; i++) {
						new_size[i] = _start[i];
					}
					delete[] _start;
				}
				_start = new_size;
				position = _start + diff_of_pos_start;
				_finish = _start + current_size;
				_end_of_storage = _start + new_capacity;
			}
			auto* s = _finish - 1;
			while (s >= position) {
				*(s + 1) = *s;
				s--;
			}
			*position = val;
			++_finish;
			return position;
		}
		//输出容器内的元素
		void print_vector() {
			auto* s = begin();
			while (s != end()) {
				cout << *s << " ";
				s++;
			}
			cout << endl;
		}
		~vector(){
			if (_start){
				delete[] _start;//释放空间
			}
			_start = _finish = _end_of_storage = nullptr;//置空
		}
	private:
		Iterator _start;
		Iterator _finish;
		Iterator _end_of_storage;
	};
}
