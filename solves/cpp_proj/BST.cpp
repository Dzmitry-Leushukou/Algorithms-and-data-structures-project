#include "BST.h"

void BST::insert(int val)
{
	root = insertNode(root,val);
}

void BST::rightRemove(int val)
{
	deleteNodeRight(root,val);
}

int BST::getMedianEq()
{
	getAnsChWithEqHeights(root, height(root) / 2);
	if (ansCh.empty())
		return 0;
	std::sort(ansCh.begin(), ansCh.end());
	return  ansCh.at(ansCh.size() / 2);
}

BST::Node* BST::deleteNodeRight(BST::Node* root, int val) {
	Node* parent = nullptr;
	Node* current = root;

	while (current != nullptr && current->val != val)
	{
		parent = current;
		if (val < current->val) current = current->left;
		else current = current->right;
	}

	if (current == nullptr) return root; 

	if (current->right != nullptr) {
		Node* minParent = current;
		Node* minNode = current->right;

		while (minNode->left != nullptr) {
			minParent = minNode;
			minNode = minNode->left;
		}

		current->val = minNode->val;

		if (minParent->left == minNode) minParent->left = minNode->right;
		else minParent->right = minNode->right;

		delete minNode;
	}
	else {
		Node* newChild = current->left;
		if (parent == nullptr) root = newChild;
		else if (parent->left == current) parent->left = newChild;
		else parent->right = newChild;
		delete current;
	}
	return root;
}

int BST::getHeight() const
{
	return std::max(height(root),0);
}

int BST::height(BST::Node* node) const {
	if (node == nullptr) return -1;

	std::queue<Node*> q;
	q.push(node);
	int height = -1;

	while (!q.empty()) {
		int levelSize = q.size();
		height++;

		for (int i = 0; i < levelSize; i++) {
			Node* current = q.front();
			q.pop();

			if (current->left) q.push(current->left);
			if (current->right) q.push(current->right);
		}
	}
	return height;
}

BST::Node* BST::insertNode(BST::Node* root, int val) {
	if (root == nullptr) {
		return new Node(val);
	}

	Node* current = root;
	Node* parent = nullptr;

	while (current != nullptr) {
		parent = current;
		if (val < current->val) {
			current = current->left;
		}
		else if (val > current->val) {
			current = current->right;
		}
		else {
			return root;
		}
	}


	Node* newNode = new Node(val);

	if (val < parent->val) {
		parent->left = newNode;
	}
	else {
		parent->right = newNode;
	}

	return root;
}

void BST::getAnsChWithEqHeights(BST::Node* root, int need) {
	ansCh.clear();
	if (root == nullptr) return;

	std::queue<std::pair<Node*, int>> q;
	q.push({ root, 0 });

	while (!q.empty()) {
		auto [node, level] = q.front();
		q.pop();

		if (level == need) {
			if (childs(node->left) == childs(node->right)) {
				ansCh.push_back(node->val);
			}
		}
		else if (level < need) {
			if (node->left) q.push({ node->left, level + 1 });
			if (node->right) q.push({ node->right, level + 1 });
		}
	}
}

void BST::getAnsChWithLeftHeights(BST::Node* root, int need) {
	ansCh.clear();
	if (root == nullptr) return;

	std::queue<std::pair<Node*, int>> q;
	q.push({ root, 0 });

	while (!q.empty()) {
		auto [node, level] = q.front();
		q.pop();

		if (level == need) {
			if (childs(node->left) > childs(node->right)) {
				ansCh.push_back(node->val);
			}
		}
		else if (level < need) {
			if (node->left) q.push({ node->left, level + 1 });
			if (node->right) q.push({ node->right, level + 1 });
		}
	}
}

BST::Node* BST::getMinNode(Node* node) 
{
	while (node->left != nullptr) 
	{
		node = node->left;
	}
	return node;
}

std::string BST::leftByPass()
{
	std::ostringstream oss;
	leftByPassHelper(root, oss);
	return oss.str();	
}

int BST::getMedianLeft()
{
	getAnsChWithLeftHeights(root, height(root) / 2);
	if (ansCh.empty())
		return 0;
	std::sort(ansCh.begin(), ansCh.end());
	return  ansCh.at(ansCh.size() / 2);
}

void BST::leftByPassHelper(Node* node, std::ostringstream& oss)
{
	if (node == nullptr) return;

	std::stack<Node*> stack;
	stack.push(node);

	while (!stack.empty()) {
		Node* current = stack.top();
		stack.pop();
		oss << current->val << " ";
		if (current->right) stack.push(current->right);
		if (current->left) stack.push(current->left);
	}
}

int BST::childs(Node* node) {
	if (node == nullptr) return 0;

	std::stack<Node*> stack;
	stack.push(node);
	int count = 0;

	while (!stack.empty()) {
		Node* current = stack.top();
		stack.pop();
		count++;

		if (current->right) stack.push(current->right);
		if (current->left) stack.push(current->left);
	}
	return count;
}

std::string BST::solveTask3() 
{
	if (root == nullptr) return "";

	std::map<Node*, int> depthMap = computeDepths();

	auto [maxDiameter, candidateRoots] = computeDiameter();

	Node* toRemove = selectCandidate(candidateRoots, depthMap);

	if (toRemove) {
		root = deleteNodeRight(root, toRemove->val);
	}

	return leftByPass();
}

