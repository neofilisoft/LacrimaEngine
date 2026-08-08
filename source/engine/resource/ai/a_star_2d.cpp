#include "a_star_2d.h"
#include "engine/core/base/macro.h"
#include <algorithm>
#include <glm/gtx/norm.hpp>
#include <rttr/registration>

namespace Lacrima
{
    AStar2D::AStar2D() = default;
    AStar2D::~AStar2D() = default;

    static glm::vec2 getClosestPointToSegment(const glm::vec2& p, const glm::vec2& s0, const glm::vec2& s1)
    {
        glm::vec2 s = s1 - s0;
        float len2 = glm::dot(s, s);
        if (len2 < 1e-6f)
        {
            return s0;
        }
        float t = glm::dot(p - s0, s) / len2;
        t = glm::clamp(t, 0.0f, 1.0f);
        return s0 + t * s;
    }

    void AStar2D::addPoint(int64_t id, const glm::vec2& position, float weight_scale)
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

    void AStar2D::removePoint(int64_t id)
    {
        auto it = m_points.find(id);
        if (it == m_points.end())
        {
            return;
        }

        for (int64_t n_id : it->second->neighbors)
        {
            auto nit = m_points.find(n_id);
            if (nit != m_points.end())
            {
                nit->second->neighbors.erase(id);
            }
        }

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

    bool AStar2D::hasPoint(int64_t id) const
    {
        return m_points.find(id) != m_points.end();
    }

    glm::vec2 AStar2D::getPointPosition(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return it->second->position;
        }
        return glm::vec2(0.0f);
    }

    void AStar2D::setPointPosition(int64_t id, const glm::vec2& position)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->position = position;
        }
    }

    float AStar2D::getPointWeightScale(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return it->second->weight_scale;
        }
        return 1.0f;
    }

    void AStar2D::setPointWeightScale(int64_t id, float weight_scale)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->weight_scale = weight_scale;
        }
    }

    void AStar2D::connectPoints(int64_t id, int64_t with_id, bool bidirectional)
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

    void AStar2D::disconnectPoints(int64_t id, int64_t with_id, bool bidirectional)
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

    bool AStar2D::arePointsConnected(int64_t id, int64_t with_id, bool bidirectional) const
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

    std::vector<int64_t> AStar2D::getPointConnections(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return std::vector<int64_t>(it->second->neighbors.begin(), it->second->neighbors.end());
        }
        return {};
    }

    std::vector<int64_t> AStar2D::getPointIds() const
    {
        std::vector<int64_t> ids;
        ids.reserve(m_points.size());
        for (const auto& kv : m_points)
        {
            ids.push_back(kv.first);
        }
        return ids;
    }

    void AStar2D::setPointDisabled(int64_t id, bool disabled)
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            it->second->enabled = !disabled;
        }
    }

    bool AStar2D::isPointDisabled(int64_t id) const
    {
        auto it = m_points.find(id);
        if (it != m_points.end())
        {
            return !it->second->enabled;
        }
        return true;
    }

    int64_t AStar2D::getClosestPoint(const glm::vec2& position, bool include_disabled) const
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

    glm::vec2 AStar2D::getClosestPositionInSegment(const glm::vec2& position) const
    {
        float closest_dist_sq = 1e20f;
        glm::vec2 closest_point{0.0f};

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

            glm::vec2 p = getClosestPointToSegment(position, from_point->position, to_point->position);
            float d = glm::distance2(position, p);
            if (d < closest_dist_sq)
            {
                closest_point = p;
                closest_dist_sq = d;
            }
        }

        return closest_point;
    }

    std::vector<glm::vec2> AStar2D::getPointPath(int64_t from_id, int64_t to_id, bool allow_partial_path)
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

        std::vector<glm::vec2> path(count);
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

    std::vector<int64_t> AStar2D::getIdPath(int64_t from_id, int64_t to_id, bool allow_partial_path)
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

    int64_t AStar2D::getAvailablePointId() const
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

    int64_t AStar2D::getPointCount() const
    {
        return static_cast<int64_t>(m_points.size());
    }

    void AStar2D::clear()
    {
        m_last_free_id = 0;
        m_points.clear();
        m_segments.clear();
        m_last_closest_point = nullptr;
    }

    float AStar2D::estimateCost(int64_t from_id, int64_t to_id)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a != m_points.end() && it_b != m_points.end())
        {
            return glm::distance(it_a->second->position, it_b->second->position);
        }
        return 0.0f;
    }

    float AStar2D::computeCost(int64_t from_id, int64_t to_id)
    {
        auto it_a = m_points.find(from_id);
        auto it_b = m_points.find(to_id);
        if (it_a != m_points.end() && it_b != m_points.end())
        {
            return glm::distance(it_a->second->position, it_b->second->position);
        }
        return 0.0f;
    }

    bool AStar2D::solve(Point* begin_point, Point* end_point, bool allow_partial_path)
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
        rttr::registration::class_<Lacrima::AStar2D>("AStar2D")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr)
            .method("addPoint", &Lacrima::AStar2D::addPoint)
            .method("removePoint", &Lacrima::AStar2D::removePoint)
            .method("hasPoint", &Lacrima::AStar2D::hasPoint)
            .method("getPointPosition", &Lacrima::AStar2D::getPointPosition)
            .method("setPointPosition", &Lacrima::AStar2D::setPointPosition)
            .method("getPointWeightScale", &Lacrima::AStar2D::getPointWeightScale)
            .method("setPointWeightScale", &Lacrima::AStar2D::setPointWeightScale)
            .method("connectPoints", &Lacrima::AStar2D::connectPoints)
            .method("disconnectPoints", &Lacrima::AStar2D::disconnectPoints)
            .method("arePointsConnected", &Lacrima::AStar2D::arePointsConnected)
            .method("getPointConnections", &Lacrima::AStar2D::getPointConnections)
            .method("getPointIds", &Lacrima::AStar2D::getPointIds)
            .method("setPointDisabled", &Lacrima::AStar2D::setPointDisabled)
            .method("isPointDisabled", &Lacrima::AStar2D::isPointDisabled)
            .method("getClosestPoint", &Lacrima::AStar2D::getClosestPoint)
            .method("getClosestPositionInSegment", &Lacrima::AStar2D::getClosestPositionInSegment)
            .method("getPointPath", &Lacrima::AStar2D::getPointPath)
            .method("getIdPath", &Lacrima::AStar2D::getIdPath)
            .method("getAvailablePointId", &Lacrima::AStar2D::getAvailablePointId)
            .method("getPointCount", &Lacrima::AStar2D::getPointCount)
            .method("clear", &Lacrima::AStar2D::clear);
    }
}
