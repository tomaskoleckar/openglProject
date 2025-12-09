#pragma once
#include <vector>
#include <glm/glm.hpp>

struct BezierSegment {
    glm::vec3 p0, p1, p2, p3;
};

class BezierSpline {
public:

    void addPoint(const glm::vec3& p) {
        m_points.push_back(p);
        rebuildSegments();
    }

    bool hasSegments() const { return !m_segments.empty(); }
    int segmentCount() const { return (int)m_segments.size(); }

    glm::vec3 evaluateSegment(int segIndex, float t, glm::vec3* tangentOut = nullptr) const {
        const auto& s = m_segments[segIndex];

        float u = 1.0f - t;
        float u2 = u * u;
        float u3 = u2 * u;
        float t2 = t * t;
        float t3 = t2 * t;

        glm::vec3 pos =
            u3 * s.p0 +
            3.0f * u2 * t * s.p1 +
            3.0f * u * t2 * s.p2 +
            t3 * s.p3;

        if (tangentOut) {
            glm::vec3 d =
                3.0f * u2 * (s.p1 - s.p0) +
                6.0f * u * t * (s.p2 - s.p1) +
                3.0f * t2 * (s.p3 - s.p2);

            *tangentOut = glm::normalize(d);
        }

        return pos;
    }

    glm::vec3 evaluate(float globalT, glm::vec3* tangentOut = nullptr) const {
        if (m_segments.empty())
            return glm::vec3(0);

        int seg = (int)glm::clamp(std::floor(globalT), 0.0f, float(m_segments.size() - 1));
        float t = globalT - seg;
        return evaluateSegment(seg, t, tangentOut);
    }

private:
    std::vector<glm::vec3> m_points;
    std::vector<BezierSegment> m_segments;

    void rebuildSegments() {
        m_segments.clear();
        if (m_points.size() < 4)
            return;

        for (int i = 0; i + 3 < (int)m_points.size(); i += 3) {
            BezierSegment seg;
            seg.p0 = m_points[i + 0];
            seg.p1 = m_points[i + 1];
            seg.p2 = m_points[i + 2];
            seg.p3 = m_points[i + 3];
            m_segments.push_back(seg);
        }
    }
};