std::map<BST::Node*, int> BST::computeDepths()
{
	std::map<Node*, int> depthMap;
	std::queue<Node*> q;

	if (root) {
		q.push(root);
		depthMap[root] = 0;

		while (!q.empty()) {
			Node* cur = q.front();
			q.pop();
			int curDepth = depthMap[cur];

			if (cur->left) {
				depthMap[cur->left] = curDepth + 1;
				q.push(cur->left);
			}
			if (cur->right) {
				depthMap[cur->right] = curDepth + 1;
				q.push(cur->right);
			}
		}
	}
	return depthMap;
}

std::pair<int, std::vector<BST::Node*>> BST::computeDiameter()
{
	std::map<Node*, int> heightMap;
	std::stack<Node*> stack;
	int maxDiameter = 0;
	std::vector<Node*> candidateRoots;

	if (root) stack.push(root);

	while (!stack.empty()) {
		Node* cur = stack.top();
		bool childrenProcessed = true;

		if (cur->left && heightMap.find(cur->left) == heightMap.end()) {
			stack.push(cur->left);
			childrenProcessed = false;
		}
		else if (cur->right && heightMap.find(cur->right) == heightMap.end()) {
			stack.push(cur->right);
			childrenProcessed = false;
		}

		if (childrenProcessed) {
			int leftH = cur->left ? heightMap[cur->left] : 0;
			int rightH = cur->right ? heightMap[cur->right] : 0;

			heightMap[cur] = 1 + std::max(leftH, rightH);
			int currentDiameter = leftH + rightH + 1;

			if (currentDiameter > maxDiameter) {
				maxDiameter = currentDiameter;
				candidateRoots.clear();
				candidateRoots.push_back(cur);
			}
			else if (currentDiameter == maxDiameter) {
				candidateRoots.push_back(cur);
			}

			stack.pop();
		}
	}

	return { maxDiameter, candidateRoots };
}

BST::Node* BST::selectCandidate(const std::vector<BST::Node*>& candidates, const std::map<BST::Node*, int>& depthMap)
{
	Node* best = nullptr;
	int minDepth = INT_MAX;

	for (Node* cand : candidates) {
		auto it = depthMap.find(cand);
		if (it == depthMap.end()) continue;

		int d = it->second;
		if (d < minDepth) {
			minDepth = d;
			best = cand;
		}
		else if (d == minDepth && best && cand->val < best->val) {
			best = cand;
		}
	}
	return best;
}

std::string BST::solveMinPathMedian() 
{
	if (root == nullptr) return "";

	int minDepth = findMinPathDepth();
	
	std::vector<std::vector<int>> minPaths = collectMinPaths(minDepth);

	std::set<int> toDelete = findMedianNodes(minPaths);
	
	for (int val : toDelete) 
	{
		root = deleteNodeLeft(root, val);
	}
	
	return leftByPass();
}

int BST::findMinPathDepth() 
{
	int minDepth = INT_MAX;
	std::queue<std::pair<Node*, int>> q;
	q.push({ root, 1 });

	while (!q.empty()) {
		auto [node, depth] = q.front();
		q.pop();

		if (!node->left && !node->right) {
			if (depth < minDepth) minDepth = depth;
		}

		if (node->left) q.push({ node->left, depth + 1 });
		if (node->right) q.push({ node->right, depth + 1 });
	}

	return minDepth;
}

std::vector<std::vector<int>> BST::collectMinPaths(int minDepth)
{
	std::vector<std::vector<int>> minPaths;
	std::stack<std::pair<Node*, std::vector<int>>> stack;
	stack.push({ root, {root->val} });

	while (!stack.empty()) {
		auto [node, path] = stack.top();
		stack.pop();

		if (!node->left && !node->right) {
			if (path.size() == minDepth) {
				minPaths.push_back(path);
			}
		}

		if (node->right) {
			std::vector<int> newPath = path;
			newPath.push_back(node->right->val);
			stack.push({ node->right, newPath });
		}

		if (node->left) {
			std::vector<int> newPath = path;
			newPath.push_back(node->left->val);
			stack.push({ node->left, newPath });
		}
	}

	return minPaths;
}


std::set<int> BST::findMedianNodes(const std::vector<std::vector<int>>& paths) {
	std::set<int> toDelete;

	for (const auto& path : paths) {
		std::vector<int> sorted = path;
		std::sort(sorted.begin(), sorted.end());
		size_t n = sorted.size();

		int median;
		if (n % 2 == 1) {
			median = sorted[n / 2];
		}
		else {
			median = sorted[n / 2 - 1]; 
		}

		toDelete.insert(median);
	}

	return toDelete;
}

BST::Node* BST::deleteNodeLeft(Node* root, int val) {
	Node* parent = nullptr;
	Node* current = root;

	
	while (current && current->val != val) 
	{
		parent = current;
		if (val < current->val) current = current->left;
		else current = current->right;
	}

	if (!current) return root; 

	if (current->left) 
	{
		Node* maxParent = current;
		Node* maxNode = current->left;

		while (maxNode->right)
		{
			maxParent = maxNode;
			maxNode = maxNode->right;
		}

		current->val = maxNode->val;

		if (maxParent == current)
		{
			maxParent->left = maxNode->left;
		}
		else
		{
			maxParent->right = maxNode->left;
		}

		delete maxNode;
	}
	
	else {
		Node* newChild = current->right;
		if (!parent) 
		{
			root = newChild;
		}
		else if (parent->left == current)
		{
			parent->left = newChild;
		}
		else
		{
			parent->right = newChild;
		}

		delete current;
	}

	return root;
}