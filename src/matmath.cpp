#include "matmath.hpp"
#include <cmath>

namespace winTerm {

	vec4& vec4::add(const vec4& other) {
		simd = _mm_add_ps(simd, other.simd);
		return *this;
	}

	void vec4::add(const vec4& other, vec4& result) const {
		result.simd = _mm_add_ps(simd, other.simd);
	}

	void vec4::add_scaled(const vec4& other, float scalar) {
		simd = _mm_add_ps(simd, _mm_mul_ps(other.simd, _mm_set1_ps(scalar)));
	}

	vec4& vec4::sub(const vec4& other) {
		simd = _mm_sub_ps(simd, other.simd);
		return *this;
	}

	void vec4::sub(const vec4& other, vec4& result) const {
		result.simd = _mm_sub_ps(simd, other.simd);
	}

	vec4& vec4::mul(float scalar) {
		simd = _mm_mul_ps(simd, _mm_set1_ps(scalar));
		return *this;
	}

	void vec4::mul(float scalar, vec4& result) const {
		result.simd = _mm_mul_ps(simd, _mm_set1_ps(scalar));
	}

	vec4& vec4::div(float scalar) {
		simd = _mm_div_ps(simd, _mm_set1_ps(scalar));
		return *this;
	}

	void vec4::div(float scalar, vec4& result) const {
		result.simd = _mm_div_ps(simd, _mm_set1_ps(scalar));
	}

	float vec4::dot(const vec4& other) const {
		__m128 mult = _mm_mul_ps(simd, other.simd);
		__m128 sum = _mm_hadd_ps(mult, mult);
		sum = _mm_hadd_ps(sum, sum);
		return _mm_cvtss_f32(sum);
	}

	float vec4::dot3(const vec4& other) const {
		__m128 mult = _mm_mul_ps(simd, other.simd);
		// Zero out w component
		__m128 mask = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
		mult = _mm_mul_ps(mult, mask);
		__m128 sum = _mm_hadd_ps(mult, mult);
		sum = _mm_hadd_ps(sum, sum);
		return _mm_cvtss_f32(sum);
	}

