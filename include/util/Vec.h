#ifndef VEC_H
#define VEC_H

#include <sstream>
#include <cmath>

/** Basic 2D vector class so that stuff need not depend on IrrLicht's vector class. */
template <typename N>
class Vector {
public:
	/** Creates a new Vector at the origin. */
	Vector(): x(0), y(0) { }
	/** Creates a new Vector with the given coordinates. */
	Vector(N x, N y): x(x), y(y) { }
	/** Creates a new Vector from any vector-like. */
	template<typename M>
	Vector(Vector<M> vec): x(vec.x), y(vec.y) { }

	/** Implicit conversion Vector to any vector-like */
	template<typename M>
	operator Vector<M>() const {
		Vector<M> v;
		v.x = x;
		v.y = y;
		return v;
	}

	/** @return The unit vector facing the given angle. */
	static Vector<N> withAngle(double angle) {
		return Vector<N>(cos(angle), sin(angle));
	}

	template<class V>
	static Vector<N> fromIrr(V vec) {
		return Vector<N>(vec.X, vec.Y);
	}
	template<class D>
	static Vector<N> fromIrrDim(D dim) {
		return Vector<N>(dim.Width, dim.Height);
	}


	/** @return The dot product of this vector and rhs. */
	N dot(Vector<N> rhs) const {
		return x * rhs.x + y * rhs.y;
	}

	/** @return The magnitude of the cross product of this vector and rhs. */
	N cross(Vector<N> rhs) const {
		return x * rhs.y - rhs.x * y;
	}


	/** @return The magnitude of this vector. */
	double length() const {
		return sqrt(x * x + y * y);
	}

	/** @return The squared magnitude of this vector. */
	N sqrLength() const {
		return x * x + y * y;
	}

	/** @return This vector normalized to a unit vector. */
	Vector<double> normalize() const {
		double len = length();
		if (len == 0) return Vector<double>(0, 1);
		return Vector<double>(x / len, y / len);
	}

	/** @return This vector rounded down to an int vector. */
	Vector<int> floor() const {
		return Vector<int>((int)::floor(x), (int)::floor(y));
	}

	/** @return The angle in radians of this vector, in range [-pi, pi]. */
	double angle() const {
		return atan2(y, x);
	}


	/** @return Whether or not the coordinates of this Vector and other are within threshold of each other. */
	bool equals(Vector other, double threshold) const {
		return std::abs(x - other.x) < threshold && std::abs(y - other.y) < threshold;
	}


