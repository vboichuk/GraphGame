#pragma once

#include <fstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

#include <cstdarg>

#define OUT

using tID = int;
using tChipId = int;
using tEdgeCost = short;

class Vertex final
{
public:
    Vertex(tID id, float x, float y) : m_id(id), m_position(x,y) { }
    tID getId() const { return m_id; }
    bool enabled() const { return m_enabled; }
    bool addNeighbor(const Vertex* other);
    const std::map<tID, const Vertex*>& getNeighbors() const { return m_neighbors; }
    tEdgeCost getDistanceTo(tID other) const;
    const auto& getPosition() const { return m_position; }
    void setEnabled(bool b) { m_enabled = b; }

    bool operator == (const Vertex& other) const { return m_id == other.getId(); }
    bool operator < (const Vertex& other) const { return m_id < other.getId(); }
    
protected:
    tID m_id;
    bool m_enabled = true;
    std::map<tID, const Vertex*> m_neighbors;
    std::pair<float, float> m_position;
};

class Dijkstra final
{
public:
    Dijkstra(const std::map<tID, Vertex>& vertexes) : m_vertexes(vertexes) { }

    void process(const Vertex* start_vertex);
    tEdgeCost getCost(tID to_vertex) const;
    const auto& getCosts() const { return m_costs; }
    tID getStartId() const { return m_start_vert_id; }
    void getPath(tID vert_id, OUT std::vector<tID>& path) const;

private:
    void step(const Vertex* vertex);
    std::map<tID, tEdgeCost> m_costs; // costs of vertexes
    std::set<tID> m_processed;
    std::set<tID> m_queue;
    tID m_start_vert_id = -1;

    const std::map<tID, Vertex>& m_vertexes;
};

struct ChipInfo
{
    tChipId id;
    tID vert_id;
    ChipInfo(tChipId _id, tID _vert_id):id(_id), vert_id(_vert_id) { }
};

class  GraphConfig final
{
public:
    GraphConfig();
    ~GraphConfig();

    auto getPosition(const int id) const ->std::pair<float, float>;
    const auto& getEdges() const { return m_edges; }

    std::set<tID> getVertexIds() const;
    std::set<tID> getAvailableVertexes(tID id) const;
    const auto& getChips() const { return m_chips; }
    const auto& getChipsOrder() const { return m_chips_order; }
    tID getVertexId(tChipId chip_id) const;
    bool moveChip(tChipId chip_id, tID vert_id, OUT std::vector<tID>& path);
    bool isAllChipsOK() const;
    bool fromFile(const char* path);
    bool fromFile(const std::string_view content);

    bool empty() const
    {
        return m_vertexes.empty() || m_edges.empty();
    }

protected:
    Vertex* getVertex(tID id);

    bool readValue(auto& file, OUT size_t& val) const;
    bool readValues(auto& file, OUT float& val2, OUT float& val3);
    bool readValues(auto& file, OUT tID& val2, OUT tID& val3);
    bool readVector(auto& file, OUT std::vector<tID>& vector);

    void setupConnections();
    void lockVertexes();

protected:
    std::vector<tID> m_chips;
    std::vector<tID> m_chips_order;
    std::map<tID, Vertex> m_vertexes;
    std::vector<std::pair<tID, tID>> m_edges;

    void clear();

    Dijkstra* dijkstra = nullptr;
};

