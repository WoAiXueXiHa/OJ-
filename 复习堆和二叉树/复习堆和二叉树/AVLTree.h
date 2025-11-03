#pragma once
#include <iostream>

using namespace std;

namespace AVL {
	template<class K,class V>
	struct AVLTreeNode {
		pair<K, V> _kv;
		AVLTreeNode<K, V>* _left;
		AVLTreeNode<K, V>* _right;
		AVLTreeNode<K, V>* _parent;
		int _bf;

		// 构造
		AVLTreeNode(const pair<K,V>& kv)
			:_kv(kv)
			,_left(nullptr)
			,_right(nullptr)
			,_bf(0){ }
	};

	template<class K, class V>
	class AVLTree {
	public:
		AVLTree() = default;

		typedef AVLTreeNode<K, V> Node;

		// 查找 BST相同逻辑
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

			// 3. 调整平衡因子
			// 插入前 parent的平衡因子 只有三种情况 1 -1 0
			// 插入之后可能会变成 1 -1 2 -2 0
			// 1> 1  ：插入之前是0 插入到右边 向上调整
			// 2> -1 ：插入之前是0 插入到左边 向上调整
			// 3> 2  ：插入之前是1 插入到右边 不平衡 旋转
			// 4> -2 : 插入之前是-1 插入到左边 不平衡 旋转
			// 5> 0  : 插入前是-1 插入到右边 插入前是1 插入到左边
			while (parent) {
				if (cur == parent->_right) --parent->_bf;

			}
			return true;
		}
	private:
		Node* _root = nullptr;
	};
}