#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <set>
#include <utility>
#include <glm/glm.hpp>
#include <rttr/type.h>
#include <rttr/registration_friend.h>

namespace Lacrima
{
    class AStar3D
    {
    public:
        AStar3D();
        virtual ~AStar3D();

        void addPoint(int64_t id, const glm::vec3& position, float weight_scale = 1.0f);
        void removePoint(int64_t id);
        bool hasPoint(int64_t id) const;

        glm::vec3 getPointPosition(int64_t id) const;
        void setPointPosition(int64_t id, const glm::vec3& position);

        float getPointWeightScale(int64_t id) const;
        void setPointWeightScale(int64_t id, float weight_scale);

        void connectPoints(int64_t id, int64_t with_id, bool bidirectional = true);
        void disconnectPoints(int64_t id, int64_t with_id, bool bidirectional = true);
        bool arePointsConnected(int64_t id, int64_t with_id, bool bidirectional = true) const;

        std::vector<int64_t> getPointConnections(int64_t id) const;
        std::vector<int64_t> getPointIds() const;

        void setPointDisabled(int64_t id, bool disabled = true);
        bool isPointDisabled(int64_t id) const;

        int64_t getClosestPoint(const glm::vec3& position, bool include_disabled = false) const;
        glm::vec3 getClosestPositionInSegment(const glm::vec3& position) const;

        std::vector<glm::vec3> getPointPath(int64_t from_id, int64_t to_id, bool allow_partial_path = false);
        std::vector<int64_t> getIdPath(int64_t from_id, int64_t to_id, bool allow_partial_path = false);

        int64_t getAvailablePointId() const;
        int64_t getPointCount() const;
        void clear();

    protected:
        virtual float estimateCost(int64_t from_id, int64_t to_id);
        virtual float computeCost(int64_t from_id, int64_t to_id);

    private:
        struct Point
        {
            int64_t id = 0;
            glm::vec3 position{0.0f};
            float weight_scale = 1.0f;
            bool enabled = true;

            std::unordered_set<int64_t> neighbors;

            // Solver variables
            Point* prev_point = nullptr;
            float g_score = 0.0f;
            float f_score = 0.0f;
            uint64_t open_pass = 0;
            uint64_t closed_pass = 0;
        };

        mutable std::unordered_map<int64_t, std::unique_ptr<Point>> m_points;
        std::set<std::pair<int64_t, int64_t>> m_segments;
        mutable int64_t m_last_free_id = 0;
        uint64_t m_pass = 1;
        Point* m_last_closest_point = nullptr;

        bool solve(Point* begin_point, Point* end_point, bool allow_partial_path);

        RTTR_REGISTRATION_FRIEND
    };
}
