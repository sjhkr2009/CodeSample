#pragma once

#include <iostream>

namespace MyTree
{
	/// <summary>
	/// 트리의 요소에 해당하는 노드 구조체
	/// </summary>
	struct Node
	{
		int mData;
		Node* pLeft;
		Node* pRight;

		Node(int data)
		{
			mData = data;
			pLeft = pRight = nullptr;
		}
		~Node()
		{
			mData = 0;
			pLeft = pRight = nullptr;
		}
	};

	/// <summary>
	/// 이진 탐색 트리
	/// </summary>
	class BST
	{
	private:
		Node* mpRoot;

	public:
		enum TraversalMode
		{
			PreOrder,
			InOrder,
			PostOrder
		};

	public:
		BST() :
			mpRoot(nullptr) {}
		~BST() { Clear(); }

		/// <summary>
		/// 트리를 순회하는 함수입니다. 기본적으로 In-Order 모드로 순회합니다.
		/// </summary>
		/// <param name="mode">전위 탐색(Pre-Order) 또는 후위 탐색(Post-Order)을 위해 사용하는 변수</param>
		void Traversal(TraversalMode mode = InOrder)
		{
			if (mpRoot == nullptr)
			{
				std::cout << "트리가 비어 있습니다." << std::endl;
				return;
			}

			switch (mode)
			{
				case PreOrder:
					TravPreOrder(mpRoot);
					break;
				case InOrder:
					TravInOrder(mpRoot);
					break;
				case PostOrder:
					TravPostOrder(mpRoot);
					break;
				default:
					break;
			}
			std::cout << std::endl;
		}

		/// <summary>
		/// 트리에 값을 추가하고 자동으로 정렬합니다. 중복된 값은 추가되지 않습니다.
		/// </summary>
		void Insert(int data)
		{
			if (mpRoot == nullptr)
			{
				mpRoot = CreateNode(data);
				return;
			}

			Node* pParent{ mpRoot };

			while (pParent != nullptr)
			{
				if (pParent->mData < data)
				{
					if (pParent->pRight == nullptr)
					{
						pParent->pRight = CreateNode(data);
						return;
					}
					else
					{
						pParent = pParent->pRight;
					}
				}
				else if (pParent->mData > data)
				{
					if (pParent->pLeft == nullptr)
					{
						pParent->pLeft = CreateNode(data);
						return;
					}
					else
					{
						pParent = pParent->pLeft;
					}
				}
				else
				{
					std::cout << "중복된 값을 입력했습니다." << std::endl;
					return;
				}
			}
		}

		/// <summary>
		/// 특정한 값을 가진 노드를 찾아 반환합니다.
		/// </summary>
		Node* Find(int data)
		{
			Node* pResult{ _Find(data) };

			if (pResult == nullptr)
			{
				std::cout << "해당 값이 트리에 없습니다." << std::endl;
			}
			else
			{
				std::cout << pResult->mData << "가 탐색됨" << std::endl;
			}
			return pResult;
		}

		/// <summary>
		/// 특정한 값을 가진 노드를 찾아 제거합니다. 자식 노드가 1개일 경우 삭제한 자리는 해당 노드로 대체됩니다.
		/// 자식 노드가 2개일 경우 서브 트리에서 삭제하려는 값과 가장 근접한 값을 가진 리프 노드로 대체합니다.
		/// </summary>
		bool Erase(int data)
		{
			Node* pTarget{ _Find(data) };
			Node* pParent{ _Find(data, true) };
			if (pTarget == nullptr)
			{
				std::cout << "삭제하려는 값이 존재하지 않습니다." << std::endl;
				return false;
			}

			if ((pTarget->pLeft == nullptr) && (pTarget->pRight == nullptr))
			{
				//자식이 없을 때
				if (pParent != nullptr)
				{
					if (pParent->pLeft == pTarget)
					{
						pParent->pLeft = nullptr;
					}
					else
					{
						pParent->pRight = nullptr;
					}
				}
			}
			else if (pTarget->pLeft == nullptr)
			{
				// 자식이 하나일 때 (오른쪽)
				if (pParent != nullptr)
				{
					if (pParent->pLeft == pTarget)
					{
						pParent->pLeft = pTarget->pRight;
					}
					else
					{
						pParent->pRight = pTarget->pRight;
					}
				}
			}
			else if (pTarget->pRight == nullptr)
			{
				// 자식이 하나일 때 (왼쪽)
				if (pParent != nullptr)
				{
					if (pParent->pLeft == pTarget)
					{
						pParent->pLeft = pTarget->pLeft;
					}
					else
					{
						pParent->pRight = pTarget->pLeft;
					}
				}
			}
			else
			{
				// 자식이 둘일 때
				Node* pAlter{ GetMinChild(pTarget->pRight) };
				_Find(pAlter->mData, true)->pLeft = nullptr;
				if (pParent != nullptr)
				{
					if (pParent->pLeft == pTarget)
					{
						pParent->pLeft = pAlter;
					}
					else
					{
						pParent->pRight = pAlter;
					}
				}
				pAlter->pLeft = pTarget->pLeft;
				pAlter->pRight = pTarget->pRight;
				if (pTarget == mpRoot)
				{
					mpRoot = pAlter;
				}
			}
			
			delete pTarget;
			return true;
		}

