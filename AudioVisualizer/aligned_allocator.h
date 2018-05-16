#pragma once

template <typename T, std::size_t Align = 16>
class aligned_allocator {
public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef T * pointer;
	typedef const T * const_pointer;

	typedef T & reference;
	typedef const T & const_reference;

public:
	inline aligned_allocator() throw () { }

	template <typename OtherType>
	inline aligned_allocator(const aligned_allocator<OtherType, Align> &) throw () { }

	inline ~aligned_allocator() throw () { }

	inline pointer adress(reference r) {
		return &r;
	}

	inline const_pointer adress(const_reference r) const {
		return &r;
	}

	inline pointer allocate(size_type n) {
		return (pointer)_aligned_malloc(n * sizeof(value_type), Align);
	}

	inline void deallocate(pointer p, size_type) {
		_aligned_free(p);
	}

	inline void construct(pointer p, const value_type & value) {
		new (p) value_type(value);
	}

	inline void destroy(pointer p) {
		p->~value_type();
	}

	inline size_type max_size() const throw () {
		return size_type(-1) / sizeof(value_type);
	}

	template <typename OtherType>
	struct rebind {
		typedef aligned_allocator<OtherType, Align> other;
	};

	bool operator!=(const aligned_allocator<T, Align>& other) const {
		return !(*this == other);
	}

	bool operator==(const aligned_allocator<T, Align>& other) const {
		return true;
	}
};