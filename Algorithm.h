//
// Created by ����Դ on 24-10-13.
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
            // ���鼯���캯��
            explicit DSU(int n):parent(n), rank(n, 1) {
                // TODO: ����ɲ��鼯�Ĺ��캯��
                for (int i = 0; i < n; ++i) {
            	parent[i] = i;
        		}
            }

            // ���Ҹ��ڵ㲢·��ѹ��
            int find(int x) {
                // TODO: ����Ԫ�صĸ��ڵ㣬�����·��ѹ��
                if (parent[x] != x){ 
	            parent[x] = find(parent[x]);
	        	}
	        return parent[x];
            }

            // �ϲ���������
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
            	
                // TODO: �ϲ�����
            }

            // ��������ڵ��Ƿ�����ͬһ����
            bool same(int x, int y) {
                return find(x) == find(y);
            }

        };

        struct ShortestPathResult {     //����������·�����Ⱥ�·��
            int distance;
            std::string path_string;
        };

        struct PlanVisitResult {
            std::string path_string; // �ص����ո�ָ���·��
            int total_time_minutes; // ��ʱ�䣨��������ι�ʱ�� + ·��ʱ�䣩
        };

        struct ConnectionEdge {
            std::string from;
            std::string to;
            int weight;
        };
        
        std::vector<ConnectionEdge> GetConnectionSuggestions(const LGraph& graph);

        std::list<Vertex> EulerCircle(const LGraph& graph); //����ŷ����·
        bool HaveEulerCircle(const LGraph& graph); //�ж��Ƿ����ŷ����·
        bool IsConnected(const LGraph& graph); //�ж�ͼ�Ƿ���ͨ
        ShortestPathResult GetShortestPath(const LGraph& graph, const std::string& vertex_name_x,
            const std::string& vertex_name_y); //���㵥Դ���·��
        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path); //�����������޵����·��
        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph); //������С������
        PlanVisitResult PlanTeachingVisit(const LGraph& graph, const std::string& start_gate);
    }
}
#endif //CAMPUSNAVIGATION_ALGORITHM_H