	/** Just adds the components. */
	Vector<N>& operator+=(const Vector<N>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	/** Just subtracts the components. */
	Vector<N>& operator-=(const Vector<N>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	/** Just adds rhs to each component. */
	Vector<N>& operator+=(N rhs) {
		x += rhs;
		y += rhs;
		return *this;
	}
	/** Just subtracts rhs from each component. */
	Vector<N>& operator-=(N rhs) {
		x -= rhs;
		y -= rhs;
		return *this;
	}
	/** Just multiplies each component by rhs. */
	Vector<N>& operator*=(N rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}
	/** Just divides each component by rhs. */
	Vector<N>& operator/=(N rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}
	/** Just returns the vector in the opposite direction. */
	Vector<N> operator-() {
		return Vector<N>(-x, -y);
	}

	/** x is at [0] and y is at [1] @{ */
	N& operator[](int idx) {
		if (idx) return y;
		return x;
	}
	N operator[](int idx) const {
		if (idx) return y;
		return x;
	}
	///@}

	/**
	 * Used for indexing 1D arrays representing 2D grids. One works/make sense when
	 * used with an integer vector.
	 * @param width The width of the grid.
	 */
	inline N idx(N width) {
		return x + y * width;
	}


	N x; ///< X coordinate of the vector.
	N y; ///< Y coordinate of the vector.
};

template<typename N>
inline bool operator==(const Vector<N>& lhs, const Vector<N>& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
template<typename N>
inline bool operator!=(const Vector<N>& lhs, const Vector<N>& rhs) {
	return !operator==(lhs, rhs);
}

template<typename N>
inline Vector<N> operator+(Vector<N> lhs, const Vector<N>& rhs) {lhs += rhs; return lhs;}
template<typename N>
inline Vector<N> operator-(Vector<N> lhs, const Vector<N>& rhs) {lhs -= rhs; return lhs;}
template<typename N>
inline Vector<N> operator+(Vector<N> lhs, double rhs) {lhs += rhs; return lhs;}
template<typename N>
inline Vector<N> operator-(Vector<N> lhs, double rhs) {lhs -= rhs; return lhs;}
template<typename N>
inline Vector<N> operator*(Vector<N> lhs, double rhs) {lhs *= rhs; return lhs;}
template<typename N>
inline Vector<N> operator/(Vector<N> lhs, double rhs) {lhs /= rhs; return lhs;}

template<typename N>
std::ostream& operator<<(std::ostream& os, const Vector<N>& obj) {
	os << "(" << obj.x << ", " << obj.y << ")";
	return os;
}



/** Basic 3D vector class so that stuff need not depend on IrrLicht's vector class. */
template <typename N>
class Vector3 {
public:
	/** Creates a new Vector3 at the origin. */
	Vector3(): x(0), y(0), z(0) { }
	/** Creates a new Vector3 with the given coordinates. */
	Vector3(N x, N y, N z): x(x), y(y), z(z) { }
	/** Creates a new Vector3 from any vector-like. */
	template<typename M>
	Vector3(Vector3<M> vec): x(vec.x), y(vec.y), z(vec.z) { }

	/** Implicit conversion Vector3 to any vector-like */
	template<typename M>
	operator Vector3<M>() const {
		Vector3<M> v;
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

	template<class V>
	static Vector3<N> fromIrr(V vec) {
		return Vector3<N>(vec.X, vec.Y, vec.Z);
	}


	/** @return The dot product of this vector and rhs. */
	N dot(Vector3<N> rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	/** @return The cross product of this vector and rhs. */
	N cross(Vector3<N> rhs) const {
		return Vector3<N>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.z);
	}


	/** @return The magnitude of this vector. */
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	/** @return The squared magnitude of this vector. */
	N sqrLength() const {
		return x * x + y * y + z * z;
	}

	/** @return This vector normalized to a unit vector. */
	Vector3<double> normalize() const {
		double len = length();
		if (len == 0) return Vector3<double>(0, 0, 1);
		return Vector3<double>(x / len, y / len, z / len);
	}

	/** @return This vector rounded down to an int vector. */
	Vector3<int> floor() const {
		return Vector3<int>((int)::floor(x), (int)::floor(y), (int)::floor(y));
	}


	/** @return Whether or not the coordinates of this Vector3 and other are within threshold of each other. */
	bool equals(Vector3 other, double threshold) const {
		return std::abs(x - other.x) < threshold &&
		       std::abs(y - other.y) < threshold &&
		       std::abs(z - other.z) < threshold;
	}


	/** Just adds the components. */
	Vector3<N>& operator+=(const Vector3<N>& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	/** Just subtracts the components. */
	Vector3<N>& operator-=(const Vector3<N>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	/** Just adds rhs to each component. */
	Vector3<N>& operator+=(N rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}
	/** Just subtracts rhs from each component. */
	Vector3<N>& operator-=(N rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}
	/** Just multiplies each component by rhs. */
	Vector3<N>& operator*=(N rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	/** Just divides each component by rhs. */
	Vector3<N>& operator/=(N rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	/** Just returns the vector in the opposite direction. */
	Vector3<N> operator-() {
		return Vector3<N>(-x, -y, -z);
	}

	/** x is at [0] and y is at [1] @{ */
	N& operator[](int idx) {
		if (!idx) return x;
		if (idx == 2) return z;
		return y;
	}
	N operator[](int idx) const {
		if (!idx) return x;
		if (idx == 2) return z;
		return y;
	}
	///@}


	N x; ///< X coordinate of the vector.
	N y; ///< Y coordinate of the vector.
	N z; ///< Z coordinate of the vector.
};

template<typename N>
inline bool operator==(const Vector3<N>& lhs, const Vector3<N>& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
template<typename N>
inline bool operator!=(const Vector3<N>& lhs, const Vector3<N>& rhs) {
	return !operator==(lhs, rhs);
}

template<typename N>
inline Vector3<N> operator+(Vector3<N> lhs, const Vector3<N>& rhs) {lhs += rhs; return lhs;}
template<typename N>
inline Vector3<N> operator-(Vector3<N> lhs, const Vector3<N>& rhs) {lhs -= rhs; return lhs;}
template<typename N>
inline Vector3<N> operator+(Vector3<N> lhs, double rhs) {lhs += rhs; return lhs;}
template<typename N>
inline Vector3<N> operator-(Vector3<N> lhs, double rhs) {lhs -= rhs; return lhs;}
template<typename N>
inline Vector3<N> operator*(Vector3<N> lhs, double rhs) {lhs *= rhs; return lhs;}
template<typename N>
inline Vector3<N> operator/(Vector3<N> lhs, double rhs) {lhs /= rhs; return lhs;}

template<typename N>
std::ostream& operator<<(std::ostream& os, const Vector3<N>& obj) {
	os << "(" << obj.x << ", " << obj.y << ")";
	return os;
}



// typedefs for common vectors
typedef Vector <double> Vec;
typedef Vector <float>  Vecf;
typedef Vector3<double> Vec3;
typedef Vector3<float>  Vec3f;
typedef Vector <int>    Coord;
typedef Vector3<int>    Coord3;

#endif // VEC_H


