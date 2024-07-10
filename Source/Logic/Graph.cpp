
#include "Graph.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstdio>
#include <string>
#include <sstream>


tEdgeCost Vertex::getDistanceTo(tID other) const
{
	if (auto it = m_neighbors.find(other); it != m_neighbors.end())
	{
		// you can also use vertex positions for calculating distance
		return 1;
	}
	return -1;
}

bool Vertex::addNeighbor(const Vertex* other)
{
	if (!other)
		return false;

	if (m_id == other->m_id)
		return false;

	auto [it, ok] = m_neighbors.insert({ other->getId(), other });
	return ok;
}

// =======================

void Dijkstra::process(const Vertex* start_vertex)
{
	if (!start_vertex)
		return;

	m_costs.clear();
	m_processed.clear();
	m_queue.clear();

	m_start_vert_id = start_vertex->getId();
	m_costs[m_start_vert_id] = 0;
	m_queue.insert(m_start_vert_id);

	while (!m_queue.empty())
	{
		auto min_id = *m_queue.begin();
		auto min_cost = m_costs[min_id];

		for (auto id : m_queue)
		{
			if (m_costs[id] < min_cost)
			{
				min_id = id;
				min_cost = m_costs[id];
			}
		}

		step(&m_vertexes.at(min_id)); // At first step it will be:  'step(start_vertex)'
	}
}

void Dijkstra::step(const Vertex* vertex)
{
	assert(m_costs.find(vertex->getId()) != m_costs.end());

	const auto& neighbors = vertex->getNeighbors();

	for (auto [id, v] : neighbors)
	{
		if (!v->enabled())
			continue;

		auto cost = m_costs[vertex->getId()] + vertex->getDistanceTo(id);
		auto it_cost = m_costs.find(id);
		if (it_cost == m_costs.end() || it_cost->second > cost)
			m_costs[id] = cost;

		if (m_processed.find(id) == m_processed.end())
			m_queue.insert(id);
	}

	m_processed.insert(vertex->getId());
	m_queue.erase(vertex->getId());
}

tEdgeCost Dijkstra::getCost(tID to_vertex) const
{
	auto it_cost = m_costs.find(to_vertex);
	if (it_cost == m_costs.end())
		return -1;

	return it_cost->second;
}

void Dijkstra::getPath(tID end_id, OUT std::vector<tID>& path) const
{
	if (m_start_vert_id == end_id)
	{
		// finish
		std::reverse(path.begin(), path.end());
		return;
	}

	auto it = m_vertexes.find(end_id);
	if (it == m_vertexes.end())
		return;

	tEdgeCost cost = getCost(end_id);
	if (cost < 0)
		return;

	for (auto [id, v] : it->second.getNeighbors())
	{
		if (cost == getCost(id) + v->getDistanceTo(end_id))
		{
			path.push_back(end_id);
			getPath(id, path);
			break;
		}
	}
}

// =======================

GraphConfig::GraphConfig()
{
	dijkstra = new Dijkstra(m_vertexes);
}

GraphConfig::~GraphConfig()
{
	if (dijkstra)
	{
		delete dijkstra;
		dijkstra = nullptr;
	}

	clear();
}

void GraphConfig::setupConnections()
{
	for (auto [i, j] : m_edges)
	{
		auto& v1 = m_vertexes.find(i)->second;
		auto& v2 = m_vertexes.find(j)->second;

		v1.addNeighbor(&v2);
		v2.addNeighbor(&v1);
	}
}

void GraphConfig::lockVertexes()
{
	for (auto vid : m_chips)
	{
		if (auto it = m_vertexes.find(vid); it != m_vertexes.end())
		{
			it->second.setEnabled(false);
		}
	}
}

auto GraphConfig::getPosition(const int id) const -> std::pair<float, float>
{
	if (auto it = m_vertexes.find(id); it != m_vertexes.end())
		return it->second.getPosition();

	return {};
}

std::set<tID> GraphConfig::getVertexIds() const
{
	std::set<tID> result;
	auto func = [] (auto& kvp) { return kvp.first; };
	std::transform(m_vertexes.begin(), m_vertexes.end(), std::inserter(result, result.begin()), func);
	return result;
}

std::set<tID> GraphConfig::getAvailableVertexes(tID id) const
{
	auto it = m_vertexes.find(id);
	if (it == m_vertexes.end())
		return {};

	const auto vertex = &it->second;

	if (dijkstra->getStartId() != id)
	{
		dijkstra->process(vertex);
	}

	const auto& costs = dijkstra->getCosts();

	std::set<tID> result;
	auto func = [](auto& kvp) { return kvp.first; };
	std::transform(costs.begin(), costs.end(), std::inserter(result, result.begin()), func);
	result.erase(id);

	return result;
}

