using System;
using System.IO;
using System.Xml;

namespace PacketGenerator
{
	class Program
	{
		// 패킷의 내용이 들어갈 string 자료형
		static string genPackets;
		// 패킷 ID들이 나열된 enum이 들어갈 string
		static string packetEnums;
		// 패킷 ID로 쓰일 정수. 새로운 패킷을 파싱할 때마다 1을 증가시킨 후, 패킷명과 이 값을 enum에 추가한다.
		static ushort packetId;

		static void Main(string[] args)
		{
			// XmlReader로 파일을 읽어올 수 있다.
			// 추상 클래스이므로 직접 인스턴스화할 수 없으며, Create를 통해 파일 경로를 입력하여 생성한다.

			// 생성 시 옵션으로 XmlReaderSettings를 지정할 수 있다.
			XmlReaderSettings settings = new XmlReaderSettings()
			{
				IgnoreComments = true,	// 주석(//)을 무시한다.
				IgnoreWhitespace = true	// 공백(스페이스 바)을 무시한다.
			};

			// XML 파일을 열고, 사용한 후에는 pdl.Dispose() 와 같이 닫는 작업이 필요하다.
			// 단, 아래와 같이 using을 이용하여 범위를 지정하면 범위를 벗어날 때 알아서 Dispose가 호출된다.
			using (XmlReader pdl = XmlReader.Create("PDL.xml", settings))
			// 파일 경로는 실행 파일(.exe)이 있는 곳을 기준으로 입력한다.
			// 여기선 임의로 xml 파일을 실행 파일이 있는 PacketGenerator\bin\Debug\netcoreapp3.1 로 옮겨서 테스트해본다.
			{
				pdl.MoveToContent();	// 헤더를 건너뛰고 내용이 있는 부분으로 커서를 옮긴다.

				while(pdl.Read())
				{
					// .Depth : 현재 읽는 부분의 깊이. 0에서 시작하여 xml 파일의 꺾쇠가 열릴 때마다 +1, 닫힐 때마다 -1
					//			여기서는 <packet> 부분이 해당된다.
					// .NodeType :  현재 읽는 부분의 노드 타입. 정보가 시작되는 부분이면 Element, 끝나는 부분이면 EndElement가 된다.
					//				여기서는 packet의 꺾쇠가 열릴 때만 ParsePacket() 으로 넘긴다.
					if (pdl.Depth == 1 && pdl.NodeType == XmlNodeType.Element)
						ParsePacket(pdl);

					//Console.WriteLine(pdl.Name + " " + pdl["name"]);
				}

				// 파싱이 끝났으면 fileText에 파일 포맷에 맞게 내용을 채우고, 파일을 생성한다.
				// 파일 생성 위치는 exe 파일이 있는 위치와 동일하다.

				string fileText = string.Format(PacketFormat.fileFormat, packetEnums, genPackets);
				File.WriteAllText("GenPackets.cs", fileText);
			}

		}

		// <packet>에 진입했을 때 호출된다. 모든 멤버를 파싱하여 genPackets에 넣는다.
		public static void ParsePacket(XmlReader xml)
		{
			// 노드를 닫는 부분(</packet>)이 들어왔다면 return 처리
			if (xml.NodeType == XmlNodeType.EndElement)
				return;

			// 소문자로 읽은 노드 이름이 의도했던 "packet"이 아닐 경우 return 처리
			if (xml.Name.ToLower() != "packet")
			{
				Console.WriteLine("Invalid packet node.");
				return;
			}

			// 클래스명이 될 패킷 이름을 읽고, 비어있다면 return 처리
			string packetName = xml["name"];
			if(string.IsNullOrEmpty(packetName))
			{
				Console.WriteLine("Packet without name.");
				return;
			}

			Tuple<string, string, string> t = ParseMembers(xml);
			genPackets += string.Format(PacketFormat.packetFormat, packetName, t.Item1, t.Item2, t.Item3);

			// 패킷 ID를 enum에 추가한다. (+추가 후 줄바꿈 및 들여쓰기 처리)
			packetEnums += string.Format(PacketFormat.packetEnumFormat, packetName, ++packetId) + Environment.NewLine + "\t";
		}

