#include "a_star_3d.h"
#include "engine/core/base/macro.h"
#include <algorithm>
#include <glm/gtx/norm.hpp>
#include <rttr/registration>

namespace Lacrima
{
    AStar3D::AStar3D() = default;
    AStar3D::~AStar3D() = default;

    static glm::vec3 getClosestPointToSegment(const glm::vec3& p, const glm::vec3& s0, const glm::vec3& s1)
    {
        glm::vec3 s = s1 - s0;
        float len2 = glm::dot(s, s);
        if (len2 < 1e-6f)
        {
            return s0;
        }
        float t = glm::dot(p - s0, s) / len2;
        t = glm::clamp(t, 0.0f, 1.0f);
        return s0 + t * s;
    }

    void AStar3D::addPoint(int64_t id, const glm::vec3& position, float weight_scale)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->position = position;
            it->second->weight_scale = weight_scale;
        }
        else
        {
            auto point = std::make_unique<Point>();
            point->id = id;
            point->position = position;
            point->weight_scale = weight_scale;
            m_points[id] = std::move(point);
        }

        if (id >= m_last_free_id)
        {
            m_last_free_id = id + 1;
        }
    }

    void AStar3D::removePoint(int64_t id)
    {
        auto it = m_points.find(id);
        if (it == m_points.end())
        {
            return;
        }

        // Remove from neighbors
        for (int64_t n_id : it->second->neighbors)
        {
            auto nit = m_points.find(n_id);
            if (nit != m_points.end())
            {
                nit->second->neighbors.erase(id);
            }
        }

        // Erase segments
        for (auto sit = m_segments.begin(); sit != m_segments.end();)
        {
            if (sit->first == id || sit->second == id)
            {
                sit = m_segments.erase(sit);
            }
            else
            {
                ++sit;
            }
        }

        m_points.erase(it);
    }

    bool AStar3D::hasPoint(int64_t id) const
    {
        return m_points.find(id) != m_points.end();
    }

    glm::vec3 AStar3D::getPointPosition(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return it->second->position;
        }
        return glm::vec3(0.0f);
    }

    void AStar3D::setPointPosition(int64_t id, const glm::vec3& position)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->position = position;
        }
    }

    float AStar3D::getPointWeightScale(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return it->second->weight_scale;
        }
        return 1.0f;
    }

    void AStar3D::setPointWeightScale(int64_t id, float weight_scale)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->weight_scale = weight_scale;
        }
    }

    void AStar3D::connectPoints(int64_t id, int64_t with_id, bool bidirectional)
    {
        auto it_a = m_points.find(id);
        auto it_b = m_points.find(with_id);
        if (it_a == m_points.end() || it_b == m_points.end())
        {
            return;
        }

        it_a->second->neighbors.insert(with_id);
        if (bidirectional)
        {
            it_b->second->neighbors.insert(id);
        }

        m_segments.insert({std::min(id, with_id), std::max(id, with_id)});
    }

    void AStar3D::disconnectPoints(int64_t id, int64_t with_id, bool bidirectional)
    {
        auto it_a = m_points.find(id);
        auto it_b = m_points.find(with_id);
        if (it_a != m_points.end())
        {
            it_a->second->neighbors.erase(with_id);
        }
        if (bidirectional && it_b != m_points.end())
        {
            it_b->second->neighbors.erase(id);
        }

        // Only remove the segment if both connections are gone
        bool still_connected = false;
        if (it_a != m_points.end() && it_a->second->neighbors.count(with_id) > 0)
        {
            still_connected = true;
        }
        if (it_b != m_points.end() && it_b->second->neighbors.count(id) > 0)
        {
            still_connected = true;
        }

        if (!still_connected)
        {
            m_segments.erase({std::min(id, with_id), std::max(id, with_id)});
        }
    }

    bool AStar3D::arePointsConnected(int64_t id, int64_t with_id, bool bidirectional) const
    {
        auto it_a = m_points.find(id);
        auto it_b = m_points.find(with_id);
        if (it_a == m_points.end() || it_b == m_points.end())
        {
            return false;
        }

        bool a_to_b = it_a->second->neighbors.count(with_id) > 0;
        bool b_to_a = it_b->second->neighbors.count(id) > 0;

        if (bidirectional)
        {
            return a_to_b && b_to_a;
        }
        return a_to_b;
    }

    std::vector<int64_t> AStar3D::getPointConnections(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return std::vector<int64_t>(it->second->neighbors.begin(), it->second->neighbors.end());
        }
        return {};
    }

    std::vector<int64_t> AStar3D::getPointIds() const
    {
        std::vector<int64_t> ids;
        ids.reserve(m_points.size());
        for (const auto& kv : m_points)
        {
            ids.push_back(kv.first);
        }
        return ids;
    }

    void AStar3D::setPointDisabled(int64_t id, bool disabled)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->enabled = !disabled;
        }
    }

    bool AStar3D::isPointDisabled(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return !it->second->enabled;
        }
        return true;
    }

    int64_t AStar3D::getClosestPoint(const glm::vec3& position, bool include_disabled) const
    {
        int64_t closest_id = -1;
        float closest_dist_sq = 1e20f;

        for (const auto& kv : m_points)
        {
            if (!include_disabled && !kv.second->enabled)
            {
                continue;
            }

            float d = glm::distance2(position, kv.second->position);
            int64_t id = kv.first;
            if (d <= closest_dist_sq)
            {
                if (d == closest_dist_sq && id > closest_id)
                {
                    continue;
                }
                closest_dist_sq = d;
                closest_id = id;
            }
        }

        return closest_id;
    }

    glm::vec3 AStar3D::getClosestPositionInSegment(const glm::vec3& position) const
    {
        float closest_dist_sq = 1e20f;
        glm::vec3 closest_point{0.0f};

        for (const auto& seg : m_segments)
        {
            auto it_from = m_points.find(seg.first);
            auto it_to = m_points.find(seg.second);
            if (it_from == m_points.end() || it_to == m_points.end())
            {
                continue;
            }

            const Point* from_point = it_from->second.get();
            const Point* to_point = it_to->second.get();

            if (!from_point->enabled || !to_point->enabled)
            {
                continue;
            }

            glm::vec3 p = getClosestPointToSegment(position, from_point->position, to_point->position);
            float d = glm::distance2(position, p);
            if (d < closest_dist_sq)
            {
                closest_point = p;
                closest_dist_sq = d;
            }
        }

        return closest_point;
    }

    std::vector<glm::vec3> AStar3D::getPointPath(int64_t from_id, int64_t to_id, bool allow_partial_path)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a == m_points.end() || it_b == m_points.end())
        {
            return {};
        }

        Point* begin_point = it_a->second.get();
        Point* end_point = it_b->second.get();

        bool found_route = solve(begin_point, end_point, allow_partial_path);
        if (!found_route)
        {
            if (!allow_partial_path || m_last_closest_point == nullptr)
            {
                return {};
            }
            end_point = m_last_closest_point;
        }

        Point* p = end_point;
        int64_t count = 1;
        while (p != begin_point)
        {
            count++;
            p = p->prev_point;
        }

        std::vector<glm::vec3> path(count);
        Point* p2 = end_point;
        int64_t idx = count - 1;
        while (p2 != begin_point)
        {
            path[idx--] = p2->position;
            p2 = p2->prev_point;
        }
        path[0] = p2->position;

        return path;
    }

    std::vector<int64_t> AStar3D::getIdPath(int64_t from_id, int64_t to_id, bool allow_partial_path)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a == m_points.end() || it_b == m_points.end())
        {
            return {};
        }

        Point* begin_point = it_a->second.get();
        Point* end_point = it_b->second.get();

        bool found_route = solve(begin_point, end_point, allow_partial_path);
        if (!found_route)
        {
            if (!allow_partial_path || m_last_closest_point == nullptr)
            {
                return {};
            }
            end_point = m_last_closest_point;
        }

        Point* p = end_point;
        int64_t count = 1;
        while (p != begin_point)
        {
            count++;
            p = p->prev_point;
        }

        std::vector<int64_t> path(count);
        Point* p2 = end_point;
        int64_t idx = count - 1;
        while (p2 != begin_point)
        {
            path[idx--] = p2->id;
            p2 = p2->prev_point;
        }
        path[0] = p2->id;

        return path;
    }

    int64_t AStar3D::getAvailablePointId() const
    {
        if (m_points.find(m_last_free_id) == m_points.end())
        {
            return m_last_free_id;
        }
        while (m_points.find(m_last_free_id) != m_points.end())
        {
            m_last_free_id++;
        }
        return m_last_free_id;
    }

    int64_t AStar3D::getPointCount() const
    {
        return static_cast<int64_t>(m_points.size());
    }

    void AStar3D::clear()
    {
        m_last_free_id = 0;
        m_points.clear();
        m_segments.clear();
        m_last_closest_point = nullptr;
    }

    float AStar3D::estimateCost(int64_t from_id, int64_t to_id)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a != m_points.end() && it_b != m_points.end())
        {
            return glm::distance(it_a->second->position, it_b->second->position);
        }
        return 0.0f;
    }

    float AStar3D::computeCost(int64_t from_id, int64_t to_id)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a != m_points.end() && it_b != m_points.end())
        {
            return glm::distance(it_a->second->position, it_b->second->position);
        }
        return 0.0f;
    }

    bool AStar3D::solve(Point* begin_point, Point* end_point, bool allow_partial_path)
    {
        m_pass++;
        m_last_closest_point = nullptr;

        if (!begin_point->enabled)
        {
            return false;
        }
        if (begin_point == end_point)
        {
            return true;
        }
        if (!end_point->enabled && !allow_partial_path)
        {
            return false;
        }

        std::vector<Point*> open_list;
        auto compare_points = [](const Point* a, const Point* b) {
            if (a->f_score > b->f_score) return true;
            if (a->f_score < b->f_score) return false;
            return a->g_score < b->g_score;
        };

        begin_point->g_score = 0.0f;
        begin_point->f_score = estimateCost(begin_point->id, end_point->id);

        open_list.push_back(begin_point);
        std::make_heap(open_list.begin(), open_list.end(), compare_points);

        bool found_route = false;

        while (!open_list.empty())
        {
            std::pop_heap(open_list.begin(), open_list.end(), compare_points);
            Point* p = open_list.back();
            open_list.pop_back();

            float p_h = estimateCost(p->id, end_point->id);

            if (m_last_closest_point == nullptr || 
                (estimateCost(m_last_closest_point->id, end_point->id) > p_h) ||
                (estimateCost(m_last_closest_point->id, end_point->id) >= p_h && m_last_closest_point->g_score > p->g_score))
            {
                m_last_closest_point = p;
            }

            if (p == end_point)
            {
                found_route = true;
                break;
            }

            p->closed_pass = m_pass;

            for (int64_t neighbor_id : p->neighbors)
            {
                auto it = m_points.find(neighbor_id);
                if (it == m_points.end())
                {
                    continue;
                }

                Point* e = it->second.get();
                if (!e->enabled || e->closed_pass == m_pass)
                {
                    continue;
                }

                float tentative_g_score = p->g_score + computeCost(p->id, e->id) * e->weight_scale;
                bool new_point = false;

                if (e->open_pass != m_pass)
                {
                    e->open_pass = m_pass;
                    new_point = true;
                }
                else if (tentative_g_score >= e->g_score)
                {
                    continue;
                }

                e->prev_point = p;
                e->g_score = tentative_g_score;
                e->f_score = e->g_score + estimateCost(e->id, end_point->id);

                if (new_point)
                {
                    open_list.push_back(e);
                    std::push_heap(open_list.begin(), open_list.end(), compare_points);
                }
                else
                {
                    std::make_heap(open_list.begin(), open_list.end(), compare_points);
                }
            }
        }

        return found_route;
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Lacrima::AStar3D>("AStar3D")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr)
            .method("addPoint", &Lacrima::AStar3D::addPoint)
            .method("removePoint", &Lacrima::AStar3D::removePoint)
            .method("hasPoint", &Lacrima::AStar3D::hasPoint)
            .method("getPointPosition", &Lacrima::AStar3D::getPointPosition)
            .method("setPointPosition", &Lacrima::AStar3D::setPointPosition)
            .method("getPointWeightScale", &Lacrima::AStar3D::getPointWeightScale)
            .method("setPointWeightScale", &Lacrima::AStar3D::setPointWeightScale)
            .method("connectPoints", &Lacrima::AStar3D::connectPoints)
            .method("disconnectPoints", &Lacrima::AStar3D::disconnectPoints)
            .method("arePointsConnected", &Lacrima::AStar3D::arePointsConnected)
            .method("getPointConnections", &Lacrima::AStar3D::getPointConnections)
            .method("getPointIds", &Lacrima::AStar3D::getPointIds)
            .method("setPointDisabled", &Lacrima::AStar3D::setPointDisabled)
            .method("isPointDisabled", &Lacrima::AStar3D::isPointDisabled)
            .method("getClosestPoint", &Lacrima::AStar3D::getClosestPoint)
            .method("getClosestPositionInSegment", &Lacrima::AStar3D::getClosestPositionInSegment)
            .method("getPointPath", &Lacrima::AStar3D::getPointPath)
            .method("getIdPath", &Lacrima::AStar3D::getIdPath)
            .method("getAvailablePointId", &Lacrima::AStar3D::getAvailablePointId)
            .method("getPointCount", &Lacrima::AStar3D::getPointCount)
            .method("clear", &Lacrima::AStar3D::clear);
    }
}
