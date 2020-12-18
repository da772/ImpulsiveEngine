namespace IEVector {

template <typename T = float>
static T sqrt(T x) {
    T cur{x}, prev{0};
    while(cur != prev)
    {
        prev = cur;
        cur = 0.5f*(cur + x/cur);
    }
    return cur;
}

template <typename T = float>
static T abs(T x) {
    return x*(x > 0 ? 1 : -1);
}

template <typename T = float>
static T lerp(T a, T b, T amt) {
    return a + amt * (b-a);
}

template <typename T>
class Vector2;
template <typename T>
class Vector3;
template <typename T>
class Vector4;

template <typename T>
class Vector2 {
    public:
        inline Vector2() : x(0.f), y(0.f) {}
        inline Vector2(const T& v) : x(v), y(v) {};
        inline Vector2(const T& x, const T& y) : x(x), y(y) {};
        inline Vector2(const Vector2<T>& other) : x(other.x),y(other.y) {}
        inline Vector2(const Vector3<T>& other) : x(other.x), y(other.y) {}
        inline Vector2(const Vector4<T>& other) : x(other.x), y(other.y) {}
        inline Vector2(const T* ptr) : x(ptr[0]), y(ptr[1]) {};
     
        T x;
        T y;
        inline Vector2<T> operator +(const Vector2<T>& other) const {
            return Vector2<T>(x+other.x, y+other.y);
        }
        inline Vector2<T> operator +(const T& other) const {
            return Vector2<T>(x+other, y+other);
        }
        inline Vector2<T> operator -(const Vector2<T>& other) const {
            return Vector2<T>(x-other.x, y-other.y);
        }
        inline Vector2<T> operator -(const T& other) const {
            return Vector2<T>(x-other, y-other);
        }
        inline Vector2<T> operator -() const {
            return Vector2<T>(-x, -y);
        }
        inline Vector2<T> operator *(const Vector2<T>& other) const {
            return Vector2<T>(x*other.x, y*other.y);
        }
        inline Vector2<T> operator *(const T& other) const {
            return Vector2<T>(x*other, y*other);
        }
        inline Vector2<T> operator /(const Vector2<T>& other) const {
            return Vector2<T>(x/other.x, y/other.y);
        }
         inline Vector2<T> operator /(const T& other) const {
            return Vector2<T>(x/other, y/other);
        }
        inline bool operator ==(const Vector2<T>& other) const {
            return other.x == x && other.y == y;
        }
		inline bool operator !=(const Vector2<T>& other) const {
			return !(other == *this);
		}
        inline  Vector2<T>& operator +=(const T& v) {
            x += v;
            y += v;
            return (*this);
        }
        inline  Vector2<T>& operator +=(const Vector2<T>& other) {
            x += other.x;
            y += other.y;
            return (*this);
        }
         inline  Vector2<T>& operator -=(const T& v) {
            x -= v;
            y -= v;
            return (*this);
        }
        inline  Vector2<T>& operator -=(const Vector2<T>& other) {
            x -= other.x;
            y -= other.y;
            return (*this);
        }
        inline  Vector2<T>& operator *=(const Vector2<T>& other) {
            x *= other.x;
            y *= other.y;
            return (*this);
        }
        inline  Vector2<T>& operator *=(const T& v) {
            x *= v;
            y *= v;
            return (*this);
        }
        inline  Vector2<T>& operator /=(const T& v) {
            x /= v;
            y /= v;
            return (*this);
        }
        inline  Vector2<T>& operator /=(const Vector2<T>& other) {
            x /= other.x;
            y /= other.y;
            return (*this);
        }
        inline  Vector2<T>& operator ++() {
            x++;
            y++;
            return (*this);
        }
        inline  Vector2<T>& operator ++(int) {
            x++;
            y++;
            return (*this);
        }
        inline  Vector2<T>& operator --() {
            x--;
            y--;
            return (*this);
        }
        inline  Vector2<T>& operator --(int) {
            x--;
            y--;
            return (*this);
        }
        inline T length() const { return sqrt<T>(x*x+y*y); }
        inline T size() const { return length(); }
        inline T magnitude() const { return length(); }
        inline Vector2<T> normalize() const { return (*this)/length();}
        static inline T dot(const Vector2<T>& a, const Vector2<T>& b )  { return a.x*b.x+a.y*b.y; }
        static inline Vector2<T> project(const Vector2<T>& a, const Vector2<T>& b)  { return (b*(Vector2<T>::dot(a,b) / Vector2<T>::dot(b,b))); }
        static inline Vector2<T> reject(const Vector2<T>& a, const Vector2<T>& b)  {return (a-b*(Vector2<T>::dot(a,b)/Vector2<T>::dot(b,b)));}
        static inline Vector2<T> reflect(const Vector2<T>& v, const Vector2<T>& n)  { return v - ((Vector2<T>::dot(v, n)) + (Vector2<T>::dot(v, n))) * n;}
        static inline T distance(const Vector2<T>& a, const Vector2<T>& b)  { return sqrt<T>(( (a.x-b.x)*(a.x-b.x) ) + ((a.y-b.y)*(a.y-b.y))); };
        static inline Vector2<T> lerp(const Vector2<T>& a, const Vector2<T>& b, T amt)  { return Vector2<T>(IEVector::lerp<T>(a.x,b.x, amt), IEVector::lerp<T>(a.y,b.y,amt)); }
        inline const T* data() const { return &x;}
        //template < typename N > Vector2& operator=(const Vector2< N >& rhs) { return Vector2<N>((T)rhs.x, (T)rhs.y); }
};

template <typename T>
class Vector3 {
    public:
        inline Vector3() : x(0.f), y(0.f), z(0.f) {}
        inline Vector3(const T& v) : x(v), y(v), z(v) {};
        inline Vector3(const T& x, const T& y, const T& z) : x(x),y(y),z(z) {}
        inline Vector3(const Vector3<T>& other) : x(other.x),y(other.y),z(other.z) {}
        inline Vector3(const Vector4<T>& other) : x(other.x), y(other.y), z(other.z) {}
        inline Vector3(const T* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]) {};

        T x;
        T y;
        T z;

        inline Vector3<T> operator +(const Vector3<T>& other) const {
            return Vector3<T>(x+other.x, y+other.y, z+other.z);
        }
        inline Vector3<T> operator +(const Vector2<T>& other) const {
            return Vector3<T>(x+other.x, y+other.y, z);
        }
        inline Vector3<T> operator +(const T& other) const {
            return Vector3<T>(x+other, y+other, z+other);
        }
        inline Vector3<T> operator -(const Vector3<T>& other) const {
            return Vector3<T>(x-other.x, y-other.y, z-other.z);
        }
        inline Vector3<T> operator -(const Vector2<T>& other) const {
            return Vector3<T>(x-other.x, y-other.y, z);
        }
        inline Vector3<T> operator -(const T& other) const {
            return Vector3<T>(x-other, y-other, z-other);
        }
        inline Vector3<T> operator -() const {
            return Vector3<T>(-x, -y, -z);
        }
        inline Vector3<T> operator *(const Vector3<T>& other) const {
            return Vector3<T>(x*other.x, y*other.y, z*other.z);
        }
        inline Vector3<T> operator *(const Vector2<T>& other) const {
            return Vector3<T>(x*other.x, y*other.y, z);
        }
        inline Vector3<T> operator *(const T& other) const {
            return Vector3<T>(x*other, y*other, z*other);
        }
        inline Vector3<T> operator /(const Vector3<T>& other) const {
            return Vector3<T>(x/other.x, y/other.y, z/other.z);
        }
        inline Vector3<T> operator /(const Vector2<T>& other) const {
            return Vector3<T>(x/other.x, y/other.y, z);
        }
         inline Vector3<T> operator /(const T& other) const {
            return Vector3<T>(x/other, y/other, z/other);
        }
        inline bool operator ==(const Vector3<T>& other) const {
            return other.x == x && other.y == y && other.z == z;
        }
		inline bool operator !=(const Vector3<T>& other) const {
			return !(other == *this);
		}
        inline  Vector3<T>& operator +=(const T& v) {
            x += v;
            y += v;
            z += v;
            return (*this);
        }
        inline  Vector3<T>& operator +=(const Vector3<T>& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return (*this);
        }
        inline  Vector3<T>& operator +=(const Vector2<T>& other) {
            x += other.x;
            y += other.y;
            return (*this);
        }
        inline  Vector3<T>& operator -=(const T& v) {
            x -= v;
            y -= v;
            z -= v;
            return (*this);
        }
        inline  Vector3<T>& operator -=(const Vector3<T>& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return (*this);
        }
        inline  Vector3<T>& operator -=(const Vector2<T>& other) {
            x -= other.x;
            y -= other.y;
            return (*this);
        }
        inline  Vector3<T>& operator *=(const Vector3<T>& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return (*this);
        }
        inline  Vector3<T>& operator *=(const Vector2<T>& other) {
            x *= other.x;
            y *= other.y;
            return (*this);
        }
        inline Vector3<T>& operator *=(const T& v) {
            x *= v;
            y *= v;
            z *= v;
            return (*this);
        }
        inline  Vector3<T>& operator /=(const T& v) {
            x /= v;
            y /= v;
            z /= v;
            return (*this);
        }
        inline  Vector3<T>& operator /=(const Vector3<T>& other) {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return (*this);
        }
        inline Vector3<T>& operator /=(const Vector2<T>& other) {
            x /= other.x;
            y /= other.y;
            return (*this);
        }
        inline  Vector3<T>& operator ++() {
            x++;
            y++;
            z++;
            return (*this);
        }
        inline  Vector3<T>& operator ++(int) {
            x++;
            y++;
            z++;
            return (*this);
        }
        inline  Vector3<T>& operator --() {
            x--;
            y--;
            z--;
            return (*this);
        }
        inline  Vector3<T>& operator --(int) {
            x--;
            y--;
            z--;
            return (*this);
        }
        inline T length() const { return sqrt<T>(x*x+y*y+z*z); }
        inline T size() const { return length(); }
        inline T magnitude() const { return length(); }
        inline Vector3<T> normalize() const { return (*this)/length();}
        static inline T dot(const Vector3<T>& a, const Vector3<T>& b )  { return a.x*b.x+a.y*b.y+a.z*b.z; }
        static inline Vector3<T> cross(const Vector3<T> a, const Vector3<T> b)  { return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};};
        static inline Vector3<T> project(const Vector3<T>& a, const Vector3<T>& b)  { return (b*(Vector3<T>::dot(a,b) / Vector3<T>::dot(b,b))); }
        static inline Vector3<T> reject(const Vector3<T>& a, const Vector3<T>& b)  {return (a-b*(Vector3<T>::dot(a,b)/Vector3<T>::dot(b,b)));}
        static inline Vector3<T> reflect(const Vector3<T>& v, const Vector3<T>& n)  { return v -  ((Vector3<T>::dot(v, n)) + (Vector3<T>::dot(v, n))) * n;}
        static inline T distance(const Vector3<T>& a, const Vector3<T>& b)  { return sqrt<T>(( (a.x-b.x)*(a.x-b.x) ) + ((a.y-b.y)*(a.y-b.y)) + ((a.z-b.z)*(a.z-b.z))); };
        static inline Vector3<T> lerp(const Vector3<T>& a, const Vector3<T>& b, T amt)  { return Vector3<T>(IEVector::lerp<T>(a.x,b.x, amt), IEVector::lerp<T>(a.y,b.y,amt), IEVector::lerp<T>(a.z,b.z,amt)); }
        inline Vector2<T> xy() const { return {x,y};}
        inline const T* data() const { return &x;}
        //template < typename N > Vector3& operator=(const Vector3< N >& rhs) { return Vector3<N>((T)rhs.x, (T)rhs.y, (T)rhs.z); }
};

