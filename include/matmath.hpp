#pragma once
#include <immintrin.h>

namespace winTerm {

	struct alignas(16) vec4 {
	private:
		struct positions { float x, y, z, w; };
		struct colours { float r, g, b, a; };
		static constexpr float EPSILON = 1e-6f;
	public:
		union {
			__m128 simd;
			positions p;
			colours c;
			float data[4];
		};
		
		vec4() : simd(_mm_setzero_ps()) {}
		vec4(float x, float y, float z, float w = 1.0f) : simd(_mm_set_ps(w, z, y, x)) {}
		vec4(__m128 v) : simd(v) {}

		// ADDITION    
		vec4& add(const vec4& other);
		void add(const vec4& other, vec4& result) const;
		void add_scaled(const vec4& other, float scalar);

		// SUBTRACTION
		vec4& sub(const vec4& other);
		void sub(const vec4& other, vec4& result) const;
		
		// MULTIPLCATION
		vec4& mul(float scalar);
		void mul(float scalar, vec4& result) const;
		
		// DIVISION
		vec4& div(float scalar);
		void div(float scalar, vec4& result) const;
		
		float dot(const vec4& other) const;
		float dot3(const vec4& other) const;
		
		void cross3(const vec4& other, vec4& result) const;
		
		float length() const;
		float length3() const;
		
		float length_squared() const;
		float length3_squared() const;
		
		vec4& normalize();
		vec4& normalize3();
		void normalize(vec4& result) const;
		void normalize3(vec4& result) const;
		
		float& operator[](int i) { return data[i]; }
		const float& operator[](int i) const { return data[i]; }
	};

	struct alignas(64) mat4 {
		union {
			__m128 rows[4];
			float m[16];
			float data[4][4];
		};
		
		mat4() {
			// Identity matrix
			rows[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
			rows[1] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
			rows[2] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
			rows[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
		}
		
		mat4(const float* data) {
			rows[0] = _mm_loadu_ps(&data[0]);
			rows[1] = _mm_loadu_ps(&data[1]);
			rows[2] = _mm_loadu_ps(&data[2]);
			rows[3] = _mm_loadu_ps(&data[3]);
		}
	   
		// ADDITION
		mat4& add(const mat4& other);
		void add(const mat4& other, mat4& result) const;
		
		// SUBTRACTION
		mat4& sub(const mat4& other);
		void sub(const mat4& other, mat4& result) const;
		
		// MULTIPLCATION
		mat4& mul(float scalar);
		void mul(float scalar, mat4& result) const;
		void mul(const mat4& other, mat4& result) const;
		void mul(const vec4& vec, vec4& result) const;
		
		// TRANSPOSE
		void transpose(mat4& result) const;
		mat4& transpose();
		
		float* operator[](int i) { return data[i]; }
		const float* operator[](int i) const { return data[i]; }
	};

	void lerp(const vec4& v1, const vec4& v2, vec4& result, const float t) noexcept;

	void weighted_sum(const vec4& a, const vec4& b, const vec4& c, const vec4& weights, vec4& result) noexcept;

	void make_translation(float x, float y, float z, mat4& result) noexcept;

	void make_translation(const vec4& t, mat4& result) noexcept;

	void make_scale(float x, float y, float z, mat4& result) noexcept;

	void make_rotation_x(float angle, mat4& result) noexcept;

	void make_rotation_y(float angle, mat4& result) noexcept;

	void make_rotation_z(float angle, mat4& result) noexcept;

	void make_rotation_axis(const vec4& axis, float angle, mat4& result) noexcept;

	void make_look_at(const vec4& eye, const vec4& center, const vec4& up, mat4& result) noexcept;

	void make_perspective(float fov, float aspect, float near_plane, float far_plane, mat4& result) noexcept;

	void make_orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane, mat4& result) noexcept;
}
