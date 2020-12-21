using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using ServerCore;

public enum PacketID 
{
	PlayerInfoReq = 1,
	test = 2,
	
}


class PlayerInfoReq
{
	public byte testByte;
	public long playerId;
	public string playerName;
	public struct Skills
	{
		public int id;
		public short level;
		public float duration;
		public struct Attribute
		{
			public int att;
		
			public bool Serialize(Span<byte> span, ref ushort count)
			{
				bool success = true;
		
				this.att = BitConverter.ToInt32(span.Slice(count, span.Length - count));
				count += sizeof(int);
		
				return success;
			}
		
			public void DeSerialize(ReadOnlySpan<byte> span, ref ushort count)
			{
				success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), att);
				count += sizeof(int);
			}
		}
		public List<Attribute> attribute = new List<Attribute>();
	
		public bool Serialize(Span<byte> span, ref ushort count)
		{
			bool success = true;
	
			this.id = BitConverter.ToInt32(span.Slice(count, span.Length - count));
			count += sizeof(int);
			this.level = BitConverter.ToInt16(span.Slice(count, span.Length - count));
			count += sizeof(short);
			this.duration = BitConverter.ToSingle(span.Slice(count, span.Length - count));
			count += sizeof(float);
			this.attributes.Clear();
			ushort attributeLen = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
			count += sizeof(ushort);
			for (int i = 0; i < attributeLen; i++)
			{
				Attribute attribute = new Attribute();
				attribute.DeSerialize(span, ref count);
				attributes.Add(attribute);
			}
	
			return success;
		}
	
		public void DeSerialize(ReadOnlySpan<byte> span, ref ushort count)
		{
			success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), id);
			count += sizeof(int);
			success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), level);
			count += sizeof(short);
			success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), duration);
			count += sizeof(float);
			success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort)attributes.Count);
			count += sizeof(ushort);
			foreach (Attribute attribute in attributes)
				success &= attribute.Serialize(span, ref count);
		}
	}
	public List<Skills> skills = new List<Skills>();

	public ArraySegment<byte> Serialize()
	{
		ArraySegment<byte> arr = SendBufferHelper.Open(4096);
		Span<byte> span = new Span<byte>(arr.Array, arr.Offset, arr.Count);

		ushort count = 0;
		bool success = true;

		count += sizeof(ushort);
		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort)PacketID.PlayerInfoReq);
		count += sizeof(ushort);
		
		this.testByte = (byte)arr.Array[arr.Offset + count];
		count += sizeof(byte);
		this.playerId = BitConverter.ToInt64(span.Slice(count, span.Length - count));
		count += sizeof(long);
		ushort playerNameLen = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
		count += sizeof(ushort);
		this.playerName = Encoding.Unicode.GetString(span.Slice(count, playerNameLen));
		count += playerNameLen;
		this.skillss.Clear();
		ushort skillsLen = BitConverter.ToUInt16(span.Slice(count, span.Length - count));
		count += sizeof(ushort);
		for (int i = 0; i < skillsLen; i++)
		{
			Skills skills = new Skills();
			skills.DeSerialize(span, ref count);
			skillss.Add(skills);
		}

		success &= BitConverter.TryWriteBytes(span, count);

		if (!success)
			return null;

		return SendBufferHelper.Close(count);
	}
	public void DeSerialize(ArraySegment<byte> arr)
	{
		ushort count = 0;
		count += sizeof(ushort);
		count += sizeof(ushort);

		ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(arr.Array, arr.Offset, arr.Count);

		arr.Array[arr.Offset + count] = (byte)this.testByte;
		count += sizeof(byte);
		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), playerId);
		count += sizeof(long);
		ushort playerNameLen = (ushort)Encoding.Unicode.GetBytes(playerName, 0, playerName.Length, arr.Array, arr.Offset + count + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), playerNameLen);
		count += sizeof(ushort);
		count += playerNameLen;
		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort)skillss.Count);
		count += sizeof(ushort);
		foreach (Skills skills in skillss)
			success &= skills.Serialize(span, ref count);
	}
}

class test
{
	public int testInt;

	public ArraySegment<byte> Serialize()
	{
		ArraySegment<byte> arr = SendBufferHelper.Open(4096);
		Span<byte> span = new Span<byte>(arr.Array, arr.Offset, arr.Count);

		ushort count = 0;
		bool success = true;

		count += sizeof(ushort);
		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), (ushort)PacketID.test);
		count += sizeof(ushort);
		
		this.testInt = BitConverter.ToInt32(span.Slice(count, span.Length - count));
		count += sizeof(int);

		success &= BitConverter.TryWriteBytes(span, count);

		if (!success)
			return null;

		return SendBufferHelper.Close(count);
	}
	public void DeSerialize(ArraySegment<byte> arr)
	{
		ushort count = 0;
		count += sizeof(ushort);
		count += sizeof(ushort);

		ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(arr.Array, arr.Offset, arr.Count);

		success &= BitConverter.TryWriteBytes(span.Slice(count, span.Length - count), testInt);
		count += sizeof(int);
	}
}

