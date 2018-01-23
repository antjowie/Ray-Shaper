#pragma once
#include <cmath>
#include <SFML\System\Vector2.hpp>

namespace Math
{
	// Point is a point in global coordinates
	// The point is rotated clock-wise relative to the origin
	inline sf::Vector2f rotateAroundOrigin(const sf::Vector2f &origin, const sf::Vector2f &point, const float degrees)
	{
		float radians{ degrees / 180.f * 3.14f };

		float tempX{ point.x - origin.x };
		float tempY{ point.y - origin.y };

		//https://gamedev.stackexchange.com/questions/86755/how-to-calculate-corner-positions-marks-of-a-rotated-tilted-rectangle
		float rotatedX = tempX * std::cos(radians) - tempY * std::sin(radians);
		float rotatedY = tempX * std::sin(radians) + tempY * std::cos(radians);

		return { rotatedX + origin.x,rotatedY + origin.y };
	}

	// Gets the angle in degrees between two vectors
	// All points are in global coordinates
	inline double getAngle(const sf::Vector2f &origin, const sf::Vector2f &a, const sf::Vector2f &b)
	{
		const sf::Vector2f A{ a - origin };
		const sf::Vector2f B{ b - origin };
		const double magA{ std::sqrt(std::powf(A.x,2) + std::powf(A.y,2)) };
		const double magB{ std::sqrt(std::powf(B.x,2) + std::powf(B.y,2)) };

		double dotProduct{ A.x * B.x + A.y * B.y };
		dotProduct /= (magA*magB);
		double angle{ acos(dotProduct) * 180. / 3.14 };

		if (A.x < 0)
			angle = 360 - angle;
		return angle;
	}

	inline void normalizeVector(sf::Vector2f &vector)
	{
		const float magnitude{ std::sqrtf(std::powf(vector.x,2) + std::powf(vector.y,2)) };
		vector /= magnitude;
	}

	inline float magnitude(const sf::Vector2f &vector)
	{
		return std::sqrtf(std::powf(vector.x, 2) + std::powf(vector.y, 2));
	}

	inline float dotProduct(const sf::Vector2f &a, const sf::Vector2f &b)
	{
		return a.x *b.x + a.y * b.y;
	}

	// Local coordinates
	// b must be unit vector
	inline sf::Vector2f projectionUnit(const sf::Vector2f &a, const sf::Vector2f &b)
	{
		float dot{ dotProduct(a,b) };
		
		// Prevents blunt triangles
		if (dot < 0)
			dot = 0;
		return b * dot;
	}
}