#pragma once

#include <iostream>

namespace MyTree
{
	/// <summary>
	/// Ʈ���� ��ҿ� �ش��ϴ� ��� ����ü
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
	/// ���� Ž�� Ʈ��
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
		/// Ʈ���� ��ȸ�ϴ� �Լ��Դϴ�. �⺻������ In-Order ���� ��ȸ�մϴ�.
		/// </summary>
		/// <param name="mode">���� Ž��(Pre-Order) �Ǵ� ���� Ž��(Post-Order)�� ���� ����ϴ� ����</param>
		void Traversal(TraversalMode mode = InOrder)
		{
			if (mpRoot == nullptr)
			{
				std::cout << "Ʈ���� ��� �ֽ��ϴ�." << std::endl;
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
		/// Ʈ���� ���� �߰��ϰ� �ڵ����� �����մϴ�. �ߺ��� ���� �߰����� �ʽ��ϴ�.
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
					std::cout << "�ߺ��� ���� �Է��߽��ϴ�." << std::endl;
					return;
				}
			}
		}

		/// <summary>
		/// Ư���� ���� ���� ��带 ã�� ��ȯ�մϴ�.
		/// </summary>
		Node* Find(int data)
		{
			Node* pResult{ _Find(data) };

			if (pResult == nullptr)
			{
				std::cout << "�ش� ���� Ʈ���� �����ϴ�." << std::endl;
			}
			else
			{
				std::cout << pResult->mData << "�� Ž����" << std::endl;
			}
			return pResult;
		}

		/// <summary>
		/// Ư���� ���� ���� ��带 ã�� �����մϴ�. �ڽ� ��尡 1���� ��� ������ �ڸ��� �ش� ���� ��ü�˴ϴ�.
		/// �ڽ� ��尡 2���� ��� ���� Ʈ������ �����Ϸ��� ���� ���� ������ ���� ���� ���� ���� ��ü�մϴ�.
		/// </summary>
		bool Erase(int data)
		{
			Node* pTarget{ _Find(data) };
			Node* pParent{ _Find(data, true) };
			if (pTarget == nullptr)
			{
				std::cout << "�����Ϸ��� ���� �������� �ʽ��ϴ�." << std::endl;
				return false;
			}

			if ((pTarget->pLeft == nullptr) && (pTarget->pRight == nullptr))
			{
				//�ڽ��� ���� ��
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
				// �ڽ��� �ϳ��� �� (������)
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
				// �ڽ��� �ϳ��� �� (����)
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
				// �ڽ��� ���� ��
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
		/// ������ ��� ��Ҹ� �����ϰ�, ��Ʈ ��带 nullptr�� �ʱ�ȭ�մϴ�.
		/// </summary>
		void Clear()
		{
			Delete(mpRoot);
			mpRoot = nullptr;
		}


	private:
		/// <summary>
		/// Ư�� ���� ������ ��带 �����Ͽ� ��ȯ�մϴ�.
		/// </summary>
		Node* CreateNode(int data) { return new Node(data); }
		/// <summary>
		/// Ư�� ��带 Ž���մϴ�. ��尡 ���� ���� ����մϴ�.
		/// </summary>
		void Visit(Node* pNode) { std::cout << pNode->mData << " "; }

		/// <summary>
		/// Ʈ���� ���� ��ȸ�մϴ�.
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
		/// Ʈ���� ���� ��ȸ�մϴ�. ���� ������ ���ĵ� ���·� ��µ˴ϴ�.
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
		/// Ʈ���� ���� ��ȸ�մϴ�.
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
		/// Ư���� ���� ���� ��带 ã�� ��ȯ�մϴ�.
		/// �� ���� ���ڷ� true�� �Էµ� ��� �ش� ����� �θ� ��带 ��ȯ�մϴ�.
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
		/// �ڽ� ��� �� ���� ū ���� ���� ��带 ã�� ��ȯ�մϴ�.
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
		/// �ڽ� ��� �� ���� ���� ���� ���� ��带 ã�� ��ȯ�մϴ�.
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
		/// �ش� ��带 �����մϴ�. �ڽ� ��尡 �ִٸ� ��� �����մϴ�.
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