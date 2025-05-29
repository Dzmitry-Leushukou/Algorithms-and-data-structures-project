from collections import deque
from queue import Queue

class BST:
    class Node:
        def __init__(self, val):
            self.val = val
            self.left = None
            self.right = None

    def __init__(self):
        self.root = None
        self.ansCh = []

    def insert(self, val):
        self.root = self._insert_node(self.root, val)

    def right_remove(self, val):
        self.root = self._delete_node_right(self.root, val)

    def get_median_eq(self):
        self._get_ans_ch_with_eq_heights(self.root, self._height(self.root) // 2)
        if not self.ansCh:
            return 0
        self.ansCh.sort()
        return self.ansCh[len(self.ansCh) // 2]

    def get_median_left(self):
        self._get_ans_ch_with_left_heights(self.root, self._height(self.root) // 2)
        if not self.ansCh:
            return 0
        self.ansCh.sort()
        return self.ansCh[len(self.ansCh) // 2]

    def get_height(self):
        return max(self._height(self.root), 0)

    def left_by_pass(self):
        result = []
        self._left_by_pass_helper(self.root, result)
        return ' '.join(map(str, result))

    def solve_task3(self):
        if not self.root:
            return ""
        depth_map = self._compute_depths()
        max_diameter, candidate_roots = self._compute_diameter()
        to_remove = self._select_candidate(candidate_roots, depth_map)
        if to_remove:
            self.root = self._delete_node_right(self.root, to_remove.val)
        return self.left_by_pass()

    def solve_min_path_median(self):
        if not self.root:
            return ""
        min_depth = self._find_min_path_depth()
        min_paths = self._collect_min_paths(min_depth)
        to_delete = self._find_median_nodes(min_paths)
        for val in to_delete:
            self.root = self._delete_node_left(self.root, val)
        return self.left_by_pass()

    def solve_max_path(self):
        if not self.root:
            return ""
        height_map = {None: -1}
        diameter_map = {}
        stack = deque([self.root]) if self.root else []
        output = deque()

        while stack:
            current = stack.pop()
            output.append(current)
            if current.left:
                stack.append(current.left)
            if current.right:
                stack.append(current.right)

        while output:
            current = output.pop()
            left_h = height_map.get(current.left, -1)
            right_h = height_map.get(current.right, -1)
            height_map[current] = 1 + max(left_h, right_h)
            depths = []
            if current.left:
                depths.append(left_h + 1)
            if current.right:
                depths.append(right_h + 1)
            depths.sort(reverse=True)
            diameter_map[current] = depths[0] + depths[1] if len(depths) >= 2 else depths[0] if depths else 0

        global_diameter = max(diameter_map.values(), default=0)
        candidates = [node for node, dia in diameter_map.items() if dia == global_diameter]

        depth_map = {}
        q = Queue()
        if self.root:
            depth_map[self.root] = 0
            q.put(self.root)

        while not q.empty():
            current = q.get()
            if current.left:
                depth_map[current.left] = depth_map[current] + 1
                q.put(current.left)
            if current.right:
                depth_map[current.right] = depth_map[current] + 1
                q.put(current.right)

        to_remove = None
        for node in candidates:
            if not to_remove:
                to_remove = node
            else:
                d1, d2 = depth_map[node], depth_map[to_remove]
                if d1 < d2 or (d1 == d2 and node.val > to_remove.val):
                    to_remove = node

        if to_remove:
            self.root = self._delete_node_right(self.root, to_remove.val)
        return self.left_by_pass()

    def solve_leaf_median_father(self):
        if not self.root:
            return ""
        leaves = []
        q = Queue()
        q.put(self.root)

        while not q.empty():
            current = q.get()
            if not current.left and not current.right:
                leaves.append(current)
            if current.left:
                q.put(current.left)
            if current.right:
                q.put(current.right)

        if not leaves:
            return ""
        if len(leaves) == 1 and leaves[0] == self.root:
            self.root = None
            return ""

        leaves.sort(key=lambda x: x.val)
        median_leaf = leaves[len(leaves) // 2]

        parent_map = {self.root: None}
        q.put(self.root)
        while not q.empty():
            current = q.get()
            if current.left:
                parent_map[current.left] = current
                q.put(current.left)
            if current.right:
                parent_map[current.right] = current
                q.put(current.right)

        father = parent_map.get(median_leaf)
        if father:
            self.root = self._delete_node_right(self.root, father.val)
        return self.left_by_pass()

    @staticmethod
    def can_be_obtained(tree1, tree2):
        result = [0]  # Use list to allow modification in nested function
        success = tree1._find_removal_candidate(tree1, tree2, result)
        return success, result[0]

    def _find_removal_candidate(self, tree1, tree2, result):
        t1 = tree1.root
        t2 = tree2.root
        candidates = []
        if not t1:
            if t2:
                candidates.append(t2)
        elif t2:
            q = Queue()
            parent_map = {}
            q.put((t2, None))
            while not q.empty():
                cur, parent = q.get()
                if parent:
                    ch = parent_map[cur][1]
                    saved = parent.left if ch == 'L' else parent.right
                    if ch == 'L':
                        parent.left = None
                    else:
                        parent.right = None
                    if self._same_struct(t1, t2):
                        candidates.append(cur)
                    if ch == 'L':
                        parent.left = saved
                    else:
                        parent.right = saved
                if cur.left:
                    q.put((cur.left, cur))
                    parent_map[cur.left] = (cur, 'L')
                if cur.right:
                    q.put((cur.right, cur))
                    parent_map[cur.right] = (cur, 'R')
        if not candidates:
            return False
        max_node = max(candidates, key=lambda x: x.val)
        result[0] = max_node.val
        return True

    def _same_struct(self, a, b):
        stack = deque([(a, b)])
        while stack:
            node_a, node_b = stack.pop()
            if not node_a and not node_b:
                continue
            if not node_a or not node_b:
                return False
            stack.append((node_a.left, node_b.left))
            stack.append((node_a.right, node_b.right))
        return True

    def _insert_node(self, node, val):
        if not node:
            return self.Node(val)
        current = node
        parent = None
        while current:
            parent = current
            if val < current.val:
                current = current.left
            elif val > current.val:
                current = current.right
            else:
                return node
        new_node = self.Node(val)
        if val < parent.val:
            parent.left = new_node
        else:
            parent.right = new_node
        return node

    def _delete_node_right(self, root, val):
        parent = None
        current = root
        while current and current.val != val:
            parent = current
            current = current.left if val < current.val else current.right
        if not current:
            return root
        if current.right:
            min_parent = current
            min_node = current.right
            while min_node.left:
                min_parent = min_node
                min_node = min_node.left
            current.val = min_node.val
            if min_parent.left == min_node:
                min_parent.left = min_node.right
            else:
                min_parent.right = min_node.right
        else:
            new_child = current.left
            if not parent:
                root = new_child
            elif parent.left == current:
                parent.left = new_child
            else:
                parent.right = new_child
        return root

    def _delete_node_left(self, root, val):
        parent = None
        current = root
        while current and current.val != val:
            parent = current
            current = current.left if val < current.val else current.right
        if not current:
            return root
        if current.left:
            max_parent = current
            max_node = current.left
            while max_node.right:
                max_parent = max_node
                max_node = max_node.right
            current.val = max_node.val
            if max_parent == current:
                max_parent.left = max_node.left
            else:
                max_parent.right = max_node.left
        else:
            new_child = current.right
            if not parent:
                root = new_child
            elif parent.left == current:
                parent.left = new_child
            else:
                parent.right = new_child
        return root

    def _height(self, node):
        if not node:
            return -1
        q = Queue()
        q.put(node)
        height = -1
        while not q.empty():
            level_size = q.qsize()
            height += 1
            for _ in range(level_size):
                current = q.get()
                if current.left:
                    q.put(current.left)
                if current.right:
                    q.put(current.right)
        return height

    def _get_ans_ch_with_eq_heights(self, root, need):
        self.ansCh.clear()
        if not root:
            return
        q = deque([(root, 0)])
        while q:
            node, level = q.popleft()
            if level == need:
                if self._childs(node.left) == self._childs(node.right):
                    self.ansCh.append(node.val)
            elif level < need:
                if node.left:
                    q.append((node.left, level + 1))
                if node.right:
                    q.append((node.right, level + 1))

    def _get_ans_ch_with_left_heights(self, root, need):
        self.ansCh.clear()
        if not root:
            return
        q = deque([(root, 0)])
        while q:
            node, level = q.popleft()
            if level == need:
                if self._childs(node.left) > self._childs(node.right):
                    self.ansCh.append(node.val)
            elif level < need:
                if node.left:
                    q.append((node.left, level + 1))
                if node.right:
                    q.append((node.right, level + 1))

    def _left_by_pass_helper(self, node, result):
        if not node:
            return result
        stack = [node]
        while stack:
            current = stack.pop()
            result.append(current.val)
            if current.right:
                stack.append(current.right)
            if current.left:
                stack.append(current.left)
        return result

    def _childs(self, node):
        if not node:
            return 0
        stack = [node]
        count = 0
        while stack:
            current = stack.pop()
            count += 1
            if current.right:
                stack.append(current.right)
            if current.left:
                stack.append(current.left)
        return count

    def _compute_depths(self):
        depth_map = {}
        if self.root:
            q = Queue()
            q.put(self.root)
            depth_map[self.root] = 0
            while not q.empty():
                cur = q.get()
                cur_depth = depth_map[cur]
                if cur.left:
                    depth_map[cur.left] = cur_depth + 1
                    q.put(cur.left)
                if cur.right:
                    depth_map[cur.right] = cur_depth + 1
                    q.put(cur.right)
        return depth_map

    def _compute_diameter(self):
        height_map = {}
        stack = deque()
        max_diameter = 0
        candidate_roots = []
        if self.root:
            stack.append(self.root)
        while stack:
            cur = stack[-1]
            children_processed = True
            if cur.left and cur.left not in height_map:
                stack.append(cur.left)
                children_processed = False
            elif cur.right and cur.right not in height_map:
                stack.append(cur.right)
                children_processed = False
            if children_processed:
                left_h = height_map.get(cur.left, 0)
                right_h = height_map.get(cur.right, 0)
                height_map[cur] = 1 + max(left_h, right_h)
                current_diameter = left_h + right_h + 1
                if current_diameter > max_diameter:
                    max_diameter = current_diameter
                    candidate_roots = [cur]
                elif current_diameter == max_diameter:
                    candidate_roots.append(cur)
                stack.pop()
        return max_diameter, candidate_roots

    def _select_candidate(self, candidates, depth_map):
        best = None
        min_depth = float('inf')
        for cand in candidates:
            d = depth_map.get(cand, float('inf'))
            if d < min_depth:
                min_depth = d
                best = cand
            elif d == min_depth and best and cand.val < best.val:
                best = cand
        return best

    def _find_min_path_depth(self):
        if not self.root:
            return float('inf')
        min_depth = float('inf')
        q = deque([(self.root, 1)])
        while q:
            node, depth = q.popleft()
            if not node.left and not node.right:
                min_depth = min(min_depth, depth)
            if node.left:
                q.append((node.left, depth + 1))
            if node.right:
                q.append((node.right, depth + 1))
        return min_depth

    def _collect_min_paths(self, min_depth):
        min_paths = []
        if not self.root:
            return min_paths
        stack = deque([(self.root, [self.root.val])])
        while stack:
            node, path = stack.pop()
            if not node.left and not node.right:
                if len(path) == min_depth:
                    min_paths.append(path)
            if node.right:
                stack.append((node.right, path + [node.right.val]))
            if node.left:
                stack.append((node.left, path + [node.left.val]))
        return min_paths

    def _find_median_nodes(self, paths):
        to_delete = set()
        for path in paths:
            sorted_path = sorted(path)
            n = len(sorted_path)
            median = sorted_path[n // 2 - 1] if n % 2 == 0 else sorted_path[n // 2]
            to_delete.add(median)
        return to_delete

class FileService:
    def __init__(self, bst, path):
        self.bst = bst
        self.path = path

    def read(self):
        try:
            with open(self.path, 'r') as f:
                for line in f:
                    line = line.strip()
                    if line and line.startswith('+'):
                        try:
                            value = int(line[2:].strip())
                            self.bst.insert(value)
                        except ValueError:
                            print(f"Invalid data in {self.path}: {line}")
        except FileNotFoundError:
            print(f"File not found: {self.path}")
        except Exception as e:
            print(f"Error reading {self.path}: {e}")