		/// <summary>
		/// 리프의 모든 요소를 제거하고, 루트 노드를 nullptr로 초기화합니다.
		/// </summary>
		void Clear()
		{
			Delete(mpRoot);
			mpRoot = nullptr;
		}


	private:
		/// <summary>
		/// 특정 값을 가지는 노드를 생성하여 반환합니다.
		/// </summary>
		Node* CreateNode(int data) { return new Node(data); }
		/// <summary>
		/// 특정 노드를 탐색합니다. 노드가 가진 값을 출력합니다.
		/// </summary>
		void Visit(Node* pNode) { std::cout << pNode->mData << " "; }

		/// <summary>
		/// 트리를 전위 순회합니다.
		/// </summary>
		void TravInOrder(Node* pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			TravInOrder(pNode->pLeft);
			Visit(pNode);
			TravInOrder(pNode->pRight);
		}
		/// <summary>
		/// 트리를 중위 순회합니다. 작은 값부터 정렬된 형태로 출력됩니다.
		/// </summary>
		void TravPreOrder(Node* pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			Visit(pNode);
			TravPreOrder(pNode->pLeft);
			TravPreOrder(pNode->pRight);
		}
		/// <summary>
		/// 트리를 후위 순회합니다.
		/// </summary>
		void TravPostOrder(Node* pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			TravPostOrder(pNode->pLeft);
			TravPostOrder(pNode->pRight);
			Visit(pNode);
		}
		/// <summary>
		/// 특정한 값을 가진 노드를 찾아 반환합니다.
		/// 두 번재 인자로 true가 입력될 경우 해당 노드의 부모 노드를 반환합니다.
		/// </summary>
		Node* _Find(int data, bool getParent = false)
		{
			Node* pNow{ mpRoot };
			Node* pParent{ nullptr };
			while (pNow != nullptr)
			{
				if (data == pNow->mData)
				{
					return getParent ? pParent : pNow;
				}
				pParent = pNow;
				pNow = (data > pNow->mData) ? pNow->pRight : pNow->pLeft;
			}

			return nullptr;
		}
		/// <summary>
		/// 자식 노드 중 가장 큰 값을 가진 노드를 찾아 반환합니다.
		/// </summary>
		Node* GetMaxChild(Node* pParent)
		{
			if (pParent->pRight == nullptr)
			{
				return pParent;
			}
			else
			{
				return GetMaxChild(pParent->pRight);
			}
		}
		/// <summary>
		/// 자식 노드 중 가장 작은 값을 가진 노드를 찾아 반환합니다.
		/// </summary>
		Node* GetMinChild(Node* pParent)
		{
			if (pParent->pLeft == nullptr)
			{
				return pParent;
			}
			else
			{
				return GetMinChild(pParent->pLeft);
			}
		}
		/// <summary>
		/// 해당 노드를 삭제합니다. 자식 노드가 있다면 모두 삭제합니다.
		/// </summary>
		void Delete(Node* pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			
			Node* pRight{ pNode->pRight };
			Node* pLeft{ pNode->pLeft };

			if (pNode->pRight != nullptr)
			{
				Delete(pNode->pRight);
			}
			if (pNode->pLeft != nullptr)
			{
				Delete(pNode->pLeft);
			}
			delete pNode;
		}
	};
}