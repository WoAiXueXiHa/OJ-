#pragma once
#include <iostream>
#include <cassert>
using namespace std;

namespace RBT {
	enum Color { RED, BLACK };
	template <class K, class V>
	struct RBTreeNode {
		pair<K, V> _kv;
		RBTreeNode<K, V>* _left;
		RBTreeNode<K, V>* _right;
		RBTreeNode<K, V>* _parent;
		Color _col;

		// 构造 默认红色
		RBTreeNode(const pair<K, V>& kv, Color color = RED)
			:_kv(kv)
			,_left(nullptr)
			,_right(nullptr)
			,_col(color)
		{ }
	};

	template <class K, class V>
	class RBTree {
	public:
		typedef RBTreeNode<K, V> Node;
		RBTree() = default;
		// 拷贝构造 深拷贝
		RBTree(const RBTree& other){}
		
		// 查找
		bool find(const pair<K, V>& kv) {
			if (nullptr == _root) return  false;
			// 二分逻辑
			Node* cur = _root;

			while (cur) {
				if (kv.first < cur->_kv.first) cur = cur->_left;
				else if (kv.first > cur->_kv.first) cur = cur->_right;
				else return true;		// 命中
			}

			return false;
		}

		// 插入 红色节点
		bool insert(const pair<K, V>& kv) {
			if (nullptr == _root) {
				_root = new Node(kv);
				_root->_col = BLACK;
				return true;
			}

			// 先查找合适插入位置
			Node* cur = _root;
			Node* parent = nullptr;
			while (cur) {
				if (kv.first < cur->_kv.first) {
					parent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) {
					parent = cur;
					cur = cur->_parent;
				}
				else return false;		// 找到相同的值 不执行插入操作
			}

			// cur的位置就是待插入位置
			cur = new Node(kv);
			cur->_col = RED;

			if (cur->_kv.first < parent->_kv.first) cur = parent->_left;
			else cur = parent->_right;
			cur->_parent = parent;

			// 检测红黑树结构是否被破坏



		}
	private:
		Node* _root;
	};
}