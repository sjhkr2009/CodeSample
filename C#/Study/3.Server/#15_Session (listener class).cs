﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace ServerCore
{
	// 서버에서 클라이언트의 접속 요청을 받고, 이벤트 함수를 호출한다.

	class Listener
	{
		Socket _listenSocket;

		// 세션 생성 방식은 외부에서 정의할 수 있도록, Session을 반환하는 델리게이트를 받는다.
		Func<Session> _sessionFactory;
		
		public void Init(IPEndPoint endPoint, Func<Session> sessionFactory, int backlog = 10)
		{
			_listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			_listenSocket.Bind(endPoint);
			_listenSocket.Listen(backlog);

			_sessionFactory += sessionFactory;

			SocketAsyncEventArgs args = new SocketAsyncEventArgs();
			args.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
			RegisterAccept(args);
		}

		void RegisterAccept(SocketAsyncEventArgs args)
		{
			args.AcceptSocket = null;
			
			bool pending = _listenSocket.AcceptAsync(args);
			if (!pending)
				OnAcceptCompleted(null, args);
		}

		void OnAcceptCompleted(object sender, SocketAsyncEventArgs args)
		{
			if(args.SocketError == SocketError.Success)
			{
				// 기존과 같이 OnAcceptHandler를 컨텐츠 영역에서 구현하는 대신, 여기서 세션을 만들고 OnConnected를 호출한다.
				Session session = _sessionFactory();
				session.Init(args.AcceptSocket);
				session.OnConnected(args.AcceptSocket.RemoteEndPoint);
			}
			else Console.WriteLine(args.SocketError.ToString());

			RegisterAccept(args);
		}
	}
}