tID GraphConfig::getVertexId(tChipId chip_id) const
{
	if (chip_id < 0 || chip_id >= m_chips.size())
		return -1;

	return m_chips[chip_id];
}

void GraphConfig::clear()
{
	m_vertexes.clear();
	m_edges.clear();
	m_chips.clear();
	m_chips_order.clear();
}

bool GraphConfig::moveChip(tChipId chip_id, tID vert_id, std::vector<tID>& path)
{
	if (chip_id < 0 || chip_id >= m_chips.size())
		return false;

	const tID cur_vid = m_chips[chip_id];
	if (vert_id == cur_vid) // the same vertex
		return false;

	auto vertex_src = getVertex(cur_vid);
	auto vertex_dst = getVertex(vert_id);

	if (!vertex_src || !vertex_dst)
		return false;

	if (!vertex_dst->enabled())
		return false;

	if (dijkstra->getStartId() != cur_vid)
		return false;

	dijkstra->getPath(vert_id, path);
	if (path.empty())
		return false;

	vertex_dst->setEnabled(false);
	vertex_src->setEnabled(true);

	m_chips[chip_id] = vert_id;
	dijkstra->process(vertex_dst);
	return true;
}

bool GraphConfig::isAllChipsOK() const
{
	return m_chips == m_chips_order;
}

Vertex* GraphConfig::getVertex(tID id)
{
	if (auto it = m_vertexes.find(id); it != m_vertexes.end())
	{
		return std::addressof(it->second);
	}
	return nullptr;
}


bool GraphConfig::fromFile(const std::string_view content) {
    clear();

    size_t chips_num = 0;
    size_t vertex_num = 0;

    std::istringstream file(std::string{content});

    bool ok = true;
    ok = ok && readValue(file, chips_num);
    ok = ok && readValue(file, vertex_num);

    tID vid = 0;
    for (size_t i = 0; i < vertex_num; i++, vid++) {
        float x, y;
        ok = ok && readValues(file, x, y);
        if (ok)
            m_vertexes.emplace(vid, Vertex{vid, x - 1, y - 1});
        else
            break;
    }

    // validation
    ok = ok && (vertex_num == m_vertexes.size());

    ok = ok && readVector(file, m_chips);
    ok = ok && readVector(file, m_chips_order);

    // validation
    ok = ok && (m_chips.size() == m_chips_order.size());
    ok = ok && std::all_of(m_chips.begin(), m_chips.end(),
                           [vertex_num](auto id) { return id > 0 && id <= vertex_num; });
    ok = ok && std::all_of(m_chips_order.begin(), m_chips_order.end(),
                           [vertex_num](auto id) { return id > 0 && id <= vertex_num; });

    size_t edges_num = 0;
    ok = ok && readValue(file, edges_num);

    for (auto i = 0; i < edges_num; i++) {
        tID v1, v2;
        ok = ok && readValues(file, v1, v2);
        if (ok)
            m_edges.emplace_back(v1 - 1, v2 - 1);
    }

    // validation
    ok = ok && std::all_of(m_edges.begin(), m_edges.end(), [vertex_num](auto kvp) {
        return kvp.first >= 0 && kvp.first < vertex_num && kvp.second >= 0 &&
               kvp.second < vertex_num;
    });

    if (!ok) {
        clear();
        return false;
    }

    std::transform(std::begin(m_chips), std::end(m_chips), std::begin(m_chips),
                   [](tID id) { return id - 1; });
    std::transform(std::begin(m_chips_order), std::end(m_chips_order), std::begin(m_chips_order),
                   [](tID id) { return id - 1; });
    setupConnections();
    lockVertexes();

    return ok;
}

bool GraphConfig::readValue(auto& file, OUT size_t& val) const
{
	std::string line;
	if (std::getline(file, line))
	{
		auto res = sscanf(line.c_str(), "%d", &val);
		return res == 1;
	}
	return false;
}

bool GraphConfig::readValues(auto& file, OUT float& val2, OUT float& val3)
{
	std::string line;
	if (std::getline(file, line))
	{
		auto res = sscanf(line.c_str(), "%f,%f", &val2, &val3);
		return res == 2;
	}
	return false;
}

bool GraphConfig::readValues(auto& file, OUT tID& val2, OUT tID& val3)
{
	std::string line;
	if (std::getline(file, line))
	{
		auto res = sscanf(line.c_str(), "%d,%d", &val2, &val3);
		return res == 2;
	}
	return false;
}

bool GraphConfig::readVector(auto& file, OUT std::vector<tID>& vector)
{
	std::string line;
	if (!std::getline(file, line))
		return false;

	std::replace(line.begin(), line.end(), ',', ' ');
	std::stringstream stream(line);
	std::vector<int> values(
		(std::istream_iterator<tID>(stream)), // begin
		(std::istream_iterator<tID>()));      // end

	vector.swap(values);

	return true;
}

