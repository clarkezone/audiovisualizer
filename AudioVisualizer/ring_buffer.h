#pragma once
#include <vector>

namespace buffers1
{
	template<typename T, size_t _Capacity> class _Iterator_Base_Fixed
	{
		typedef int difference_type;
	protected:
		T *_ptr;
		T *_base_ptr;


		void _Increment_Ptr()
		{
			if (_ptr < _base_ptr + _Capacity - 1)
				_ptr++;
			else
				_ptr = _base_ptr;
		}
		void _Decrement_Ptr()
		{
			if (_ptr > _base_ptr)
				_ptr--;
			else
				_ptr = _base_ptr + _Max_Size;
		}
		void _Add_Ptr(int offset)
		{
			int current_offset = (int)(_ptr - _base_ptr);
			int new_offset = current_offset + offset;
			if (new_offset < 0)
			{
				// Negative offset
				new_offset = (new_offset % (int)_Capacity) + (int)_Capacity;
			}
			else if (new_offset >= _Capacity)
			{
				// Past end
				new_offset = new_offset % _Capacity;
			}
			_ptr = _base_ptr + new_offset;
		}
		void _AssignFrom(const _Iterator_Base_Fixed<T, _Capacity> &iter)
		{
			_base_ptr = iter._base_ptr;
			_ptr = iter._ptr;
		}
		difference_type _Diff(const _Iterator_Base_Fixed<T, _Capacity> &_Right) const
		{
			return difference_type(_Right._ptr <= 
				_ptr ? _ptr - _Right._ptr : 
				difference_type(_Capacity) + _ptr - _Right._ptr);
		}

		_Iterator_Base_Fixed() : _ptr(nullptr), _base_ptr(nullptr)
		{
		}

		_Iterator_Base_Fixed(T *base, T *ptr) : _base_ptr(base), _ptr(ptr)
		{
		}
	public:
		bool operator==(const _Iterator_Base_Fixed<T, _Capacity> &_Right) const
		{      // test for iterator equality
			return (_ptr == _Right._ptr);
		}

		bool operator!=(const _Iterator_Base_Fixed<T, _Capacity> &_Right) const
		{      // test for iterator inequality
			return !(_ptr != _Right._ptr);
		}
	};
	template<typename T, size_t _Capacity> class _Buffer_Iterator : public _Iterator_Base_Fixed<T, _Capacity>
	{
		typedef std::forward_iterator_tag iterator_category;
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type *pointer;
		typedef int difference_type;

	public:
		_Buffer_Iterator()
		{}
		_Buffer_Iterator(T* _base_ptr) : _Iterator_Base_Fixed(_base_ptr, _base_ptr)
		{
		}
		_Buffer_Iterator(T* _base_ptr, int offset) : _Iterator_Base_Fixed(_base_ptr, _base_ptr)
		{
			_Add_Ptr(offset);
		}

		reference operator*() const
		{
			return (*_ptr);
		}

		_Buffer_Iterator& operator++()
		{
			// preincrement
			_Increment_Ptr();
			return (*this);
		}

		_Buffer_Iterator operator++(int)
		{
			// postincrement
			_Buffer_Iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_Buffer_Iterator operator+(const difference_type offset) const
		{

			_Buffer_Iterator _Tmp = *this;
			_Tmp._Add_Ptr(offset);
			return _Tmp;
		}
		_Buffer_Iterator operator-(const difference_type offset) const
		{
			return *this + (-offset);
		}
		_Buffer_Iterator<T, _Capacity> &operator+=(const difference_type offset)
		{
			_Add_Ptr(offset);
			return *this;
		}
		_Buffer_Iterator<T, _Capacity> &operator-=(const difference_type offset)
		{
			_Add_Ptr(-offset);
			return *this;
		}

		difference_type operator-(const _Iterator_Base_Fixed<T, _Capacity> &_Right) const
		{
			return _Diff(_Right);
		}
	};
	template<typename T, size_t _Capacity> class _Buffer_Const_Iterator : public _Iterator_Base_Fixed<T, _Capacity>
	{
		typedef std::forward_iterator_tag iterator_category;
		typedef const T value_type;
		typedef value_type &reference;
		typedef value_type *pointer;
		typedef int difference_type;

	public:
		_Buffer_Const_Iterator()
		{
		}
		_Buffer_Const_Iterator(T* _base_ptr) : _Iterator_Base_Fixed(_base_ptr, _base_ptr)
		{
		}
		// Allow asigning from a non-const iterator
		_Buffer_Const_Iterator(const _Buffer_Iterator<T, _Capacity> &iter)
		{
			_AssignFrom(iter);
		}

		_Buffer_Const_Iterator(T* _base_ptr, int offset) : _Iterator_Base_Fixed(_base_ptr, _base_ptr)
		{
			_Add_Ptr(offset);
		}
		reference operator*() const
		{
			return (*_ptr);
		}

		_Buffer_Const_Iterator<T, _Capacity> & operator++()
		{
			// preincrement
			_Increment_Ptr();
			return (*this);
		}

		_Buffer_Const_Iterator<T, _Capacity> operator++(int)
		{
			// postincrement
			_Buffer_Const_Iterator<T, _Capacity> _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_Buffer_Const_Iterator<T, _Capacity> operator+(const difference_type offset) const
		{
			_Buffer_Const_Iterator<T, _Capacity> _Tmp = *this;
			_Tmp._Add_Ptr(offset);
			return _Tmp;
		}
		_Buffer_Const_Iterator<T, _Capacity> &operator+=(const difference_type offset)
		{
			_Add_Ptr(offset);
			return *this;
		}
		_Buffer_Const_Iterator<T, _Capacity> &operator-=(const difference_type offset)
		{
			_Add_Ptr(-offset);
			return *this;
		}
		_Buffer_Const_Iterator<T, _Capacity> operator-(const difference_type offset) const
		{
			return *this + (-offset);
		}
		difference_type operator-(const _Iterator_Base_Fixed<T, _Capacity> &_Right) const
		{
			return _Diff(_Right);
		}

	};

	template<typename T, size_t _Capacity> class ring_buffer
	{
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type *pointer;
		typedef value_type const *const_pointer;
		typedef int difference_type;
		typedef typename _Buffer_Iterator<T, _Capacity> iterator;
		typedef typename _Buffer_Const_Iterator<T, _Capacity> const_iterator;

	private:
		T *_ptr;
		iterator _writer;
		const_iterator _reader;
	public:
		ring_buffer()
		{
			_ptr = static_cast<T*>(malloc(sizeof(T) * _Capacity));
			memset(_ptr, 0, sizeof(T) * _Capacity);
			_writer = iterator(_ptr);
			_reader = _writer;
		}
		~ring_buffer()
		{
			free(_ptr);
		}
		iterator &writer() { return _writer; }
		const_iterator &reader() { return _reader; }
		bool empty() { return _writer == _reader; }
		void clear()
		{
			_reader = _writer;
		}
		size_t size()
		{
			return _writer - _reader;
		}
		// Copy items to buffer
		void insert(const_pointer pSource, size_t count)
		{
			_SCL_SECURE_VALIDATE(count < _Capacity - size() - 1);	// Buffer overflow
			for (size_t i = 0; i < count; i++)
			{
				*(_writer++) = pSource[i];
			}
		}

		// Window, deinterleave and copy values
		bool get(pointer pTarget, size_t source_count, size_t channels, size_t blockSize, size_t overlap, const_pointer pWindow)
		{
			if ((source_count * channels) > size())	// Not enough items in the buffer
				return false;

			auto source = reader() - (difference_type)(overlap * channels);

			size_t frames_to_copy = source_count + overlap;

			T* pDest = pTarget;
			for (size_t i = 0; i < blockSize; i++, pDest++)
			{
				for (size_t channel_index = 0; channel_index < channels; channel_index++)
				{
					if (i < frames_to_copy)
						pDest[channel_index * blockSize] = *(source++) * pWindow[i];
					else
						pDest[channel_index * blockSize] = 0;	// Pad block with zeros
				}
			}
			_reader = _reader + (difference_type) (source_count * channels);

			return true;
		}
	};
}

#pragma once