template <typename T>
class Vector4 {
    public:
        inline Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
        inline Vector4(const T& v) : x(v), y(v), z(v), w(v) {};
        inline Vector4(const T& x, const T& y, const T& z, const T& w) : x(x),y(y),z(z),w(w) {}
        inline Vector4(const Vector4<T>& other) : x(other.x),y(other.y),z(other.z),w(other.w) {}
        inline Vector4(const T* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {};
        
        T x;
        T y;
        T z;
        T w;

        inline Vector4<T> operator +(const Vector4<T>& other) const {
            return Vector4<T>(x+other.x, y+other.y, z+other.z, w+other.w);
        }
        inline Vector4<T> operator +(const Vector2<T>& other) const {
            return Vector4<T>(x+other.x, y+other.y, z, w);
        }
        inline Vector4<T> operator +(const Vector3<T>& other) const {
            return Vector4<T>(x+other.x, y+other.y, other.z+z, w);
        }
        inline Vector4<T> operator +(const T& other) const {
            return Vector4<T>(x+other, y+other, z+other, w+other);
        }
        inline Vector4<T> operator -(const Vector4<T>& other) const {
            return Vector4<T>(x-other.x, y-other.y, z-other.z, w-other.w);
        }
        inline Vector4<T> operator -(const Vector2<T>& other) const {
            return Vector4<T>(x-other.x, y-other.y, z, w);
        }
        inline Vector4<T> operator -(const Vector3<T>& other) const {
            return Vector4<T>(x-other.x, y-other.y, z-other.z, w);
        }
        inline Vector4<T> operator -(const T& other) const {
            return Vector4<T>(x-other, y-other, z-other, w-other);
        }
        inline Vector4<T> operator -() const {
            return Vector4<T>(-x, -y, -z, -w);
        }
        inline Vector4<T> operator *(const Vector4<T>& other) const {
            return Vector4<T>(x*other.x, y*other.y, z*other.z, w*other.w);
        }
        inline Vector4<T> operator *(const Vector2<T>& other) const {
            return Vector4<T>(x*other.x, y*other.y, z, w);
        }
        inline Vector4<T> operator *(const Vector3<T>& other) const {
            return Vector4<T>(x*other.x, y*other.y, z*other.z, w);
        }
        inline Vector4<T> operator *(const T& other) const {
            return Vector4<T>(x*other, y*other, z*other, w*other);
        }
        inline Vector4<T> operator /(const Vector4<T>& other) const {
            return Vector4<T>(x/other.x, y/other.y, z/other.z, w/other.w);
        }
        inline Vector4<T> operator /(const Vector2<T>& other) const {
            return Vector4<T>(x/other.x, y/other.y, z, w);
        }
        inline Vector4<T> operator /(const Vector3<T>& other) const {
            return Vector4<T>(x/other.x, y/other.y, z/other.z, w);
        }
        inline Vector4<T> operator /(const T& other) const {
            return Vector4<T>(x/other, y/other, z/other, w/other);
        }
        inline bool operator ==(const Vector4<T>& other) const {
            return other.x == x && other.y == y && other.z == z && other.w == w;
        }
		inline bool operator !=(const Vector4<T>& other) const {
            return !(other == *this);
		}
        inline Vector4<T>& operator +=(const T& v) {
            x += v;
            y += v;
            z += v;
            w += v;
            return (*this);
        }
        inline Vector4<T>& operator +=(const Vector4<T>& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return (*this);
        }
        inline Vector4<T>& operator +=(const Vector2<T>& other) {
            x += other.x;
            y += other.y;
            return (*this);
        }
        inline Vector4<T>& operator +=(const Vector3<T>& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return (*this);
        }
        inline Vector4<T>& operator -=(const T& v) {
            x -= v;
            y -= v;
            z -= v;
            w -= v;
            return (*this);
        }
        inline Vector4<T>& operator -=(const Vector4<T>& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return (*this);
        }
        inline Vector4<T>& operator -=(const Vector2<T>& other) {
            x -= other.x;
            y -= other.y;
            return (*this);
        }
        inline Vector4<T>& operator -=(const Vector3<T>& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return (*this);
        }
        inline Vector4<T>& operator *=(const Vector4<T>& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return (*this);
        }
        inline Vector4<T>& operator *=(const Vector2<T>& other) {
            x *= other.x;
            y *= other.y;
            return (*this);
        }
        inline Vector4<T>& operator *=(const Vector3<T>& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return (*this);
        }
        inline Vector4<T>& operator *=(const T& v) {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
            return (*this);
        }
        inline  Vector4<T>& operator /=(const T& v) {
            x /= v;
            y /= v;
            z /= v;
            w /= v;
            return (*this);
        }
        inline  Vector4<T>& operator /=(const Vector4<T>& other) {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w / other.w;
            return (*this);
        }
        inline Vector4<T>& operator /=(const Vector2<T>& other) {
            x /= other.x;
            y /= other.y;
            return (*this);
        }
        inline Vector4<T>& operator /=(const Vector3<T>& other) {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return (*this);
        }
        inline  Vector4<T>& operator ++() {
            x++;
            y++;
            z++;
            w++;
            return (*this);
        }
        inline  Vector4<T>& operator ++(int) {
            x++;
            y++;
            z++;
            w++;
            return (*this);
        }
        inline  Vector4<T>& operator --() {
            x--;
            y--;
            z--;
            w--;
            return (*this);
        }
        inline  Vector4<T>& operator --(int) {
            x--;
            y--;
            z--;
            w--;
            return (*this);
        }
        inline T length() const { return sqrt<T>(x*x+y*y+z*z+w*w); }
        inline T size() const { return length(); }
        inline T magnitude() const { return length(); }
        inline Vector4<T> normalize() const { return (*this)/length();}
        static inline T dot(const Vector4<T>& a, const Vector4<T>& b ) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }
        static inline Vector4<T> reflect(const Vector4<T>& v, const Vector4<T>& n) { return v - ((Vector4<T>::dot(v, n)) + (Vector4<T>::dot(v, n))) * n;}
        static inline T distance(const Vector4<T>& a, const Vector4<T>& b) { return sqrt<T>(( (a.x-b.x)*(a.x-b.x) ) + ((a.y-b.y)*(a.y-b.y)) + ((a.z-b.z)*(a.z-b.z)) + ((a.w-b.w)*(a.w-b.w))); };
        static inline Vector4<T> lerp(const Vector4<T>& a, const Vector4<T>& b, T amt) { return Vector4<T>(IEVector::lerp<T>(a.x,b.x, amt), IEVector::lerp<T>(a.y,b.y,amt), IEVector::lerp<T>(a.z,b.z,amt), IEVector::lerp<T>(a.w, b.w, amt)); }
        static inline Vector4<T> project(const Vector4<T>& a, const Vector4<T>& b)  { return (b*(Vector4<T>::dot(a,b) / Vector4<T>::dot(b,b))); }
        static inline Vector4<T> reject(const Vector4<T>& a, const Vector4<T>& b)  {return (a-b*(Vector4<T>::dot(a,b)/Vector4<T>::dot(b,b)));}
        inline Vector2<T> xy() const { return {x,y};}
        inline Vector3<T> xyz() const { return {x,y,z};}
        inline const T* data() const { return &x;}
		//template < typename N > Vector4& operator=(const Vector4< N >& rhs) { return Vector4<N>((T)rhs.x, (T) rhs.y, (T) rhs.z, (T) rhs.w); }
};