		// ParsePacket에서 호출된다. 현재 읽은 지점의 모든 내부 멤버를 파싱하여 멤버 변수명, 직렬화, 역직렬화 포맷으로 반환한다.
		private static Tuple<string, string, string> ParseMembers(XmlReader xml)
		{
			string packetName = xml["name"];

			string memberCode = "";
			string readCode = "";
			string writeCode = "";

			// <packet> 안쪽의 내용들이 파싱의 대상이므로 현재 읽는 부분의 내부 depth만 파싱한다.
			int depth = xml.Depth + 1;

			while (xml.Read())
			{
				// 멤버를 모두 읽어 </packet> 으로 나왔다면 break
				if (xml.Depth != depth)
					break;

				string memberName = xml["name"];
				if(string.IsNullOrEmpty(memberName))
				{
					Console.WriteLine("Member without name.");
					return null;
				}

				// 각 포맷은 멤버를 읽을 때마다 줄바꿈을 해 준다. 최초로 읽는 경우는 비어 있을테니 호출되지 않는다.
				if (!string.IsNullOrEmpty(memberCode))
					memberCode += Environment.NewLine;
				if (!string.IsNullOrEmpty(readCode))
					readCode += Environment.NewLine;
				if (!string.IsNullOrEmpty(writeCode))
					writeCode += Environment.NewLine;

				// 안전을 위해 타입 이름은 모두 소문자로 바꿔서 읽는다.
				string memberType = xml.Name.ToLower();

				// 타입에 따른 처리. 기본 자료형일 경우, 변환이 불필요한 byte, 가변 문자열 string, 그리고 리스트일 경우로 나뉜다.
				// (리스트의 요소는 모두 구조체 타입으로 파싱 방법이 별도로 정의되어 있음)
				switch (memberType)
				{
					case "byte":
					case "sbyte":
						memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
						readCode += string.Format(PacketFormat.readByteFormat, memberName, memberType);
						writeCode += string.Format(PacketFormat.writeByteFormat, memberName, memberType);
						break;
					case "bool":
					case "short":
					case "ushort":
					case "int":
					case "long":
					case "float":
					case "double":
						memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
						readCode += string.Format(PacketFormat.readFormat, memberName, ToConvertFuncName(memberType), memberType);
						writeCode += string.Format(PacketFormat.writeFormat, memberName, memberType);
						break;
					case "string":
						memberCode += string.Format(PacketFormat.memberFormat, memberType, memberName);
						readCode += string.Format(PacketFormat.readStringFormat, memberName);
						writeCode += string.Format(PacketFormat.writeStringFormat, memberName);
						break;
					case "list":
						Tuple<string, string, string> t = ParseStructList(xml);
						memberCode += t.Item1;
						readCode += t.Item2;
						writeCode += t.Item3;
						break;
					default:
						break;
				}
			}

			// 포맷에 적합하게 들여쓰기 처리를 한다. (변수 선언부는 1칸, 멤버 함수 내부는 2칸)
			memberCode = memberCode.Replace("\n", "\n\t");
			readCode = readCode.Replace("\n", "\n\t\t");
			writeCode = writeCode.Replace("\n", "\n\t\t");

			return new Tuple<string, string, string>(memberCode, readCode, writeCode);
		}

		// 리스트 내부를 읽어서, 선언부와 직렬화/역직렬화 포맷에 맞게 파싱하여 반환한다.
		// 리스트의 요소를 파싱하는 방법은 포맷에 따라 선언부에서 정의되어 있다.
		private static Tuple<string, string, string> ParseStructList(XmlReader xml)
		{
			string listName = xml["name"];
			if(string.IsNullOrEmpty(listName))
			{
				Console.WriteLine("List without name.");
				return null;
			}

			// ParseMembers로 리스트 내부를 파싱하고, 리스트 포맷에 맞게 변환한다.
			Tuple<string, string, string> t = ParseMembers(xml);

			string memberCode = string.Format(PacketFormat.memberListFormat,
				FirstCharToUpper(listName),
				FirstCharToLower(listName),
				t.Item1,
				t.Item2,
				t.Item3
			);

			string readCode = string.Format(PacketFormat.readListFormat,
				FirstCharToUpper(listName),
				FirstCharToLower(listName)
			);

			string writeCode = string.Format(PacketFormat.writeListFormat,
				FirstCharToUpper(listName),
				FirstCharToLower(listName)
			);

			return new Tuple<string, string, string>(memberCode, readCode, writeCode);
		}

		// 타입 이름을 입력받아 To~ 계열의 비트 변환 함수명을 반환한다.
		// 변환 함수가 없는 타입(이미 byte인 경우 포함)이면 빈 문자열을 반환한다.
		public static string ToConvertFuncName(string typeName)
		{
			switch (typeName)
			{
				case "bool":
					return "ToBoolean";
				case "short":
					return "ToInt16";
				case "ushort":
					return "ToUInt16";
				case "int":
					return "ToInt32";
				case "long":
					return "ToInt64";
				case "float":
					return "ToSingle";
				case "double":
					return "ToDouble";

				case "byte": default:
					return "";
			}
		}

		// 문자열의 첫 글자를 대문자로 바꿔 반환한다.
		// 타입명 등 파스칼 표기법에서 사용된다.
		public static string FirstCharToUpper(string input)
		{
			if (string.IsNullOrEmpty(input))
				return "";

			return input[0].ToString().ToUpper() + input.Substring(1);
		}
		// 문자열의 첫 글자를 소문자로 바꿔 반환한다.
		// 변수 등 카멜 표기법에서 사용된다.
		public static string FirstCharToLower(string input)
		{
			if (string.IsNullOrEmpty(input))
				return "";

			return input[0].ToString().ToLower() + input.Substring(1);
		}
	}
}
