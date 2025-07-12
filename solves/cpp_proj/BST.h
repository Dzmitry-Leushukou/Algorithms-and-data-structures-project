#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
#include <map>
#include <set>

class BST
{
public:
    class Node
    {
    public:
        int val;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    std::string solveMaxPath() {
        if (!root) return "";

        std::map<Node*, int> heightMap;
        std::map<Node*, int> diameterMap;
        heightMap[nullptr] = -1;

        std::stack<Node*> stack;
        std::stack<Node*> output;
        stack.push(root);

        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();
            output.push(current);

            if (current->left) stack.push(current->left);
            if (current->right) stack.push(current->right);
        }

        while (!output.empty()) {
            Node* current = output.top();
            output.pop();

            int leftH = heightMap[current->left];
            int rightH = heightMap[current->right];
            heightMap[current] = 1 + std::max(leftH, rightH);

            std::vector<int> depths;
            if (current->left) depths.push_back(leftH + 1);
            if (current->right) depths.push_back(rightH + 1);
            std::sort(depths.rbegin(), depths.rend());

            if (depths.size() >= 2)
                diameterMap[current] = depths[0] + depths[1];
            else if (depths.size() == 1)
                diameterMap[current] = depths[0];
            else
                diameterMap[current] = 0;
        }

        int global_diameter = 0;
        for (const auto& kv : diameterMap) {
            if (kv.second > global_diameter)
                global_diameter = kv.second;
        }

        std::vector<Node*> candidates;
        for (const auto& kv : diameterMap) {
            if (kv.second == global_diameter)
                candidates.push_back(kv.first);
        }

        std::map<Node*, int> depthMap;
        std::queue<Node*> q;
        depthMap[root] = 0;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current->left) {
                depthMap[current->left] = depthMap[current] + 1;
                q.push(current->left);
            }
            if (current->right) {
                depthMap[current->right] = depthMap[current] + 1;
                q.push(current->right);
            }
        }

        Node* toRemove = nullptr;
        for (Node* node : candidates) {
            if (!toRemove) {
                toRemove = node;
            }
            else {
                int d1 = depthMap[node];
                int d2 = depthMap[toRemove];
                if (d1 < d2) {
                    toRemove = node;
                }
                else if (d1 == d2 && node->val > toRemove->val) {
                    toRemove = node;
                }
            }
        }

        if (toRemove) {
            root = deleteNodeRight(root, toRemove->val);
        }

        return leftByPass();
    }
	void insert(int val);
	void rightRemove(int val);
	int getHeight() const;
	int getMedianEq();
	int getMedianLeft();
	std::string leftByPass();
	std::string solveTask3();
	std::string solveMinPathMedian();
    bool sameStruct(Node* a, Node* b) 
    {
        std::stack<std::pair<Node*, Node*>> st;
        st.push({ a, b });

        while (!st.empty()) {
            auto [nodeA, nodeB] = st.top();
            st.pop();

            if (!nodeA && !nodeB) continue;
            if (!nodeA || !nodeB) return false;

            st.push({ nodeA->left, nodeB->left });
            st.push({ nodeA->right, nodeB->right });
        }
        return true;
    }

    bool findRemovalCandidate(BST& tree1, BST& tree2, int& result) {
        Node* T1 = tree1.root;
        Node* T2 = tree2.root;
        std::vector<Node*> candidates;

        if (T1 == nullptr) {
            if (T2 != nullptr) {
                candidates.push_back(T2);
            }
        }
        else if (T2 != nullptr) {
            std::queue<std::pair<Node*, Node*>> q; 
            std::map<Node*, std::pair<Node*, char>> parentMap; 

            q.push({ T2, nullptr });

            while (!q.empty()) {
                Node* cur = q.front().first;
                Node* parent = q.front().second;
                q.pop();

                if (parent != nullptr) {
                    char ch = parentMap[cur].second;
                    Node* saved = (ch == 'L') ? parent->left : parent->right;

                    if (ch == 'L') parent->left = nullptr;
                    else parent->right = nullptr;

                    if (sameStruct(T1, T2)) 
                    {
                        candidates.push_back(cur);
                    }

                    if (ch == 'L') parent->left = saved;
                    else parent->right = saved;
                }

                if (cur->left) 
                {
                    q.push({ cur->left, cur });
                    parentMap[cur->left] = { cur, 'L' };
                }
                if (cur->right) {
                    q.push({ cur->right, cur });
                    parentMap[cur->right] = { cur, 'R' };
                }
            }
        }

        if (candidates.empty()) return false;

        auto it = std::max_element(candidates.begin(), candidates.end(),
            [](Node* a, Node* b) { return a->val < b->val; });

        result = (*it)->val;
        return true;
    }

    static bool canBeObtained(BST& tree1, BST& tree2, int& result) 
    {
        return tree1.findRemovalCandidate(tree1, tree2, result);
    }

    std::string solveLeafMedianFather() {
        if (!root) return "";

        std::vector<Node*> leaves;
        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (!current->left && !current->right) {
                leaves.push_back(current);
            }

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }

        if (leaves.empty()) {
            return "";
        }
        if (leaves.size() == 1 && leaves[0] == root) {
            delete root;
            root = nullptr;
            return "";
        }

        std::sort(leaves.begin(), leaves.end(),
            [](Node* a, Node* b) { return a->val < b->val; });

        Node* medianLeaf = leaves[leaves.size() / 2];

        Node* father = nullptr;
        std::map<Node*, Node*> parentMap;
        parentMap[root] = nullptr;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current->left) {
                parentMap[current->left] = current;
                q.push(current->left);
            }
            if (current->right) {
                parentMap[current->right] = current;
                q.push(current->right);
            }
        }

        father = parentMap[medianLeaf];

        if (father) {
            root = deleteNodeRight(root, father->val);
        }

        return leftByPass();
    }


private:
	Node* root = nullptr;
	std::vector<int>ansCh;

	void getAnsChWithEqHeights(Node* v,int need);
	void getAnsChWithLeftHeights(Node* v, int need);
	Node* insertNode(Node* node, int val);
	Node* deleteNodeRight(Node* node, int val);
	int height(Node *node) const;
	Node* getMinNode(Node* node);
	void leftByPassHelper(Node* node, std::ostringstream& oss);
	int childs(Node* node);
	std::map<Node*, int> computeDepths();
    std::pair<int, std::vector<Node*>> computeDiameter();
    Node* selectCandidate(const std::vector<Node*>& candidates,
        const std::map<Node*, int>& depthMap);
	int findMinPathDepth();
	std::vector<std::vector<int>> collectMinPaths(int minDepth);
    std::set<int> findMedianNodes(const std::vector<std::vector<int>>& paths);
    Node* deleteNodeLeft(Node* root, int val);

};

