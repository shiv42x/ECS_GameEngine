#pragma once
#include <iostream>
class Vec2
{
public:

	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xin, float yin);

	friend std::ostream& operator << (std::ostream& stream, const Vec2& vec);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator /= (const float val);
	void operator *= (const float val);

	double dist(const Vec2& rhs) const;
	double length() const;
	void   normalize();
	static Vec2 normalize(Vec2 vec);
};