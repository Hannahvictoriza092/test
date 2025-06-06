//
// Created by 刘凯源 on 24-10-13.
//

#ifndef CAMPUSNAVIGATION_ALGORITHM_H
#define CAMPUSNAVIGATION_ALGORITHM_H

#include "LGraph.h"

namespace Graph {
    namespace Algorithm {
        class DSU {
        private:
            std::vector<int> parent, rank;
        public:
            // 并查集构造函数
            explicit DSU(int n):parent(n), rank(n, 1) {
                // TODO: 请完成并查集的构造函数
                for (int i = 0; i < n; ++i) {
            	parent[i] = i;
        		}
            }

            // 查找根节点并路径压缩
            int find(int x) {
                // TODO: 查找元素的根节点，并完成路径压缩
                if (parent[x] != x){ 
	            parent[x] = find(parent[x]);
	        	}
	        return parent[x];
            }

            // 合并两个集合
            void unite(int x, int y) {
            	
            	int fx = find(x), fy = find(y);
		        if (fx == fy) return;
		        if (rank[fx] > rank[fy]) {
		            parent[fy] = fx;
		        } 
		        else {
		            parent[fx] = fy;
		            if (rank[fx] == rank[fy]) rank[fy]++;
		        }
            	
                // TODO: 合并集合
            }

            // 检查两个节点是否属于同一集合
            bool same(int x, int y) {
                return find(x) == find(y);
            }

        };

        struct ShortestPathResult {     //用于输出最短路径长度和路径
            int distance;
            std::string path_string;
        };

        struct PlanVisitResult {
            std::string path_string; // 地点名空格分隔的路径
            int total_time_minutes; // 总时间（包括建议参观时间 + 路上时间）
        };

        struct ConnectionEdge {
            std::string from;
            std::string to;
            int weight;
        };
        
        std::vector<ConnectionEdge> GetConnectionSuggestions(const LGraph& graph);

        std::list<Vertex> EulerCircle(const LGraph& graph); //计算欧拉回路
        bool HaveEulerCircle(const LGraph& graph); //判断是否存在欧拉回路
        bool IsConnected(const LGraph& graph); //判断图是否联通
        ShortestPathResult GetShortestPath(const LGraph& graph, const std::string& vertex_name_x,
            const std::string& vertex_name_y); //计算单源最短路径
        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path); //计算拓扑受限的最短路径
        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph); //计算最小生成树
        PlanVisitResult PlanTeachingVisit(const LGraph& graph, const std::string& start_gate);
    }
}
#endif //CAMPUSNAVIGATION_ALGORITHM_H
