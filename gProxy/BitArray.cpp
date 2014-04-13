#include "StdAfx.h"
#include "BitArray.h"


CBitArray::CBitArray(void)
{
	Store = 0;
}

bool CBitArray::Get( byte Location )
{
	if(Location >= 0 && Location <= 7)
	{
		switch(Location)
		{
		case 0:
			return ((Store & 1) == 1);
			break;

		case 1:
			return ((Store & 2) == 2);
			break;

		case 2:
			return ((Store & 4) == 4);
			break;

		case 3:
			return ((Store & 8) == 8);
			break;

		case 4:
			return ((Store & 16) == 16);
			break;

		case 5:
			return ((Store & 32) == 32);
			break;

		case 6:
			return ((Store & 64) == 64);
			break;

		case 7:
			return ((Store & 128) == 128);
			break;
		}
	}

	return false;
}

void CBitArray::Set( byte Location, bool Value )
{
	if(Value && Location >= 0 && Location <= 7)
	{
		switch(Location)
		{
		case 0:
			Store = (byte) (Store |  1);
			break;

		case 1:
			Store = (byte) (Store |  2);
			break;

		case 2:
			Store = (byte) (Store |  4);
			break;

		case 3:
			Store = (byte) (Store |  8);
			break;

		case 4:
			Store = (byte) (Store |  16);
			break;

		case 5:
			Store = (byte) (Store |  32);
			break;

		case 6:
			Store = (byte) (Store |  64);
			break;

		case 7:
			Store = (byte) (Store |  128);
			break;
		}
	}
}

byte CBitArray::Set( byte Location, bool Value, byte Base )
{
	if(Value && Location >= 0 && Location <= 7)
	{
		switch(Location)
		{
		case 0:
			return (byte) (Base |  1);
			break;

		case 1:
			return (byte) (Base |  2);
			break;

		case 2:
			return (byte) (Base |  4);
			break;

		case 3:
			return (byte) (Base |  8);
			break;

		case 4:
			return (byte) (Base |  16);
			break;

		case 5:
			return (byte) (Base |  32);
			break;

		case 6:
			return (byte) (Base |  64);
			break;

		case 7:
			return (byte) (Base |  128);
			break;
		}
	}

	return false;
}
