#pragma once

#include<vector>

struct Point
{
	float x = 0.0f, y = 0.0f;
	Point(float _x, float _y) : x(_x), y(_y) {}
};

struct Quad
{
	float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
	Quad(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {}

	bool Contains(Point point)
	{
		float hw = w * 0.5f;
		float hh = h * 0.5f;

		return (
			point.x <= x + hw &&
			point.x >= x - hw &&
			point.y >= y - hh &&
			point.y <= y + hh );
	}

	bool Intersects(Quad quad)
	{
		float hw = w * 0.5f;
		float hh = h * 0.5f;
		return !(
			quad.x - (quad.w * 0.5f) > x + hw ||
			quad.x + (quad.w * 0.5f) < x - hw ||
			quad.y + (quad.h * 0.5f) < y - hh ||
			quad.y - (quad.h * 0.5f) > y + hh );
	}
};

class QuadTree
{
public:
	QuadTree(Quad baseQuad, uint32_t capacity) : m_baseQuad(baseQuad), m_capacity(capacity) {}

	bool Insert(Point point)
	{
		if (!m_baseQuad.Contains(point))
			return false;

		if (!m_divided)
		{
			if (m_points.size() < m_capacity)
			{
				m_points.push_back(point);
				return true;
			}

			Subdivide();
		}

		return (m_ul->Insert(point) ||
				m_ur->Insert(point) ||
				m_dl->Insert(point) ||
				m_dr->Insert(point) );
	}

	void Subdivide()
	{
		Quad tempQuad(0.0f, 0.0f, 0.0f, 0.0f);

		tempQuad.x = m_baseQuad.x - m_baseQuad.w * 0.25f;
		tempQuad.y = m_baseQuad.y + m_baseQuad.h * 0.25f;
		tempQuad.w = m_baseQuad.w * 0.5f;
		tempQuad.h = m_baseQuad.h * 0.5f;
		m_ul = new QuadTree(tempQuad, m_capacity);

		tempQuad.x = m_baseQuad.x + m_baseQuad.w * 0.25f;
		tempQuad.y = m_baseQuad.y + m_baseQuad.h * 0.25f;
		tempQuad.w = m_baseQuad.w * 0.5f;
		tempQuad.h = m_baseQuad.h * 0.5f;
		m_ur = new QuadTree(tempQuad, m_capacity);

		tempQuad.x = m_baseQuad.x - m_baseQuad.w * 0.25f;
		tempQuad.y = m_baseQuad.y - m_baseQuad.h * 0.25f;
		tempQuad.w = m_baseQuad.w * 0.5f;
		tempQuad.h = m_baseQuad.h * 0.5f;
		m_dl = new QuadTree(tempQuad, m_capacity);

		tempQuad.x = m_baseQuad.x + m_baseQuad.w * 0.25f;
		tempQuad.y = m_baseQuad.y - m_baseQuad.h * 0.25f;
		tempQuad.w = m_baseQuad.w * 0.5f;
		tempQuad.h = m_baseQuad.h * 0.5f;
		m_dr = new QuadTree(tempQuad, m_capacity);

		m_divided = true;

		for (int i = 0; i < m_points.size(); i++)
		{
			m_ul->Insert(m_points[i]);
			m_ur->Insert(m_points[i]);
			m_dl->Insert(m_points[i]);
			m_dr->Insert(m_points[i]);
		}

		this->m_points.clear();
	}

	std::vector<Point> Query(Quad quad)
	{
		std::vector<Point> result;

		if (!m_baseQuad.Intersects(quad))
		{
			return result;
		}

		if (m_divided)
		{
			std::vector<Point> ulPoints = m_ul->Query(quad);
			for (int i = 0; i < ulPoints.size(); i++)
			{
				result.push_back(ulPoints[i]);
			}

			std::vector<Point> urPoints = m_ur->Query(quad);
			for (int i = 0; i < urPoints.size(); i++)
			{
				result.push_back(urPoints[i]);
			}

			std::vector<Point> dlPoints = m_dl->Query(quad);
			for (int i = 0; i < dlPoints.size(); i++)
			{
				result.push_back(dlPoints[i]);
			}

			std::vector<Point> drPoints = m_dr->Query(quad);
			for (int i = 0; i < drPoints.size(); i++)
			{
				result.push_back(drPoints[i]);
			}

			return result;
		}

		for (int i = 0; i < m_points.size(); i++)
		{
			if (quad.Contains(m_points[i]))
			{
				result.push_back(m_points[i]);
			}
		}

		return result;
	}

private:
	QuadTree *m_ul = nullptr, *m_ur = nullptr, *m_dl = nullptr, *m_dr = nullptr;
	std::vector<Point> m_points;
	Quad m_baseQuad = Quad(0.0f, 0.0f, 2.0f, 2.0f);
	int m_capacity = 1;
	bool m_divided = false;
};
