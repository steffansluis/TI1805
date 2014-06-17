#ifndef VEC2D_H
#define VEC2D_H


//2D vectorial computations 
#include <cmath>
#include <iostream>

template<typename T> class Vec2D;

template <class T> bool operator!= (const Vec2D<T> & p1, const Vec2D<T> & p2) {
	return (p1[0] != p2[0] || p1[1] != p2[1]);
}

template <class T> const Vec2D<T> operator* (const Vec2D<T> & p, float factor) {
	return Vec2D<T>(p[0] * factor, p[1] * factor);
}

template <class T> const Vec2D<T> operator* (float factor, const Vec2D<T> & p) {
	return Vec2D<T>(p[0] * factor, p[1] * factor);
}

template <class T> const Vec2D<T> operator* (const Vec2D<T> & p1, const Vec2D<T> & p2) {
	return Vec2D<T>(p1[0] * p2[0], p1[1] * p2[1]);
}

template <class T> const Vec2D<T> operator+ (const Vec2D<T> & p1, const Vec2D<T> & p2) {
	return Vec2D<T>(p1[0] + p2[0], p1[1] + p2[1]);
}

template <class T> const Vec2D<T> operator- (const Vec2D<T> & p1, const Vec2D<T> & p2) {
	return Vec2D<T>(p1[0] - p2[0], p1[1] - p2[1]);
}

template <class T> const Vec2D<T> operator- (const Vec2D<T> & p) {
	return Vec2D<T>(-p[0], -p[1]);
}

template <class T> const Vec2D<T> operator/ (const Vec2D<T> & p, float divisor) {
	return Vec2D<T>(p[0] / divisor, p[1] / divisor);
}

template <class T> bool operator== (const Vec2D<T> & p1, const Vec2D<T> & p2) {
	return (p1[0] == p2[0] && p1[1] == p2[1]);
}

template <class T> bool operator< (const Vec2D<T> & a, const Vec2D<T> & b) {
	return (a[0] < b[0] && a[1] < b[1]);
}

template <class T> bool operator>= (const Vec2D<T> & a, const Vec2D<T> & b) {
	return (a[0] >= b[0] || a[1] >= b[1]);
}


/**
* Vector in 3 dimensions, with basics operators overloaded.
*/
template <typename T>
class Vec2D {
public:
	inline Vec2D(void)	{
		p[0] = p[1] = T();
	}
	inline Vec2D(T p0, T p1) {
		p[0] = p0;
		p[1] = p1;
	};
	inline Vec2D(const Vec2D & v) {
		init(v[0], v[1]);
	}
	inline Vec2D(T* pp) {
		p[0] = pp[0];
		p[1] = pp[1];
	};
	// ---------
	// Operators
	// ---------
	inline T& operator[] (int Index) {
		return (p[Index]);
	};
	inline const T& operator[] (int Index) const {
		return (p[Index]);
	};
	inline Vec2D& operator= (const Vec2D & P) {
		p[0] = P[0];
		p[1] = P[1];
		return (*this);
	};
	inline Vec2D& operator+= (const Vec2D & P) {
		p[0] += P[0];
		p[1] += P[1];
		return (*this);
	};
	inline Vec2D& operator-= (const Vec2D & P) {
		p[0] -= P[0];
		p[1] -= P[1];
		return (*this);
	};
	inline Vec2D& operator*= (const Vec2D & P) {
		p[0] *= P[0];
		p[1] *= P[1];
		return (*this);
	};
	inline Vec2D& operator*= (T s) {
		p[0] *= s;
		p[1] *= s;
		return (*this);
	};
	inline Vec2D& operator/= (const Vec2D & P) {
		p[0] /= P[0];
		p[1] /= P[1];
		return (*this);
	};
	inline Vec2D& operator/= (T s) {
		p[0] /= s;
		p[1] /= s;
		return (*this);
	};

	//---------------------------------------------------------------

	inline Vec2D & init(T x, T y) {
		p[0] = x;
		p[1] = y;
		return (*this);
	};
	inline T getSquaredLength() const {
		return (dotProduct(*this, *this));
	};
	inline T getLength() const {
		return (T)sqrt(getSquaredLength());
	};
	/// Return length after normalization
	inline T normalize(void) {
		T length = getLength();
		if (length == 0.0f)
			return 0;
		T rezLength = 1.0f / length;
		p[0] *= rezLength;
		p[1] *= rezLength;
		return length;
	};
	inline void fromTo(const Vec2D & P1, const Vec2D & P2) {
		p[0] = P2[0] - P1[0];
		p[1] = P2[1] - P1[1];
	};
	inline float transProduct(const Vec2D & v) const {
		return (p[0] * v[0] + p[1] * v[1]);
	}
	static inline Vec2D segment(const Vec2D & a, const Vec2D & b) {
		Vec2D r;
		r[0] = b[0] - a[0];
		r[1] = b[1] - a[1];
		return r;
	};
	static inline T dotProduct(const Vec2D & a, const Vec2D & b) {
		return (a[0] * b[0] + a[1] * b[1]);
	}
	static inline T squaredDistance(const Vec2D &v1, const Vec2D &v2) {
		Vec2D tmp = v1 - v2;
		return (tmp.getSquaredLength());
	}
	static inline T distance(const Vec2D &v1, const Vec2D &v2) {
		Vec2D tmp = v1 - v2;
		return (tmp.getLength());
	}
	static inline Vec2D interpolate(const Vec2D & u, const Vec2D & v, T alpha) {
		return (u * (1.0f - alpha) + v * alpha);
	}

	//attention, this function might not do, what you expect if v2 is not normalized.
	//This is no bug, but wanted for some applications.
	static inline Vec2D projectOntoVector(const Vec2D & v1, const Vec2D & v2) {
		return v2 * dotProduct(v1, v2);
	}

	T * pointer()
	{
		return p;
	}

	const T * pointer() const
	{
		return p;
	}

	T p[2];
};

template <class T> inline Vec2D<T> swap(Vec2D<T> & P, Vec2D<T> & Q) {
	Vec2D<T> tmp = P;
	P = Q;
	Q = tmp;
}

template <class T> std::ostream & operator<< (std::ostream & output, const Vec2D<T> & v) {
	output << v[0] << " " << v[1];
	return output;
}

template <class T> std::istream & operator>> (std::istream & input, Vec2D<T> & v) {
	input >> v[0] >> v[1];
	return input;
}

typedef Vec2D<float> Vec2Df;
typedef Vec2D<double> Vec2Dd;
typedef Vec2D<int> Vec2Di;

#endif