template<typename T = float>
inline Vector4<T> operator *(const Vector4<T>& lhs, const T& rhs )  {
            return Vector4<T>(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs, lhs.w*rhs);
}
template<typename T = float>
inline Vector4<T> operator *(const T& lhs,const Vector4<T>& rhs)  {
            return Vector4<T>(rhs.x*lhs, rhs.y*lhs, rhs.z*lhs, rhs.w*lhs);
}
template<typename T = float>
inline Vector3<T> operator *(const Vector3<T>& lhs, const T& rhs )  {
            return Vector3<T>(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
}
template<typename T = float>
inline Vector3<T> operator *(const T& lhs,const Vector3<T>& rhs)  {
            return Vector3<T>(rhs.x*lhs, rhs.y*lhs, rhs.z*lhs);
}

template<typename T = float>
inline Vector2<T> operator *(const Vector2<T>& lhs, const T& rhs )  {
            return Vector2<T>(lhs.x*rhs, lhs.y*rhs);
}
template<typename T = float>
inline Vector2<T> operator *(const T& lhs,const Vector2<T>& rhs)  {
            return Vector2<T>(rhs.x*lhs, rhs.y*lhs);
}

template<typename T = float>
inline Vector4<T> operator +(const Vector4<T>& lhs, const T& rhs )  {
            return Vector4<T>(lhs.x+rhs, lhs.y+rhs, lhs.z+rhs, lhs.w+rhs);
}
template<typename T = float>
inline Vector4<T> operator +(const T& lhs,const Vector4<T>& rhs)  {
            return Vector4<T>(rhs.x+lhs, rhs.y+lhs, rhs.z+lhs, rhs.w+lhs);
}
template<typename T = float>
inline Vector3<T> operator +(const Vector3<T>& lhs, const T& rhs )  {
            return Vector3<T>(lhs.x+rhs, lhs.y+rhs, lhs.z+rhs);
}
template<typename T = float>
inline Vector3<T> operator +(const T& lhs,const Vector3<T>& rhs)  {
            return Vector3<T>(rhs.x+lhs, rhs.y+lhs, rhs.z+lhs);
}

template<typename T = float>
inline Vector2<T> operator +(const Vector2<T>& lhs, const T& rhs )  {
            return Vector2<T>(lhs.x+rhs, lhs.y+rhs);
}
template<typename T = float>
inline Vector2<T> operator +(const T& lhs,const Vector2<T>& rhs)  {
            return Vector2<T>(rhs.x+lhs, rhs.y+lhs);
}


typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;


}