	void vec4::cross3(const vec4& other, vec4& result) const {
		__m128 a_yzx = _mm_shuffle_ps(simd, simd, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 b_yzx = _mm_shuffle_ps(other.simd, other.simd, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 a_zxy = _mm_shuffle_ps(simd, simd, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 b_zxy = _mm_shuffle_ps(other.simd, other.simd, _MM_SHUFFLE(3, 1, 0, 2));
		
		result.simd = _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx));
		result.p.w = 0.0f;
	}

	float vec4::length() const {
		return std::sqrt(dot(*this));
	}

	float vec4::length3() const {
		return std::sqrt(dot3(*this));
	}

	float vec4::length_squared() const {
		return dot(*this);
	}

	float vec4::vec4::length3_squared() const {
		return dot3(*this);
	}

	vec4& vec4::normalize() {
		float len = length();
		if (len > EPSILON) {
			mul(1.0f / len);
		}
		return *this;
	}

	vec4& vec4::normalize3() {
		float len = length3();
		if (len > EPSILON) {
			simd = _mm_mul_ps(simd, _mm_set_ps(1.0f, 1.0f / len, 1.0f / len, 1.0f / len));
		}
		return *this;
	}

	void vec4::normalize(vec4& result) const {
		float len = length();
		if (len > EPSILON) {
			mul(1.0f / len, result);
		} else {
			result = *this;
		}
	}

	void vec4::normalize3(vec4& result) const {
		float len = length3();
		if (len > EPSILON) {
			result.simd = _mm_mul_ps(simd, _mm_set_ps(p.w, 1.0f / len, 1.0f / len, 1.0f / len));
		} else {
			result = *this;
		}
	}


	mat4& mat4::add(const mat4& other) {
		rows[0] = _mm_add_ps(rows[0], other.rows[0]);
		rows[1] = _mm_add_ps(rows[1], other.rows[1]);
		rows[2] = _mm_add_ps(rows[2], other.rows[2]);
		rows[3] = _mm_add_ps(rows[3], other.rows[3]);
		return *this;
	}

	void mat4::add(const mat4& other, mat4& result) const {
		result.rows[0] = _mm_add_ps(rows[0], other.rows[0]);
		result.rows[1] = _mm_add_ps(rows[1], other.rows[1]);
		result.rows[2] = _mm_add_ps(rows[2], other.rows[2]);
		result.rows[3] = _mm_add_ps(rows[3], other.rows[3]);
	}

	mat4& mat4::sub(const mat4& other) {
		rows[0] = _mm_sub_ps(rows[0], other.rows[0]);
		rows[1] = _mm_sub_ps(rows[1], other.rows[1]);
		rows[2] = _mm_sub_ps(rows[2], other.rows[2]);
		rows[3] = _mm_sub_ps(rows[3], other.rows[3]);
		return *this;
	}

	void mat4::sub(const mat4& other, mat4& result) const {
		result.rows[0] = _mm_sub_ps(rows[0], other.rows[0]);
		result.rows[1] = _mm_sub_ps(rows[1], other.rows[1]);
		result.rows[2] = _mm_sub_ps(rows[2], other.rows[2]);
		result.rows[3] = _mm_sub_ps(rows[3], other.rows[3]);
	}

	mat4& mat4::mul(float scalar) {
		__m128 s = _mm_set1_ps(scalar);
		rows[0] = _mm_mul_ps(rows[0], s);
		rows[1] = _mm_mul_ps(rows[1], s);
		rows[2] = _mm_mul_ps(rows[2], s);
		rows[3] = _mm_mul_ps(rows[3], s);
		return *this;
	}

	void mat4::mul(float scalar, mat4& result) const {
		__m128 s = _mm_set1_ps(scalar);
		result.rows[0] = _mm_mul_ps(rows[0], s);
		result.rows[1] = _mm_mul_ps(rows[1], s);
		result.rows[2] = _mm_mul_ps(rows[2], s);
		result.rows[3] = _mm_mul_ps(rows[3], s);
	}

	void mat4::mul(const mat4& other, mat4& result) const {
		__m128 col0 = _mm_set_ps(other.data[3][0], other.data[2][0], other.data[1][0], other.data[0][0]);
		__m128 col1 = _mm_set_ps(other.data[3][1], other.data[2][1], other.data[1][1], other.data[0][1]);
		__m128 col2 = _mm_set_ps(other.data[3][2], other.data[2][2], other.data[1][2], other.data[0][2]);
		__m128 col3 = _mm_set_ps(other.data[3][3], other.data[2][3], other.data[1][3], other.data[0][3]);
		
		for (int i = 0; i < 4; i++) {
			__m128 row = rows[i];
			
			__m128 x = _mm_shuffle_ps(row, row, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 y = _mm_shuffle_ps(row, row, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 z = _mm_shuffle_ps(row, row, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 w = _mm_shuffle_ps(row, row, _MM_SHUFFLE(3, 3, 3, 3));
			
			result.rows[i] = _mm_add_ps(
				_mm_add_ps(_mm_mul_ps(x, col0), _mm_mul_ps(y, col1)),
				_mm_add_ps(_mm_mul_ps(z, col2), _mm_mul_ps(w, col3))
			);
		}
	}

	void mat4::mul(const vec4& vec, vec4& result) const {
		__m128 x = _mm_shuffle_ps(vec.simd, vec.simd, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 y = _mm_shuffle_ps(vec.simd, vec.simd, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 z = _mm_shuffle_ps(vec.simd, vec.simd, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 w = _mm_shuffle_ps(vec.simd, vec.simd, _MM_SHUFFLE(3, 3, 3, 3));
		
		result.simd = _mm_add_ps(
			_mm_add_ps(_mm_mul_ps(x, rows[0]), _mm_mul_ps(y, rows[1])),
			_mm_add_ps(_mm_mul_ps(z, rows[2]), _mm_mul_ps(w, rows[3]))
		);
	}

	void mat4::transpose(mat4& result) const {
		__m128 tmp0 = _mm_unpacklo_ps(rows[0], rows[1]);
		__m128 tmp1 = _mm_unpacklo_ps(rows[2], rows[3]);
		__m128 tmp2 = _mm_unpackhi_ps(rows[0], rows[1]);
		__m128 tmp3 = _mm_unpackhi_ps(rows[2], rows[3]);
		
		result.rows[0] = _mm_movelh_ps(tmp0, tmp1);
		result.rows[1] = _mm_movehl_ps(tmp1, tmp0);
		result.rows[2] = _mm_movelh_ps(tmp2, tmp3);
		result.rows[3] = _mm_movehl_ps(tmp3, tmp2);
	}

	mat4& mat4::transpose() {
		mat4 temp;
		transpose(temp);
		*this = temp;
		return *this;
	}

	void lerp(const vec4& v1, const vec4& v2, vec4& result, const float t) noexcept {
		v2.sub(v1, result);
		result.mul(t, result);
		v1.add(result, result);
	}

	void weighted_sum(const vec4& a, const vec4& b, const vec4& c, const vec4& weights, vec4& result) noexcept {
		a.mul(weights[0], result);
		result.add_scaled(b, weights[1]);
		result.add_scaled(c, weights[2]);
	}

	void make_translation(float x, float y, float z, mat4& result) noexcept {
		result = mat4(); // Identity
		result[0][3] = x;
		result[1][3] = y;
		result[2][3] = z;
	}

	void make_translation(const vec4& t, mat4& result) noexcept {
		make_translation(t.p.x, t.p.y, t.p.z, result);
	}

	// Scale matrix
	void make_scale(float x, float y, float z, mat4& result) noexcept {
		result = mat4(); // Identity
		result[0][0] = x;
		result[1][1] = y;
		result[2][2] = z;
	}

	// Rotation matrices
	void make_rotation_x(float angle, mat4& result) noexcept {
		result = mat4(); // Identity
		float c = std::cos(angle);
		float s = std::sin(angle);
		result[1][1] = c;  result[1][2] = -s;
		result[2][1] = s;  result[2][2] = c;
	}

	void make_rotation_y(float angle, mat4& result) noexcept {
		result = mat4(); // Identity
		float c = std::cos(angle);
		float s = std::sin(angle);
		result[0][0] = c;   result[0][2] = s;
		result[2][0] = -s;  result[2][2] = c;
	}

	void make_rotation_z(float angle, mat4& result) noexcept {
		result = mat4(); // Identity
		float c = std::cos(angle);
		float s = std::sin(angle);
		result[0][0] = c;  result[0][1] = -s;
		result[1][0] = s;  result[1][1] = c;
	}

	// Rotate around arbitrary axis
	void make_rotation_axis(const vec4& axis, float angle, mat4& result) noexcept {
		vec4 a;
		axis.normalize3(a);
		
		float c = std::cos(angle);
		float s = std::sin(angle);
		float t = 1.0f - c;
		
		result = mat4(); // Identity
		result[0][0] = c + a.p.x * a.p.x * t;
		result[0][1] = a.p.x * a.p.y * t - a.p.z * s;
		result[0][2] = a.p.x * a.p.z * t + a.p.y * s;
		
		result[1][0] = a.p.y * a.p.x * t + a.p.z * s;
		result[1][1] = c + a.p.y * a.p.y * t;
		result[1][2] = a.p.y * a.p.z * t - a.p.x * s;
		
		result[2][0] = a.p.z * a.p.x * t - a.p.y * s;
		result[2][1] = a.p.z * a.p.y * t + a.p.x * s;
		result[2][2] = c + a.p.z * a.p.z * t;
	}

	// Look-at matrix
	void make_look_at(const vec4& eye, const vec4& center, const vec4& up, mat4& result) noexcept {
		vec4 f, u, s;
		vec4(center.p.x - eye.p.x, center.p.y - eye.p.y, center.p.z - eye.p.z, 0).normalize3(f);
		up.normalize3(u);
		f.cross3(u, s);
		s.normalize3();
		s.cross3(f, u);
		
		result = mat4(); // Identity
		result[0][0] = s.p.x;   result[0][1] = s.p.y;   result[0][2] = s.p.z;   result[0][3] = -s.dot3(eye);
		result[1][0] = u.p.x;   result[1][1] = u.p.y;   result[1][2] = u.p.z;   result[1][3] = -u.dot3(eye);
		result[2][0] = -f.p.x;  result[2][1] = -f.p.y;  result[2][2] = -f.p.z;  result[2][3] = f.dot3(eye);
		result[3][0] = 0;     result[3][1] = 0;     result[3][2] = 0;     result[3][3] = 1;
	}

	// Perspective projection
	void make_perspective(float fov, float aspect, float near_plane, float far_plane, mat4& result) noexcept {
		float tan_half_fov = std::tan(fov * 0.5f);
		
		result = mat4();
		result[0][0] = 1.0f / (aspect * tan_half_fov);
		result[1][1] = 1.0f / tan_half_fov;
		result[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
		result[2][3] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
		result[3][2] = -1.0f;
		result[3][3] = 0.0f;
	}

	// Orthographic projection
	void make_orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane, mat4& result) noexcept {
		result = mat4(); // Identity
		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = -2.0f / (far_plane - near_plane);
		result[0][3] = -(right + left) / (right - left);
		result[1][3] = -(top + bottom) / (top - bottom);
		result[2][3] = -(far_plane + near_plane) / (far_plane - near_plane);
	}
}
