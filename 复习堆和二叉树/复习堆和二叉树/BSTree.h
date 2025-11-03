#pragma once
#include <iostream>
#include <string>
using namespace std;

namespace BST {
	template<class K, class V>
	struct BSTreeNode {
		pair<K, V> _kv;
		BSTreeNode<K, V>* _left;
		BSTreeNode<K, V>* _right;

		// 构造
		BSTreeNode(const pair<K, V>& kv)
			:_kv(kv)
			, _left(nullptr)
			, _right(nullptr) {
		}
	};

	template<class K, class V>
	class BSTree {
	public:
		typedef BSTreeNode<K, V> Node;
		BSTree() = default;

		V* find(const K& key) {
			Node* cur = _root;
			while (cur) {
				if (key < cur->_kv.first) cur = cur->_left;
				else if (cur->_kv.first < key) cur = cur->_right;
				else return &cur->_kv.second; // 命中，返回 value 指针
			}
			return nullptr;
		}
		const V* find(const K& key) const {
			Node* cur = _root;
			while (cur) {
				if (key < cur->_kv.first) cur = cur->_left;
				else if (cur->_kv.first < key) cur = cur->_right;
				else return &cur->_kv.second;
			}
			return nullptr;
		}

		// 插入
		bool insert(const pair<K, V>& kv) {
			// 空树 直接插入
			if (_root == nullptr) {
				_root = new Node(kv);
				return true;
			}

			// 非空树
			Node* cur = _root;
			Node* parent = nullptr; // 记录cur的父节点
			// 1. 查找
			while (cur) {
				if (kv.first < cur->_kv.first) {
					parent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) {
					parent = cur;
					cur = cur->_right;
				}
				else return false; // 相同值 不插入
			}

			// 2. 插入
			// 此时 cur就是待插入位置 要判断待插入位置在parent的哪一边
			cur = new Node(kv);
			if (cur->_kv.first < parent->_kv.first)
				parent->_left = cur;
			else parent->_right = cur;

			return true;
		}

		// 删除 
		bool erase(const pair<K, V>& kv) {
			// 空树不能删除
			if (_root == nullptr) return false;

			// 非空树
			// 1. 找待删除节点
			Node* cur = _root;
			Node* curParent = nullptr;
			while (cur) {
				if (kv.first < cur->_kv.first) {
					curParent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) {
					curParent = cur;
					cur = cur->_right;
				}
				else break;	//  命中
			}

			// 2. 删除 cur就是待删除节点

			if (cur == nullptr) return false;

			// 1> 0/1个孩子 
			if (cur->_left == nullptr || cur->_right == nullptr) {
				// 找cur的子节点
				Node* curChild = cur->_left == nullptr ? cur->_right : cur->_left;
				if (cur != _root) { // 删除的不是根
					if (curParent->_left == cur) curParent->_left = curChild;
					else curParent->_right = curChild;
				}
				else { // 删除的是根节点
					_root = curChild;
				}
				delete cur;
			}
			else {
				// 2> 2个孩子 找右子树最小子节点or左子树最大子节点
				Node* rightMin = cur->_right;
				Node* rightMinParent = nullptr;
				while (rightMin->_left) {
					rightMinParent = rightMin;
					rightMin = rightMin->_left;
				}
				// rightMin覆盖cur的值
				cur->_kv.first = rightMin->_kv.first;
				cur->_kv.second = rightMin->_kv.second;
				// 找到右子树最小子节点 它可能还有右孩子 这里不需要判空 就算是空也无危险
				Node* rightMinChild = rightMin->_right;
				// 连接rightMin的父子
				if (rightMinParent->_left == rightMin)
					rightMinParent->_left = rightMinChild;
				else rightMinParent->_right = rightMinChild;

				delete rightMin;
			}
			return true;
		}


		// 中序遍历 有序序列
		void inorder() { _inorder(_root); }
	private:
		Node* _root = nullptr;

		void _inorder(Node* root) {
			if (root == nullptr) return;

			_inorder(root->_left);
			cout << root->_kv.first << " ";
			_inorder(root->_right);
		}
	};

	// 输入中文 输出对应英文
	void dictionary() {
		BSTree<string, string> dict;
		dict.insert({ "书包", "bag" });
		dict.insert({ "水果", "fruit" });
		dict.insert({ "钢笔", "pen" });
		dict.insert({ "书", "book" });
		dict.insert({ "树", "tree" });

		// 插入词库中所有单词
		string str;
		while (cin >> str) {
			auto ret = dict.find(str);
			if (ret == nullptr) cout << "输入错误或词库中未包含该词" << str << endl;
			else cout << str << "英文单词：->" << *ret << endl;
		}
	}

	void testErase() {
		BSTree<int, int> t;

		// 构建一棵树：        5
		//                  /   \
	    //                 3     7
		//                / \   / \
	    //               2  4  6   8
		//                          \
	    //                           9
		int a[] = { 5,3,7,2,4,6,8,9 };
		for (int x : a) t.insert({ x, 0 });
		t.inorder();
		cout << endl;

		// 1) 删叶子节点：2
		cout << "erase(2): " << (t.erase({ 2,0 }) ? "true" : "false") << "\n";
		cout << "中序："; t.inorder();           // 3 4 5 6 7 8 9
		cout << endl;

		// 2) 删只有一个孩子的节点：8（它只有右孩子 9）
		cout << "erase(8): " << (t.erase({ 8,0 }) ? "true" : "false") << "\n";
		cout << "中序："; t.inorder();           // 3 4 5 6 7 9
		cout << endl;

		// 3) 删有两个孩子的节点：5（当前为根）
		cout << "erase(5): " << (t.erase({ 5,0 }) ? "true" : "false") << "\n";
		cout << "中序："; t.inorder();           // 3 4 6 7 9  或 3 4 6 7 9（视实现一致）
		cout << endl;

		// 4) 删不存在的键：42
		cout << "erase(42): " << (t.erase({ 42,0 }) ? "true" : "false") << "\n";

		// 5) 再次删除已删掉的键：2（应返回 false）
		cout << "erase(2): " << (t.erase({ 2,0 }) ? "true" : "false") << "\n";

	}


}