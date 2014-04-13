#pragma once
#include "Structs.h"
#include "PRW.h"
#include "IniReader.h"
#include "Functions.h"

class DMap
{
public:
	static map<UInt16, string> MapID2Name;
	static map<string, gMap*> AllMaps; 
	static vector<string> MapNames;

	static void LoadMaps(string GameMapFile)
	{
		ifstream file (GameMapFile, ios::in|ios::binary|ios::ate);
		ifstream::pos_type size;
		CBuffer memblock;

		if (file.is_open())
		{
			size = file.tellg();
			memblock = CBuffer(size);
			file.seekg (0, ios::beg);
			file.read ((char*) memblock, size);
			file.close();

			PRW Reader(memblock);
			UInt32 MapCount = Reader.Read<UInt32>();

			for(int i = 0; i < MapCount; i++)
			{
				UInt32 MapID =  Reader.Read<UInt32>();
				UInt32 NameLen = Reader.Read<UInt32>();
				string MapName = Functions::Replace(Reader.ReadString(NameLen), ".7z", ".dmap");
				MapID2Name.insert(pair<UInt16, string>(MapID, MapName));
				
				vector<string>::iterator it = MapNames.begin();
				if(find(MapNames.begin(), MapNames.end(), MapName) == MapNames.end())
					it = MapNames.insert(it, MapName);
				Reader.Position += 4;
			}

			for(vector<string>::iterator it = MapNames.begin(); it != MapNames.end(); it++)
			{
				gMap* Map = LoadMap(*it);
				if(Map != NULL)
				{
					AllMaps.insert(pair<string, gMap*>(*it, Map));
				}
			}
		}
	}

	static gMap* LoadMap(UInt16 MapID)
	{
// 		CIniReader iReader("./Settings.ini");
// 		string COPath = iReader.ReadString("Settings", "COPath", "NONE");
// 		if(MapID2Name.find(MapID) != MapID2Name.end())
// 		{
// 			ifstream file (COPath + "\\" + MapID2Name[MapID], ios::in|ios::binary|ios::ate);
// 			ifstream::pos_type size;
// 			CBuffer memblock;
// 			if (file.is_open())
// 			{
// 				size = file.tellg();
// 				memblock = CBuffer(size);
// 				file.seekg (0, ios::beg);
// 				file.read ((char*) memblock, size);
// 				file.close();
// 				PRW Reader(memblock);
// 				Reader.Position += 268;
// 				UInt32 xCount = Reader.Read<UInt32>();
// 				UInt32 yCount = Reader.Read<UInt32>();
// 				gMap* Map = new gMap;
// 				memset((void*) Map->Name, 0, 100);
// 
// 				if(MapID2Name.find(MapID) != MapID2Name.end())
// 					memcpy((void*) Map->Name, MapID2Name[MapID].c_str(), MapID2Name[MapID].length());
// 
// 
// 				Map->Width = xCount;
// 				Map->Height = yCount;
// 				Map->Accessibility = new byte[(int) ceil(((double)(Map->Width * Map->Height) / 8))];
// 				memset(Map->Accessibility, 0, (int) ceil(((double)(Map->Width * Map->Height) / 8)));
// 				for (ushort y = 0; y < yCount; y++)
// 				{
// 					for (ushort x = 0; x < xCount; x++)
// 					{
// 						double Pos = (y * Map->Width) + x;
// 						Map->Accessibility[(int) floor(Pos / 8)] = CBitArray::Set((int)Pos % 8, (Reader.Read<UInt16>() == 1) ? false : true, Map->Accessibility[(int) floor(Pos / 8)]);
// 						Reader.Position += 4;
// 					}
// 					Reader.Position += 4;
// 				}
// 
// 				return Map;
// 			}
// 		}

		if(MapID2Name.find(MapID) != MapID2Name.end())
		{
			if(AllMaps.find(MapID2Name[MapID]) != AllMaps.end())
			{
				return AllMaps[MapID2Name[MapID]];
			}
		}

		return NULL;
	}

	static gMap* LoadMap(string MapName)
	{
		CIniReader iReader("./Settings.ini");
		string COPath = iReader.ReadString("Settings", "COPath", "NONE");

		ifstream file (COPath + "\\" + MapName, ios::in|ios::binary|ios::ate);
		ifstream::pos_type size;
		CBuffer memblock;
		if (file.is_open())
		{
			size = file.tellg();
			memblock = CBuffer(size);
			file.seekg (0, ios::beg);
			file.read ((char*) memblock, size);
			file.close();
			PRW Reader(memblock);
			Reader.Position += 268;
			UInt32 xCount = Reader.Read<UInt32>();
			UInt32 yCount = Reader.Read<UInt32>();
			gMap* Map = new gMap;
			memset((void*) Map->Name, 0, 100);
			memcpy((void*) Map->Name, MapName.c_str(), MapName.length());

			Map->Width = xCount;
			Map->Height = yCount;
			Map->Accessibility = new byte[(int) ceil(((double)(Map->Width * Map->Height) / 8))];
			memset(Map->Accessibility, 0, (int) ceil(((double)(Map->Width * Map->Height) / 8)));
			for (ushort y = 0; y < yCount; y++)
			{
				for (ushort x = 0; x < xCount; x++)
				{
					double Pos = (y * Map->Width) + x;
					Map->Accessibility[(int) floor(Pos / 8)] = CBitArray::Set((int)Pos % 8, (Reader.Read<UInt16>() == 1) ? false : true, Map->Accessibility[(int) floor(Pos / 8)]);
					Reader.Position += 4;
				}
				Reader.Position += 4;
			}

			return Map;
		}

		return NULL;
	}
};

